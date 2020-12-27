#include <kernel/lib/ds.h>
#include <kernel/util/debug.h>

void linked_list_init(linked_list_ptr_t list)
{
    list->count = 0;
    list->head = NULL;
}

void linked_list_add_back(linked_list_ptr_t list, linked_list_node_ptr_t toAdd)
{
    kassert(list != NULL && toAdd != NULL);

    if (list->head == NULL)
    {
        linked_list_add_empty(list, toAdd);
    }
    else
    {
        linked_list_add_before(list->head, toAdd);
    }

    list->count++;
}

void linked_list_add_front(linked_list_ptr_t list, linked_list_node_ptr_t toAdd)
{
    kassert(list != NULL && toAdd != NULL);

    if (list->head == NULL)
    {
        linked_list_add_empty(list, toAdd);
    }
    else
    {
        linked_list_add_before(list->head, toAdd);
        list->head = toAdd;
    }

    list->count++;
}

void linked_list_remove(linked_list_ptr_t list, linked_list_node_ptr_t toRemove)
{
    kassert(list != NULL && toRemove != NULL);

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

static void linked_list_add_before(linked_list_node_ptr_t node, linked_list_node_ptr_t toAdd)
{
    toAdd->next = node;
    toAdd->prev = node->prev;
    node->prev->next = toAdd;
    node->prev = toAdd;
}

static void linked_list_add_empty(linked_list_ptr_t list, linked_list_node_ptr_t toAdd)
{
    list->head = toAdd;
    list->head->next = toAdd;
    list->head->prev = toAdd;
}