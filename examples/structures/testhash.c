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

int main(void) {
	test_keyvalues();
	test_load();
	test_sort();
	return 0;
}
