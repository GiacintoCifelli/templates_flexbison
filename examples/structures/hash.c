#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

hashmap_t *new_hashmap(long initial_size, long growth, void *(*get_key)(void *record), uint32_t (*calc_hash)(void *record), int (*compare)(void *keyA, void *keyB), void (*free_record)(void *record)) {
	if(!get_key || !calc_hash || !compare) return NULL;
	hashmap_t *map = calloc(1, sizeof(hashmap_t));
	map->size = initial_size;
	map->growth = growth;
	map->current = 0;
	map->collisions = 0;
	map->get_key = get_key;
	map->calc_hash = calc_hash;
	map->compare = compare;
	map->free_record = free_record;
	map->hash = calloc(map->size, sizeof(hash_element_t));
	return map;
}

void free_hashmap(hashmap_t *map) {
	for(long i=0; i<map->size; i++) {
		if(map->hash[i].record) {
			if(map->free_record)
				map->free_record(map->hash[i].record);
			for(hash_element_t *m = map->hash[i].next; m;) {
				if(map->free_record)
					map->free_record(m->record);
				hash_element_t *m2 = m->next;
				free(m);
				m = m2;
			}
		}
	}
	free(map->hash);
	free(map);
}

static void *hashmap_get_key(hashmap_t *map, void *record) {
	if(!record) return NULL;
	return map->get_key(record);
}

static int hashmap_compare(hashmap_t *map, void *keyA, void *keyB) {
	if(!keyA) {
		if(!keyB) return 0;
		return -1;
	}
	if(!keyB) return 1;
	return map->compare(keyA, keyB);
}

void resize_hashmap(hashmap_t* map, long new_size) {
	hashmap_t *new_map = new_hashmap(new_size, 0, map->get_key, map->calc_hash, map->compare, map->free_record);
	map->collisions = 0;
	for(long i=0;i<map->size;i++) {
		hashmap_add_record(new_map, map->hash[i].record);
		hash_element_t *m = &map->hash[i];
		while(m->next) {
			hashmap_add_record(new_map, m->next->record);
			m = m->next;
		}
	}
	for(long i=0; i<map->size; i++) {
		if(map->hash[i].record) {
			for(hash_element_t *m = map->hash[i].next; m;) {
				hash_element_t *m2 = m->next;
				free(m);
				m = m2;
			}
		}
	}
	free(map->hash);
	map->hash = new_map->hash;
	map->size = new_map->size;
	free(new_map);
}

void hashmap_add_record(hashmap_t *map, void *record) {
	if(!record) return;
	if(map->current+1 > map->size*80/100) { /* hash tables are efficient up to 80% full: consider resize now! */
		if(map->growth<-1)
			resize_hashmap(map, map->size * (-map->growth));
		else if(map->growth>0)
			resize_hashmap(map, map->size + map->growth);
	}
	map->current++;
	uint32_t h = map->calc_hash(map->get_key(record));
	uint32_t slot = h % map->size;
	if(!map->hash[slot].record) {
		map->hash[slot].record = record;
	} else {
		map->collisions++;
		if(hashmap_compare(map, hashmap_get_key(map, map->hash[slot].record), hashmap_get_key(map, record)) == 0) return; /* already inserted; */
		hash_element_t *m = &map->hash[slot];
		while(m->next) {
			if(hashmap_compare(map, hashmap_get_key(map, m->next->record), hashmap_get_key(map, record)) == 0) return; /* already inserted; */
			m = m->next;
		}
		m->next = calloc(1, sizeof(hash_element_t));
		m->next->record = record;
	}
}

void *hashmap_get_record(hashmap_t *map, void *key) {
	uint32_t h = map->calc_hash(key);
	uint32_t slot = h % map->size;
	if(!map->hash[slot].record) return NULL;
	if(hashmap_compare(map, hashmap_get_key(map, map->hash[slot].record), key) == 0) return map->hash[slot].record;
	hash_element_t *m = &map->hash[slot];
	while(m->next) {
		if(hashmap_compare(map, hashmap_get_key(map, m->next->record), key) == 0) return m->next->record;
		m = m->next;
	}
	return NULL;
}

void **get_array(hashmap_t *map, long *size) {
	*size = map->current;
	if(!map->current) return NULL;
	void **array = calloc(map->current, sizeof(void*));
	long n = 0;
	for(long i=0;i<map->size;i++) {
		if(map->hash[i].record)
			array[n++] = map->hash[i].record;
		hash_element_t *m = &map->hash[i];
		while(m->next) {
			if(m->next->record)
				array[n++] = m->next->record;
			m = m->next;
		}
	}
	return array;
}

void sort_array(void **array, long size, hashmap_t *map) {
	if(!array) return;
	int resolve_compare(const void *a, const void *b) {
		int ret = map->compare(map->get_key(*(void**)a), map->get_key(*(void**)b));
		return ret;
	}
	qsort((void *)array, size, sizeof(void*), resolve_compare);
}

/************************************************************************/
uint32_t calc_hash_string(const char *key, uint32_t seed) {
	const uint8_t* name = (const uint8_t*)key;
	uint32_t h = seed;
	for (; *name; name++)
		h = (h << 5) + h + *name;
	return h;
}

uint32_t calc_hash_binary(const uint8_t *key, int length, long seed) {
	uint32_t h = seed;
	for (int i=0; i<length; i++)
		h = h*9 ^ *key;
	return h;
}

/************************************************************************/
uint32_t hash_string(void *key) {
	char *kv_key = key;
	return calc_hash_string(kv_key, HASHMAP_INIT_SEED);
}

int compare_strings(void *keyA, void *keyB) {
	return strcmp((char*)keyA, (char*)keyB);
}

void *string_get_key(void *record) {
	return record;
}

/************************************************************************/
kv_t *new_kv(const char *key, const char *value) {
	kv_t *kv = calloc(1, sizeof(kv_t));
	kv->key = strdup(key?key:"");
	if(value) kv->value = strdup(value);
	return kv;
}

kv_t *kv_set_value(kv_t *kv, const char *value) {
	free(kv->value);
	kv->value = NULL;
	if(value) kv->value = strdup(value);
}

void *kv_get_key(void *record) {
	kv_t *kv = record;
	return kv->key;
}


void free_kv(void *record) {
	kv_t *kv = record;
	free(kv->key);
	free(kv->value);
	free(kv);
}
