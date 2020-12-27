#pragma once

#include <kernel/util/stddef.h>

// Data Structures

/* Converts pointer to list element LIST_ELEM into a pointer to
   the structure that LIST_ELEM is embedded inside.  Supply the
   name of the outer structure STRUCT and the member name MEMBER
   of the list element.  See the big comment at the top of the
   file for an example. */
#define list_entry(LIST_ELEM, STRUCT, MEMBER)           \
        ((STRUCT *) ((_u8 *) &(LIST_ELEM)->next     \
                     - offsetof (STRUCT, MEMBER.next)))

typedef struct linked_list_node {
    struct linked_list_node* next;
    struct linked_list_node* prev;
} linked_list_node_t, *linked_list_node_ptr_t;

typedef struct linked_list
{
    int count;
    linked_list_node_ptr_t head; // tail is always head->prev
} linked_list_t, *linked_list_ptr_t;

void linked_list_init(linked_list_ptr_t list);
void linked_list_add_back(linked_list_ptr_t list, linked_list_node_ptr_t toAdd);
void linked_list_add_front(linked_list_ptr_t list, linked_list_node_ptr_t toAdd);
void linked_list_remove(linked_list_ptr_t list, linked_list_node_ptr_t toRemove);
static void linked_list_add_before(linked_list_node_ptr_t node, linked_list_node_ptr_t toAdd);
static void linked_list_add_empty(linked_list_ptr_t list, linked_list_node_ptr_t toAdd);