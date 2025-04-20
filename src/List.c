#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "List_node/List_node.h"
// #include "List_node/List_node.h"

LIST_ERROR List_ctor(List_t* list, size_t elem_size, void (*copy_element)(void*, const void*), void (*delete_element)(void*), void (*print_element)(const void*))
{
    assert(list);

    list->head = NULL;
    list->list_size = 0;
    list->elem_size = elem_size;
    list->copy_element = copy_element;
    list->delete_element = delete_element;
    list->print_element = print_element;

    return LIST_OK;
}

LIST_ERROR List_dtor(List_t* list)
{
    assert(list);

    while (list->list_size) {
        LIST_ERROR_HANDLE(List_remove(list, 0));
    }

    return LIST_OK;
}

LIST_ERROR List_insert(List_t* list, size_t pos, const void* data)
{
    assert(list);

    if (pos > list->list_size) {
        pos = list->list_size;
    }

    List_node_t* new_node = calloc(1, sizeof(*new_node));
    LIST_NODE_ERROR_HANDLE(List_node_ctor(new_node, list->elem_size, data, list->copy_element), free(new_node); return LIST_ALLOC_FAILURE);

    if (pos == 0) { // insert in head
        new_node->next = list->head;
        list->head = new_node;
        ++list->list_size;
        return LIST_OK;
    }

    List_node_t* node = list->head;
    while (pos > 1) {
        // add NULL checker!
        node = node->next;
        --pos;
    }

    new_node->next = node->next;
    node->next = new_node;
    ++list->list_size;

    return LIST_OK;
}

LIST_ERROR List_remove(List_t* list, size_t idx)
{
    assert(list);

    if (idx >= list->list_size) {
        return LIST_EMPTY_FAILURE;
    }

    List_node_t* prev_node = list->head;
    if (idx == 0) {
        list->head = prev_node->next;
        List_node_dtor(prev_node, list->delete_element);
        free(prev_node);
        --list->list_size;
        return LIST_OK;
    }

    while (idx > 1) {
        prev_node = prev_node->next;
        --idx;
    }

    List_node_t* victim = prev_node->next;
    prev_node->next = prev_node->next->next;
    List_node_dtor(victim, list->delete_element);
    free(victim);
    --list->list_size;

    return LIST_OK;
}

LIST_ERROR List_get(List_t* list, size_t idx, void* data)
{
    assert(list);
    assert(data);

    if (idx >= list->list_size) {
        return LIST_BAD_INDEX_FAILURE;
    }

    List_node_t* node = list->head;

    while (idx > 0) {
        node = node->next;
        --idx;
    }
    // node = node->next;

    if (list->copy_element) {
        list->copy_element(data, node->data);
    } else {
        memcpy(data, node->data, list->elem_size);
    }

    return LIST_OK;
}

// function assumes that two lists are the same in terms of data the manage
LIST_ERROR List_copy(List_t* dest, const List_t* source)
{
    assert(dest);
    assert(source);

    List_node_t* node = source->head;

    while (node) {
        LIST_ERROR_HANDLE(List_insert(dest, dest->list_size, node->data));
        node = node->next;
    }

    return LIST_OK;
}

LIST_ERROR List_insert_back(List_t* list, const void* data)
{
    assert(list);
    assert(data);

    return List_insert(list, list->list_size, data);
}

#define CASE_ENUM_TO_STRING_(error) \
    case error:                     \
        return #error
const char* List_strerror(const LIST_ERROR error)
{
    switch (error) {
        CASE_ENUM_TO_STRING_(LIST_OK);
        CASE_ENUM_TO_STRING_(LIST_FAILURE);
        CASE_ENUM_TO_STRING_(LIST_INSERT_FAILURE);
        CASE_ENUM_TO_STRING_(LIST_EMPTY_FAILURE);
        CASE_ENUM_TO_STRING_(LIST_BAD_INDEX_FAILURE);
        CASE_ENUM_TO_STRING_(LIST_NO_PRINT_ERROR);
        CASE_ENUM_TO_STRING_(LIST_ALLOC_FAILURE);
    default:
        return "UNKNOWN_TABLE_ERROR";
    }
    return "UNKNOWN_TABLE_ERROR";
}
#undef CASE_ENUM_TO_STRING_