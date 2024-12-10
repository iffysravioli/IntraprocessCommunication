#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Allocate and initialize a new list
list_t* list_alloc() {
    list_t *list = malloc(sizeof(list_t));
    if (list != NULL) {
        list->head = NULL;
    }
    return list;
}

// Free all nodes in the list and the list itself
void list_free(list_t *list) {
    if (list == NULL) return;
    node_t *current = list->head;
    node_t *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

// Get the length of the list
int list_length(list_t *list) {
    if (list == NULL) return 0;
    int count = 0;
    node_t *current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Add an element to the front of the list
void list_add_to_front(list_t *list, elem value) {
    if (list == NULL) return;
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) return; // Allocation failed
    new_node->value = value;
    new_node->next = list->head;
    list->head = new_node;
}

// Add an element to the back of the list
void list_add_to_back(list_t *list, elem value) {
    if (list == NULL) return;
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) return; // Allocation failed
    new_node->value = value;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Add an element at a specific index
void list_add_at_index(list_t *list, elem value, int index) {
    if (list == NULL || index < 0) return;
    if (index == 0) {
        list_add_to_front(list, value);
        return;
    }
    node_t *current = list->head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }
    if (current == NULL) return; // Index out of bounds
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) return; // Allocation failed
    new_node->value = value;
    new_node->next = current->next;
    current->next = new_node;
}

// Remove and return the front element
elem list_remove_from_front(list_t *list) {
    if (list == NULL || list->head == NULL) return -1; // Indicates failure
    node_t *temp = list->head;
    elem value = temp->value;
    list->head = temp->next;
    free(temp);
    return value;
}

// Remove and return the back element
elem list_remove_from_back(list_t *list) {
    if (list == NULL || list->head == NULL) return -1; // Indicates failure
    if (list->head->next == NULL) {
        elem value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }
    node_t *current = list->head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    elem value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

// Remove and return an element at a specific index
elem list_remove_at_index(list_t *list, int index) {
    if (list == NULL || list->head == NULL || index < 0) return -1; // Indicates failure
    if (index == 0) {
        return list_remove_from_front(list);
    }
    node_t *current = list->head;
    for (int i = 0; i < index - 1 && current->next != NULL; i++) {
        current = current->next;
    }
    if (current->next == NULL) return -1; // Index out of bounds
    node_t *temp = current->next;
    elem value = temp->value;
    current->next = temp->next;
    free(temp);
    return value;
}

// Check if a value is in the list
bool list_is_in(list_t *list, elem value) {
    if (list == NULL) return false;
    node_t *current = list->head;
    while (current != NULL) {
        if (current->value == value) return true;
        current = current->next;
    }
    return false;
}

// Get the element at a specific index
elem list_get_elem_at(list_t *list, int index) {
    if (list == NULL || index < 0) return -1; // Indicates failure
    node_t *current = list->head;
    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }
    if (current == NULL) return -1; // Index out of bounds
    return current->value;
}

// Get the index of a specific value
int list_get_index_of(list_t *list, elem value) {
    if (list == NULL) return -1; // Indicates failure
    node_t *current = list->head;
    int index = 0;
    while (current != NULL) {
        if (current->value == value) return index;
        current = current->next;
        index++;
    }
    return -1; // Not found
}

// Convert the list to a comma-separated string
int list_to_string(list_t *list, char *buffer, size_t buffer_size) {
    if (list == NULL || buffer == NULL) return -1;
    node_t *current = list->head;
    size_t len = 0;

    buffer[0] = '\0'; // Initialize buffer

    while (current != NULL) {
        int written = snprintf(buffer + len, buffer_size - len, "%d", current->value);
        if (written < 0 || (size_t)written >= buffer_size - len) {
            // Not enough space
            return -1;
        }
        len += written;

        if (current->next != NULL) {
            if (len < buffer_size - 2) { // For comma and space
                strcat(buffer, ", ");
                len += 2;
            } else {
                return -1;
            }
        }
        current = current->next;
    }
    return 0;
}





