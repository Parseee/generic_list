#ifndef LIST_NODE_H_
#define LIST_NODE_H_

#include <stdint.h>
#include <stdlib.h>

#define LIST_NODE_ERROR_HANDLE(call_func, ...)                             \
    do {                                                                   \
        LIST_NODE_ERROR error_handler = call_func;                         \
        if (error_handler) {                                               \
            fprintf(stderr, "Error calling " #call_func ". error is %s\n", \
                List_node_strerror(error_handler));                        \
            __VA_ARGS__;                                                   \
            return (int)error_handler;                                     \
        }                                                                  \
    } while (0)

typedef enum {
    LIST_NODE_OK,
    LIST_NODE_ALLOC_FAILURE,
    LIST_NODE_FAILURE
} LIST_NODE_ERROR;

typedef struct List_node_t {
    void* data;
    struct List_node_t* next;
} List_node_t;

LIST_NODE_ERROR List_node_ctor(List_node_t* node, size_t elem_size, const void* data, void (*deep_copy)(void*, const void*));
LIST_NODE_ERROR List_node_dtor(List_node_t* node, void (*delete_element)(void*));

const char* List_node_strerror(const LIST_NODE_ERROR error);

#endif /* LIST_NODE_H_ */