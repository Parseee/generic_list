#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strdup

#include "List_node.h"

#define TRY(act, label) \
    if (act)            \
    goto label

LIST_NODE_ERROR List_node_ctor(List_node_t* node, size_t elem_size, const void* data, void (*deep_copy)(void*, const void*))
{
    assert(node);

    void* new_data = calloc(1, elem_size);
    if (deep_copy) {
        deep_copy(new_data, data);
    } else {
        memcpy(new_data, data, elem_size);
    }

    node->data = new_data;
    node->next = NULL;

    return LIST_NODE_OK;
}

LIST_NODE_ERROR List_node_dtor(List_node_t* node, void (*delete_element)(void*))
{
    assert(node);

    if (delete_element) {
        delete_element(node->data);
    }

    free(node->data);
    node->data = NULL;

    return LIST_NODE_OK;
}

#define CASE_ENUM_TO_STRING_(error) \
    case error:                     \
        return #error
const char* List_node_strerror(const LIST_NODE_ERROR error)
{
    switch (error) {
        CASE_ENUM_TO_STRING_(LIST_NODE_OK);
        CASE_ENUM_TO_STRING_(LIST_NODE_FAILURE);
        CASE_ENUM_TO_STRING_(LIST_NODE_ALLOC_FAILURE);
    default:
        return "UNKNOWN_LIST_NODE_ERROR";
    }
    return "UNKNOWN_LIST_NODE_ERROR";
}
#undef CASE_ENUM_TO_STRING_
