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
} linked_list_node_t;

typedef struct linked_list
{
    int count;
    linked_list_node_t* head; // tail is always head->prev
} linked_list_t;

void linked_list_init(linked_list_t* list);
void linked_list_add_back(linked_list_t* list, linked_list_node_t* toAdd);
void linked_list_add_front(linked_list_t* list, linked_list_node_t* toAdd);
void linked_list_remove(linked_list_t* list, linked_list_node_t* toRemove);
static void linked_list_add_before(linked_list_node_t* node, linked_list_node_t* toAdd);
static void linked_list_add_empty(linked_list_t* list, linked_list_node_t* toAdd);

typedef struct ring_buffer
{
    size_t* buffer;
    int buf_len;
    int ring_len;
    int idx_start;
    int idx_end;
    int cnt;
} ring_buffer_t, *ring_buffer_ptr_t;

void ring_buffer_init(ring_buffer_t* ring, size_t* buffer, int buf_len, int ring_len);
void ring_buffer_push(ring_buffer_t* ring, size_t elem);
size_t ring_buffer_get(ring_buffer_t* ring, int idx);
int ring_buffer_next(ring_buffer_t* ring, int read_idx);