#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>

static const size_t STD_CAPACITY      = 64;
static const size_t VECTOR_EMPTY_SIZE = 0;

struct vector
{
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *data;
};

enum class VectorError
{
    NO_ERROR,
    INVALID_ELEM_SIZE,
    ALLOCATION_ERROR,
    VECTOR_OVERFLOW,
    COPY_ERROR,
    RESIZE_ERROR,
    EMPTY_STACK
};

struct vector *vector_new(size_t elems, size_t elem_size);
int vector_push(struct vector *v, void const *elem);
int vector_pop (struct vector *v, void *elem);
int vector_empty(struct vector const *v);
struct vector *vector_delete(struct vector *v);
void vector_print(struct vector const *v, void (*pf)(void const *data));
int vector_set(struct vector *v, size_t index, void const *elem);
int vector_get(struct vector const *v, size_t index, void *elem);
int vector_resize(struct vector *v, size_t new_size);
size_t vector_size(struct vector const *v);
void print_vector_error(VectorError err);
int adapt_vector(struct vector *v);

void print_vector_error(VectorError err)
{
    switch (err)
    {
        case VectorError::NO_ERROR:
            break;
        case VectorError::INVALID_ELEM_SIZE:
            fprintf(stderr, "[VECTOR ERROR] invalid element size (elem_size must be > 0)\n");
            break;
        case VectorError::ALLOCATION_ERROR:
            fprintf(stderr, "[VECTOR ERROR] memory allocation failed\n");
            break;
        case VectorError::VECTOR_OVERFLOW:
            fprintf(stderr, "[VECTOR ERROR] vector overflow (capacity exceeded)\n");
            break;
        case VectorError::COPY_ERROR:
            fprintf(stderr, "[VECTOR ERROR] failed to copy element into vector\n");
            break;
        case VectorError::EMPTY_STACK:
            fprintf(stderr, "[VECTOR ERROR] pop from empty vector\n");
            break;
        case VectorError::RESIZE_ERROR:
            fprintf(stderr, "[VECTOR ERROR] failed to resize vector\n");
            break;
        default:
            fprintf(stderr, "[VECTOR ERROR] unknown error code (%d)\n", (int)err);
            break;
    }
}

struct vector *vector_new(size_t elems, size_t elem_size)
{
    if (elem_size == 0)
    {
    //    print_vector_error(VectorError::INVALID_ELEM_SIZE);
        return nullptr;
    }

    vector *v = (vector *) calloc(1, sizeof(vector));
    if (!v)
    {
      //  print_vector_error(VectorError::ALLOCATION_ERROR);
        return nullptr;
    } 
    
    if (elems == 0) elems = STD_CAPACITY;

    v->data = calloc(elems, elem_size);
    if (!v->data)
    {
        //print_vector_error(VectorError::ALLOCATION_ERROR);
        free(v);
        return nullptr;
    }

    v->capacity  = elems;
    v->size      = VECTOR_EMPTY_SIZE;
    v->elem_size = elem_size;

    return v;
}

int adapt_vector(struct vector *v)
{
    if (!v) return EXIT_FAILURE;

    if (v->size >= v->capacity)
    {
        if (v->capacity > SIZE_MAX / 2) {
          //  print_vector_error(VectorError::VECTOR_OVERFLOW);
            return EXIT_FAILURE;
        }
        
        size_t new_capacity = v->capacity * 2;
        void *new_data = realloc(v->data, new_capacity * v->elem_size);
        if (!new_data)
        {
            //print_vector_error(VectorError::RESIZE_ERROR);
            return EXIT_FAILURE;
        }
        v->data = new_data;
        v->capacity = new_capacity;
    }

    return EXIT_SUCCESS;
}

int vector_push(struct vector *v, const void *elem)
{   
    if (!v || !v->data || !elem) return EXIT_FAILURE;

    if (adapt_vector(v) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    void *last_ptr = (char *) v->data + v->size * v->elem_size;
    memcpy(last_ptr, elem, v->elem_size);
    v->size++;

    return EXIT_SUCCESS;
}

int vector_pop(struct vector *v, void *elem)
{
    if (!v || !v->data || !elem) return EXIT_FAILURE;

    if (vector_empty(v))
    {
     //   print_vector_error(VectorError::EMPTY_STACK);
        return EXIT_FAILURE;
    }

    v->size--;  
    void *last_ptr = (char *) v->data + v->size * v->elem_size; 
    memcpy(elem, last_ptr, v->elem_size);

    return EXIT_SUCCESS;
}

int vector_empty(struct vector const *v)
{
    if (!v) return 1;
    return v->size == 0;
}

struct vector *vector_delete(struct vector *v)
{
    if (!v) return nullptr; 

    free(v->data);
    free(v);
    return nullptr;
}

void vector_print(struct vector const *v, void (*pf)(void const*data))
{
    if (!v) return;
    
    if (!v->data || v->size == 0) {
        puts("[]");
        return;
    }

    putchar('[');
    
    char *elem_to_print = (char *) v->data;
    for (size_t i = 0; i < v->size - 1; i++)
    {
        pf(elem_to_print);
        printf(", ");
        elem_to_print += v->elem_size;
    }
    pf(elem_to_print);
    puts("]");
}

int vector_set(struct vector *v, size_t index, void const *elem)
{
    if (!v || !elem) return EXIT_FAILURE;

    if (index >= v->capacity)
    {
        if (index == SIZE_MAX) 
        {
       //     print_vector_error(VectorError::VECTOR_OVERFLOW);
            return EXIT_FAILURE;
        }
        
        size_t new_size = index + 1;
        if (vector_resize(v, new_size) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    
    void *index_elem_ptr = (char *) v->data + index * v->elem_size;
    memcpy(index_elem_ptr, elem, v->elem_size);

    if (index >= v->size)
        v->size = index + 1;

    return EXIT_SUCCESS;
}

int vector_get(struct vector const *v, size_t index, void *elem)
{   
    if (!v || !elem) return EXIT_FAILURE;

    if (index >= v->size)
        return EXIT_FAILURE;

    void *index_elem_ptr = (char *) v->data + index * v->elem_size;
    memcpy(elem, index_elem_ptr, v->elem_size);

    return EXIT_SUCCESS;
}

int vector_resize(struct vector *v, size_t new_size)
{
    if (!v) return EXIT_FAILURE;

    if (new_size == 0) return EXIT_FAILURE;

    if (new_size < v->size)
        v->size = new_size;

    void *new_data = realloc(v->data, new_size * v->elem_size);
    if (!new_data)
    {
       // print_vector_error(VectorError::RESIZE_ERROR);
        return EXIT_FAILURE;
    }
    v->data = new_data;
    v->capacity = new_size;

    return EXIT_SUCCESS;
}

size_t vector_size(struct vector const *v)
{
    if (!v) return 0;
    return v->size;
}
