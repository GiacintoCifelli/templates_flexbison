/****************************************************************************************************

hashmap structure and functions

****************************************************************************************************/

#ifndef _HASH_H_
#define _HASH_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct hash_element_t hash_element_t;
struct hash_element_t{
	void *record;	/* record stored in the hash map */
	hash_element_t *next;
};

#define HASHMAP_INIT_SIZE_DEFAULT	(1024*10)
#define HASHMAP_INIT_SIZE_SMALL		(1024) /* consider using a list! */
#define HASHMAP_GROWTH_DOUBLE		(-2)
#define HASHMAP_GROWTH_DISABLED		(0)

#define HASHMAP_INIT_SEED		(5381)

typedef struct {
	long size;
	long growth;
	long current;
	long collisions;
	void *(*get_key)(void *record);
	uint32_t (*calc_hash)(void *key);
	int (*compare)(void *recordA, void *recordB);
	void (*free_record)(void *record); /* if given, the records will be freed on free_hashmap */
	hash_element_t *hash;
} hashmap_t;

hashmap_t *new_hashmap(long initial_size, long growth, void *(*get_key)(void *record), uint32_t (*calc_hash)(void *record), int (*compare)(void *keyA, void *keyB), void (*free_record)(void *record));
void free_hashmap(hashmap_t *map);
void resize_hashmap(hashmap_t* map, long new_size);
void hashmap_add_record(hashmap_t *map, void *record); /* the function calc_hash will extract the key */
void hashmap_remove_record(hashmap_t *map, void *key);
void *hashmap_get_record(hashmap_t *map, void *key);

void **get_array_from_hashmap(hashmap_t *map, long *size);
void sort_array(void **array, long size, void *(*get_key)(void *record), int (*compare)(void *recordA, void *recordB));
void sort_array_from_hashmap(void **array, long size, hashmap_t *map);

/* standard hash functions */

uint32_t calc_hash_binary(const uint8_t *key, int length, long seed);
uint32_t calc_hash_string(const char *key, uint32_t seed);

/* string-only record (presence check) */

void *string_get_key(void *record);
uint32_t hash_string(void *record);
int compare_strings(void *keyA, void *keyB);

/* key-value record */

typedef struct {
	char *key;
	char *value;
} kv_t;

kv_t *new_kv(const char *key, const char *value);
void free_kv(void *record);
kv_t *kv_set_value(kv_t *kv, const char *value);
void *kv_get_key(void *record);
/* hash and compare from string-only record */

#endif /* _HASH_H_ */
