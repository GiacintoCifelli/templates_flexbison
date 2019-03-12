/**********************************************************************************************
build with:
	gcc -g -o testhash hash.c testhash.c

run with:
	valgrind --leak-check=full ./testhash

expected program output when run with: ./testast:
int: 5
string: 'aooo'

**********************************************************************************************/

#include <stdio.h>
#include <string.h>

#include "hash.h"
#include "dllist.h"

void test_keyvalues() {
	hashmap_t* map = new_hashmap(HASHMAP_INIT_SIZE_DEFAULT, HASHMAP_GROWTH_DOUBLE, kv_get_key, hash_string, compare_strings, free_kv);
	hashmap_get_record(map, "test");
	kv_t *kv = new_kv("name", "yo!");
	hashmap_add_record(map, kv);
	kv = hashmap_get_record(map, "name");
	if(kv)
		kv_set_value(kv, "aoooooooooooo");
	free_hashmap(map);
}

void test_load() {
	char str[1024];
	hashmap_t* map = new_hashmap(HASHMAP_INIT_SIZE_DEFAULT, HASHMAP_GROWTH_DOUBLE, string_get_key, hash_string, compare_strings, free); // yelds: collisions final: 15824 (0.09/elem)
	//hashmap_t* map = new_hashmap(HASHMAP_INIT_SIZE_DEFAULT, HASHMAP_GROWTH_DISABLED, string_get_key, hash_string, compare_strings, free); // yelds: collisions final: 89663 (8.75/elem)
	//hashmap_t* map = new_hashmap(HASHMAP_INIT_SIZE_SMALL, HASHMAP_GROWTH_DISABLED, string_get_key, hash_string, compare_strings, free); // yelds: collisions final: 83378 (81.42/elem). sensibly slower with valgrind (3 seconds instead of 1)

	FILE *f = fopen("wiki-100k.txt", "r"); /* the word list comes from: https://gist.githubusercontent.com/h3xx/1976236/raw/bbabb412261386673eff521dddbe1dc815373b1d/wiki-100k.txt */
	for(int i=0;!feof(f);i++) {
		fgets(str, 1024, f);
		if(!hashmap_get_record(map, str))
			hashmap_add_record(map, strdup(str));
	}
	printf("collisions final: %ld (%ld.%02ld/elem)\n", map->collisions, map->collisions/map->size, (map->collisions*100/map->size)%100);
	fclose(f);
	free_hashmap(map);
}

void test_sort() {
	hashmap_t* map = new_hashmap(HASHMAP_INIT_SIZE_SMALL, HASHMAP_GROWTH_DISABLED, string_get_key, hash_string, compare_strings, NULL);
	hashmap_add_record(map, "apples");
	hashmap_add_record(map, "oranges");
	hashmap_add_record(map, "boxes");
	hashmap_add_record(map, "shoes");
	long arraysize;
	void **array = get_array_from_hashmap(map, &arraysize);
	sort_array_from_hashmap(array, arraysize, map);
	char **sortedstrings = (char**)array;
	for(int i=0;i<arraysize;i++)
		printf("%3d: %s\n", i, sortedstrings[i]);
	free(array);
	free_hashmap(map);
}

void dllist_walk(dllist_t *dllist) {
	printf("walking %p\n", dllist);
	if(!dllist) return;
	list_element_t *elem = dllist->first;
	int i=0;
	while(elem) {
		printf("%3d: '%s'\n", i++, (char*)elem->record);
		elem = elem->next;
	}
}

int main(void) {
	dllist_t *dllist = new_dllist(string_get_key, compare_strings, free);
	dllist_walk(dllist);
	free_dllist(dllist);
	dllist = new_dllist(string_get_key, compare_strings, free);
	dllist_append(dllist, strdup("aaaa"));
	dllist_append(dllist, strdup("bbbb"));
	dllist_append(dllist, strdup("cccc"));
	dllist_append(dllist, strdup("dddd"));
	dllist_walk(dllist);
	list_element_t *elem = dllist_find_elem_first(dllist, "bbbb");
	dllist_remove_elem(dllist, elem);
	dllist_remove_tail(dllist);
	dllist_walk(dllist);
	elem = dllist_find_elem_first(dllist, "cccc");
	dllist_remove_elem(dllist, elem);
	dllist_remove_head(dllist);
	dllist_remove_tail(dllist);
	dllist_walk(dllist);
	dllist_append(dllist, strdup("aaaa"));
	dllist_append(dllist, strdup("bbbb"));
	dllist_append(dllist, strdup("cccc"));
	dllist_append(dllist, strdup("dddd"));
	elem = dllist_find_elem_first(dllist, "cccc");
	dllist_remove_elem(dllist, elem);
	dllist_remove_head(dllist);
	dllist_remove_tail(dllist);
	dllist_walk(dllist);
	char *s = dllist_find_first(dllist, "bbbb", NULL);
	printf("found: %s\n", s);
	s = dllist_find_first(dllist, "nothere", NULL);
	printf("found: %s\n", s);
	free_dllist(dllist);
	dllist = new_dllist(string_get_key, compare_strings, free);
	dllist_append(dllist, strdup("bbbb"));
	dllist_append(dllist, strdup("dddd"));
	dllist_append(dllist, strdup("aaaa"));
	dllist_append(dllist, strdup("cccc"));
	dllist_append(dllist, strdup("dddd"));
	s = dllist_find_first(dllist, "dddd", &elem);
	printf("%p=%s\n", s, s);
	s = dllist_find_next(dllist, "dddd", &elem);
	printf("%p=%s\n", s, s);
	s = dllist_find_next(dllist, "dddd", &elem);
	printf("%p=%s << expected null because no more matches\n", s, s);
	s = dllist_find_next(dllist, "dddd", &elem);
	printf("%p=%s << expected first match again...\n", s, s);

	long size;
	char **array = (char**)get_array_from_dllist(dllist, &size);
	printf("printing array %p\n", array);
	if(array) {
		printf("dllist->size=%ld, size=%ld\n", dllist->size, size);
		for(long i=0;i<size;i++)
			printf("%3ld: '%s'\n", i, array[i]);
	}
	printf("printing sorted array %p\n", array);
	if(array) {
		sort_array_from_dllist((void**)array, size, dllist);
		for(long i=0;i<size;i++)
			printf("%3ld: '%s'\n", i, array[i]);
	}
	free(array);
	dllist_remove_first(dllist, "bbbb");
	dllist_walk(dllist);
	dllist_remove_all(dllist, "dddd");
	dllist_walk(dllist);
	free_dllist(dllist);
	return 0;
}
