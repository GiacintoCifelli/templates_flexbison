#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"
#include "hash.h"

dllist_t *new_dllist(void *(*get_key)(void *record), int (*compare)(void *keyA, void *keyB), void(*free_record)(void *record)) {
	dllist_t *dllist = calloc(1, sizeof(dllist_t));
	dllist->get_key = get_key;
	dllist->compare = compare;
	dllist->free_record = free_record;
}

void free_dllist(dllist_t *dllist) {
	if(!dllist) return;
	list_element_t *elem = dllist->first;
	while(elem) {
		list_element_t *next = elem->next;
		if(dllist->free_record)
			dllist->free_record(elem->record);
		free(elem);
		elem=next;
	}
	free(dllist);
}

void dllist_append(dllist_t *dllist, void *record) {
	if(!dllist) return;
	list_element_t *newelem = calloc(1, sizeof(list_element_t));
	newelem->record = record;
	if(!dllist->last)
		dllist->first = dllist->last = newelem;
	else {
		newelem->prev = dllist->last;
		dllist->last->next = newelem;
		dllist->last = newelem;
	}
	dllist->size++;
}

void dllist_prepend(dllist_t *dllist, void *record) {
	if(!dllist) return;
	list_element_t *newelem = calloc(1, sizeof(list_element_t));
	newelem->record = record;
	if(!dllist->first)
		dllist->first = dllist->last = newelem;
	else {
		newelem->next = dllist->first;
		dllist->first->prev = newelem;
		dllist->first = newelem;
	}
	dllist->size++;
}

void dllist_remove_head(dllist_t *dllist) {
	if(!dllist) return;
	list_element_t* elem = dllist->first;
	if(!elem) return;
	dllist->first = elem->next;
	if(elem->next)
		elem->next->prev = NULL;
	else
		dllist->last = NULL;
	if(dllist->free_record)
		dllist->free_record(elem->record);
	free(elem);
	dllist->size--;
}
void dllist_remove_tail(dllist_t *dllist) {
	if(!dllist) return;
	list_element_t* elem = dllist->last;
	if(!elem) return;
	dllist->last = elem->prev;
	if(elem->prev)
		elem->prev->next = NULL;
	else
		dllist->first = NULL;
	if(dllist->free_record)
		dllist->free_record(elem->record);
	free(elem);
	dllist->size--;
}

void dllist_remove_elem(dllist_t *dllist, list_element_t* elem) {
	if(!dllist || !elem) return;
	if(elem == dllist->first)
		dllist_remove_head(dllist);
	else if(elem == dllist->last)
		dllist_remove_tail(dllist);
	else {
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
		if(dllist->free_record)
			dllist->free_record(elem->record);
		free(elem);
		dllist->size--;
	}
}

list_element_t *dllist_find_elem_next(dllist_t *dllist, void *key, list_element_t *pos) {
	if(!dllist) return NULL;
	if(!dllist->compare || !dllist->get_key) return NULL;
	if(!pos)
		pos = dllist->first;
	else
		pos=pos->next;
	while(pos) {
		if(dllist->compare(key, dllist->get_key(pos->record))==0)
			return pos;
		pos = pos->next;
	}
	return NULL;
}
list_element_t *dllist_find_elem_first(dllist_t *dllist, void *key) {
	return dllist_find_elem_next(dllist, key, NULL);
}
void *dllist_find_first(dllist_t *dllist, void *key, list_element_t **pos) {
	list_element_t *elem = dllist_find_elem_first(dllist, key);
	if(pos)
		*pos = elem;
	if(!elem) return NULL;
	return elem->record;
}
void *dllist_find_next(dllist_t *dllist, void *key, list_element_t **pos) {
	list_element_t *elem = dllist_find_elem_next(dllist, key, pos ? (*pos) : NULL);
	if(pos)
		*pos = elem;
	if(!elem) return NULL;
	return elem->record;
}
void dllist_remove_first(dllist_t *dllist, void *key) {
	dllist_remove_elem(dllist, dllist_find_elem_first(dllist, key));
}
void dllist_remove_after(dllist_t *dllist, void *key, list_element_t *pos) {
	dllist_remove_elem(dllist, dllist_find_elem_next(dllist, key, pos));
}

list_element_t *dllist_find_elem_prev(dllist_t *dllist, void *key, list_element_t *pos) {
	if(!dllist) return NULL;
	if(!dllist->compare || !dllist->get_key) return NULL;
	if(!pos)
		pos = dllist->last;
	else
		pos=pos->prev;
	while(pos) {
		if(dllist->compare(key, dllist->get_key(pos->record))==0)
			return pos;
		pos = pos->prev;
	}
	return NULL;
}
list_element_t *dllist_find_elem_last(dllist_t *dllist, void *key) {
	return dllist_find_elem_prev(dllist, key, NULL);
}
void *dllist_find_last(dllist_t *dllist, void *key, list_element_t **pos) {
	list_element_t *elem = dllist_find_elem_last(dllist, key);
	if(pos)
		*pos = elem;
	if(!elem) return NULL;
	return elem->record;
}
void *dllist_find_prev(dllist_t *dllist, void *key, list_element_t **pos) {
	list_element_t *elem = dllist_find_elem_prev(dllist, key, pos ? (*pos) : NULL);
	if(pos)
		*pos = elem;
	if(!elem) return NULL;
	return elem->record;
}
void dllist_remove_last(dllist_t *dllist, void *key) {
	dllist_remove_elem(dllist, dllist_find_elem_last(dllist, key));
}
void dllist_remove_before(dllist_t *dllist, void *key, list_element_t *pos) {
	dllist_remove_elem(dllist, dllist_find_elem_prev(dllist, key, pos));
}

void dllist_remove_all(dllist_t *dllist, void *key) {
	list_element_t *elem = dllist_find_elem_next(dllist, key, NULL);
	while(elem) {
		list_element_t *next = dllist_find_elem_next(dllist, key, elem);
		dllist_remove_elem(dllist, elem);
		elem = next;
	}
}

void **get_array_from_dllist(dllist_t *dllist, long *size) {
	if(!dllist) return NULL;
	if(size)
		*size = dllist->size;
	if(!dllist->first) return NULL;
	void **array = calloc(dllist->size, sizeof(void*));
	list_element_t *elem = dllist->first;
	for(long i=0;i<dllist->size;i++, elem=elem->next) {
		array[i]=elem->record;
	}
	return array;
}

void sort_array_from_dllist(void **array, long size, dllist_t *dllist) {
	sort_array(array, size, dllist->get_key, dllist->compare);
}

/*
void dllist_walk(dllist_t *dllist) {
	if(!dllist) return;
	list_element_t *elem = dllist->first;
	while(elem)
		elem = elem->next;
}

void dllist_walk_reverse(dllist_t *dllist) {
	if(!dllist) return;
	list_element_t *elem = dllist->last;
	while(elem)
		elem = elem->prev;
}
*/
