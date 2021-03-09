#include <ds.h>
#include <debug.h>

void linked_list_init(linked_list_t* list)
{
    list->count = 0;
    list->head = NULL;
}

void linked_list_add_back(linked_list_t* list, linked_list_node_t* toAdd)
{
    kassert(list != NULL);
    kassert(toAdd != NULL);

    if (list->head == NULL)
    {
        linked_list_add_empty(list, toAdd);
    }
    else
    {
        linked_list_add_before(list, list->head, toAdd);
    }
}

void linked_list_add_front(linked_list_t* list, linked_list_node_t* toAdd)
{
    kassert(list != NULL);
    kassert(toAdd != NULL);

    if (list->head == NULL)
    {
        linked_list_add_empty(list, toAdd);
    }
    else
    {
        linked_list_add_before(list, list->head, toAdd);
        list->head = toAdd;
    }
}

void linked_list_remove(linked_list_t* list, linked_list_node_t* toRemove)
{
    kassert(list != NULL);
    kassert(toRemove != NULL);

    if (toRemove->next == toRemove)
    {
        kassert(list->count == 1 && list->head == toRemove);
        list->head = NULL;
    }
    else
    {
        toRemove->next->prev = toRemove->prev;
        toRemove->prev->next = toRemove->next;
        if (list->head == toRemove)
        {
            list->head = toRemove->next;
        }
    }

    list->count--;
}

void linked_list_add_before(linked_list_t* list, linked_list_node_t* node, linked_list_node_t* toAdd)
{
    toAdd->next = node;
    toAdd->prev = node->prev;
    node->prev->next = toAdd;
    node->prev = toAdd;

    list->count++;
}

void linked_list_add_after(linked_list_t* list, linked_list_node_t* node, linked_list_node_t* toAdd)
{
    linked_list_add_before(list, node->next, toAdd);
}

void linked_list_add_empty(linked_list_t* list, linked_list_node_t* toAdd)
{
    list->head = toAdd;
    list->head->next = toAdd;
    list->head->prev = toAdd;

    list->count++;
}

void ring_buffer_init(ring_buffer_t* ring, size_t* buffer, int buf_len, int ring_len)
{
    kassert(ring != NULL);
    kassert(buffer != NULL);
    kassert(buf_len > 0);
    kassert(buf_len >= ring_len);

    ring->buffer = buffer;
    ring->buf_len = buf_len;
    ring->ring_len = ring_len;
    ring->size = 0;
    ring->idx_start = 0;
    ring->idx_end = 0;
    ring->idx_read = 0;
    ring->total_push = 0;
}

void ring_buffer_copy(ring_buffer_t* from, ring_buffer_t* to)
{
    memcpy(to, from, sizeof(ring_buffer_t));
}

size_t ring_buffer_push(ring_buffer_t* ring, size_t elem)
{
    kassert(ring != NULL);

    size_t old_elem = ring->buffer[ring->idx_end];

    ring->buffer[ring->idx_end] = elem;
    ring->total_push++;
    ring->idx_end = modulo(ring->idx_end + 1, ring->buf_len);
    ring->size++;
    ring->size = min(ring->size, ring->buf_len);

    if (ring->size > ring->ring_len)
        ring->idx_start = modulo(ring->idx_start + 1, ring->buf_len);
    
    return old_elem;
}

size_t ring_buffer_pop(ring_buffer_t* ring)
{
    kassert(ring != NULL);
    kassert(ring->idx_start != ring->idx_end);
    kassert(ring->size > 0);

    size_t old_elem = ring->buffer[ring->idx_end];

    ring->size--;

    if (ring->size > ring->ring_len)
        ring->idx_start = modulo(ring->idx_start - 1, ring->buf_len);

    int idx = modulo(ring->idx_end-1, ring->buf_len);

    if (ring->idx_read == ring->idx_end)
    {
        ring->idx_read = idx;
    }

    ring->idx_end = idx;

    return old_elem;
}

size_t ring_buffer_get(ring_buffer_t* ring, int idx)
{
    kassert(ring != NULL);

    return ring->buffer[modulo(idx, ring->buf_len)];
}

size_t ring_buffer_get_last(ring_buffer_t* ring)
{
    kassert(ring != NULL);

    return ring_buffer_get(ring, ring->idx_end - 1);
}

void ring_buffer_clear(ring_buffer_t* ring)
{
    ring->idx_start = 0;
    ring->idx_end = 0;
    ring->idx_read = 0;
    ring->total_push = 0;
    ring->size = 0;
}

int ring_buffer_next(ring_buffer_t* ring, int read_idx)
{
    kassert(read_idx <= ring->buf_len);
    kassert(read_idx >= 0);

    read_idx = modulo(read_idx + 1, ring->buf_len);

    if (read_idx == ring->idx_end)
    {
        return -1;
    }
    else
    {
        return read_idx;
    }
}

int ring_buffer_prev(ring_buffer_t* ring, int read_idx)
{
    kassert(read_idx <= ring->buf_len);
    kassert(read_idx >= 0);

    read_idx = modulo(read_idx - 1, ring->buf_len);

    if (read_idx == ring->idx_end)
    {
        return -1;
    }
    else
    {
        return read_idx;
    }
}

int ring_buffer_idx(ring_buffer_t* ring, int read_idx)
{
    return modulo(read_idx, ring->buf_len);
}

int ring_buffer_distance(int start_idx, int end_idx, int buf_len)
{
    return modulo(end_idx - start_idx, buf_len);
}

void vector_init(vector_t* this, _u64 init_size, _u64 sizeof_T)
{
    if (init_size == 0)
    {
        init_size = 1;
    }

    this->size = init_size;
    this->count = 0;
    this->buffer = kmalloc(init_size * sizeof_T);
    this->sizeof_T = sizeof_T;
}

void vector_deallocate(vector_t* this)
{
    free(this->buffer);
}

void vector_clear(vector_t* this)
{
    this->count = 0;
    memset(this->buffer, 0, this->size * this->sizeof_T);
}

void vector_push_back(vector_t* this, void* data)
{
    _u64 byte_offset;
    _u64 new_size;
    _u8* new_buffer;

    // Resize array
    if (this->count == this->size)
    {
        new_size = this->size * 2;
        new_buffer = kmalloc(new_size * this->sizeof_T);
        memcpy(new_buffer, this->buffer, this->size * this->sizeof_T);
        free(this->buffer);
        this->size = new_size;
        this->buffer = new_buffer;
    }

    byte_offset = this->count * this->sizeof_T;

    memcpy(&this->buffer[byte_offset], data, this->sizeof_T);

    this->count++;
}

void* vector_elem_at(vector_t* this, _u64 index)
{
    if (index >= this->count)
    {
        return NULL;
    }

    return &this->buffer[index * this->sizeof_T];
}