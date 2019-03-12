/****************************************************************************************************

double-linked structure and functions

****************************************************************************************************/

#ifndef _DLLIST_H_
#define _DLLIST_H_

typedef struct list_element_t list_element_t;
struct list_element_t {
	void *record;	/* record stored in the hash map */
	list_element_t *prev;
	list_element_t *next;
};

/* unsorted list with repetitions */
typedef struct {
	long size;
	list_element_t *first;
	list_element_t *last;
	void *(*get_key)(void *record);
	int (*compare)(void *recordA, void *recordB);
	void (*free_record)(void *record); /* if given, the records will be freed on free_hashmap */
} dllist_t;

dllist_t *new_dllist(void *(*get_key)(void *record), int (*compare)(void *keyA, void *keyB), void(*free_record)(void *record));
void free_dllist(dllist_t *dllist);

void dllist_append(dllist_t *dllist, void *record);
void dllist_prepend(dllist_t *dllist, void *record);

void dllist_remove_head(dllist_t *dllist);
void dllist_remove_tail(dllist_t *dllist);
void dllist_remove_elem(dllist_t *dllist, list_element_t* elem);

list_element_t *dllist_find_elem_first(dllist_t *dllist, void *key);
list_element_t *dllist_find_elem_next(dllist_t *dllist, void *key, list_element_t *pos);
void *dllist_find_first(dllist_t *dllist, void *key, list_element_t **pos);
void *dllist_find_next(dllist_t *dllist, void *key, list_element_t **pos);
void dllist_remove_first(dllist_t *dllist, void *key);
void dllist_remove_after(dllist_t *dllist, void *key, list_element_t *pos);

list_element_t *dllist_find_elem_last(dllist_t *dllist, void *key);
list_element_t *dllist_find_elem_prev(dllist_t *dllist, void *key, list_element_t *pos);
void *dllist_find_last(dllist_t *dllist, void *key, list_element_t **pos);
void *dllist_find_prev(dllist_t *dllist, void *key, list_element_t **pos);
void dllist_remove_last(dllist_t *dllist, void *key);
void dllist_remove_before(dllist_t *dllist, void *key, list_element_t *pos);

void dllist_remove_all(dllist_t *dllist, void *key);

void **get_array_from_dllist(dllist_t *dllist, long *size);
void sort_array_from_dllist(void **array, long size, dllist_t *dllist);

#endif /* _DLLIST_H_ */
