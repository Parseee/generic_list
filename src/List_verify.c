#include <assert.h>
#include <stdio.h>

#include "List.h"

#ifndef NDEBUG
LIST_ERROR List_traverse(List_t* list)
{
    assert(list);

    // TODO: think if this needed
    // if (list->list_size == 0) {
    //     return LIST_EMPTY_FAILURE;
    // }

    List_node_t* ptr = list->head;

    while (ptr) {
        if (list->print_element) {
            list->print_element(ptr->data);
        } else {
            return LIST_NO_PRINT_ERROR;
        }
        ptr = ptr->next;
    }

    fprintf(stderr, "{NULL}\n");

    return LIST_OK;
}
#endif /* NDEBUG */