// list.h
#ifndef LIST_H
#define LIST_H

#include <stdbool.h> // For bool type

// Define elem as an integer
typedef int elem;

// Node structure
typedef struct node {
    elem value;
    struct node *next;
} node_t;

// List structure
typedef struct {
    node_t *head;
} list_t;

// Function prototypes
list_t* list_alloc();
void list_free(list_t *list);
int list_length(list_t *list);
void list_add_to_front(list_t *list, elem value);
void list_add_to_back(list_t *list, elem value);
void list_add_at_index(list_t *list, elem value, int index);
elem list_remove_from_front(list_t *list);
elem list_remove_from_back(list_t *list);
elem list_remove_at_index(list_t *list, int index);
bool list_is_in(list_t *list, elem value);
elem list_get_elem_at(list_t *list, int index);
int list_get_index_of(list_t *list, elem value);
int list_to_string(list_t *list, char *buffer, size_t buffer_size);

#endif // LIST_H



