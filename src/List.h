#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

#include "List_node/List_node.h"

#define LIST_ERROR_HANDLE(call_func, ...)                              \
    do {                                                               \
        LIST_ERROR error_handler = call_func;                          \
        if (error_handler) {                                           \
            fprintf(stderr, "Error calling " #call_func " on line %d," \
                            " file %s. error is %s\n",                 \
                __LINE__, __FILE__, List_strerror(error_handler));     \
            __VA_ARGS__;                                               \
            return (int)error_handler;                                 \
        }                                                              \
    } while (0)

typedef enum {
    LIST_OK,
    LIST_FAILURE,
    LIST_INSERT_FAILURE,
    LIST_EMPTY_FAILURE,
    LIST_BAD_INDEX_FAILURE,
    LIST_NO_PRINT_ERROR,
    LIST_ALLOC_FAILURE
} LIST_ERROR;

typedef struct List_t {
    List_node_t* head;
    size_t list_size;
    size_t elem_size;

    void (*copy_element)(void*, const void*);
    void (*delete_element)(void*);
    void (*print_element)(const void*);
} List_t;

LIST_ERROR List_ctor(List_t* list, size_t elem_size, void (*copy_element)(void*, const void*), void (*delete_element)(void*), void (*print_element)(const void*));
LIST_ERROR List_dtor(List_t* list);

LIST_ERROR List_insert(List_t* list, size_t pos, const void* data);
LIST_ERROR List_insert_back(List_t* list, const void* data);
LIST_ERROR List_remove(List_t* list, size_t idx);
LIST_ERROR List_get(List_t* list, size_t idx, void* data);
LIST_ERROR List_copy(List_t* dest, const List_t* source);

#ifndef NDEBUG
LIST_ERROR List_traverse(List_t* list);
#endif /* NDEBUG */

const char* List_strerror(const LIST_ERROR error);

#endif /* LIST_H_ */