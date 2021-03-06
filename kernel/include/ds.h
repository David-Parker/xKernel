#pragma once

#include <stddef.h>

// Data Structures

/* Converts pointer to list element LIST_ELEM into a pointer to
   the structure that LIST_ELEM is embedded inside.  Supply the
   name of the outer structure STRUCT and the member name MEMBER
   of the list element. */
#define list_entry(LIST_ELEM, STRUCT, MEMBER)           \
        ((STRUCT *) ((_u8 *) &(LIST_ELEM)->next     \
                     - offsetof (STRUCT, MEMBER.next)))


#define VECTOR_GET_GENERIC(this, idx, type) *((type*)vector_elem_at(&this, idx))

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

void linked_list_add_before(linked_list_t* list, linked_list_node_t* node, linked_list_node_t* toAdd);
void linked_list_add_after(linked_list_t* list, linked_list_node_t* node, linked_list_node_t* toAdd);
static void linked_list_add_empty(linked_list_t* list, linked_list_node_t* toAdd);

typedef struct ring_buffer
{
    size_t* buffer;
    int buf_len;
    int ring_len;
    int size;
    int idx_start;
    int idx_end;
    int idx_read;
    _u64 total_push;
} ring_buffer_t, *ring_buffer_ptr_t;

void ring_buffer_init(ring_buffer_t* ring, size_t* buffer, int buf_len, int ring_len);
void ring_buffer_copy(ring_buffer_t* from, ring_buffer_t* to);
size_t ring_buffer_push(ring_buffer_t* ring, size_t elem);
size_t ring_buffer_pop(ring_buffer_t* ring);
size_t ring_buffer_get(ring_buffer_t* ring, int idx);
size_t ring_buffer_get_last(ring_buffer_t* ring);
void ring_buffer_clear(ring_buffer_t* ring);
int ring_buffer_next(ring_buffer_t* ring, int read_idx);
int ring_buffer_prev(ring_buffer_t* ring, int read_idx);
int ring_buffer_idx(ring_buffer_t* ring, int read_idx);
int ring_buffer_distance(int start_idx, int end_idx, int buf_len);

/*
    Generic, dynamically expanding array.
*/
typedef struct vector
{
    _u8* buffer;
    _u64 count;
    _u64 size;
    _u64 sizeof_T;
} vector_t;

void vector_init(vector_t* this, _u64 init_size, _u64 sizeof_T);
void vector_deallocate(vector_t* this);
void vector_clear(vector_t* this);
void vector_push_back(vector_t* this, void* data);
void* vector_elem_at(vector_t* this, _u64 index);