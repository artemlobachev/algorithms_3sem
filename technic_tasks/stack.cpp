#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static const size_t STACK_CAPACITY   = 16;
static const size_t STACK_EMPTY_SIZE = 0;

struct stack *stack_new(size_t elem_size);
int stack_push(struct stack *st, const void *elem);
int resize_stack(struct stack *st);
int stack_pop (struct stack *st, void *elem);
int stack_top (struct stack *st, void *elem);
int stack_empty(struct stack const *st);
struct stack *stack_delete(struct stack *st);
void stack_print(struct stack const *st, void (*pf)(void const *st));


struct stack
{
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *data;
};

enum class StackError
{
    NO_ERROR,
    INVALID_ELEM_SIZE,
    ALLOCATION_ERROR,
    STACK_OVERFLOW,
    COPY_ERROR,
    RESIZE_ERROR,
    EMPTY_STACK
};

void print_stack_error(StackError err)
{
    switch (err)
    {
        case StackError::NO_ERROR:
            break;

        case StackError::INVALID_ELEM_SIZE:
            fprintf(stderr, 
             "[STACK ERROR] invalid element size (elem_size must be > 0)\n");
            break;

        case StackError::ALLOCATION_ERROR:
            fprintf(stderr, 
             "[STACK ERROR] memory allocation failed\n");
            break;

        case StackError::STACK_OVERFLOW:
            fprintf(stderr, 
             "[STACK ERROR] stack overflow (capacity exceeded)\n");
            break;

        case StackError::COPY_ERROR:
            fprintf(stderr, 
             "[STACK ERROR] failed to copy element into stack\n");
            break;

        case StackError::EMPTY_STACK:
            fprintf(stderr, 
             "[STACK ERROR] pop from empty stack\n");
            break;

        case StackError::RESIZE_ERROR:
            fprintf(stderr, 
             "[STACK ERROR] failed to resize stack\n");
            break;

        default:
            fprintf(stderr, 
             "[STACK ERROR] unknown error code (%d)\n", (int)err);
            break;
    }
}

struct stack *stack_new(size_t elem_size)
{
    if (elem_size == 0)
    {
        print_stack_error(StackError::INVALID_ELEM_SIZE);
        return nullptr;
    }

    stack *st = (stack *) calloc(1, sizeof(stack));
    if (!st)
    {
        print_stack_error(StackError::ALLOCATION_ERROR);
        return nullptr;
    }    

    st->data = calloc(STACK_CAPACITY, elem_size);
    if (!st->data)
    {
        print_stack_error(StackError::ALLOCATION_ERROR);
        free(st);
        return nullptr;
    }

    st->capacity  = STACK_CAPACITY;
    st->size      = STACK_EMPTY_SIZE;
    st->elem_size = elem_size;

    return st;
}

int resize_stack(struct stack *st)
{
    assert(st);

    if (st->size >= st->capacity)
    {
        size_t new_capacity = st->capacity * 2;
        void *new_data = realloc(st->data, new_capacity * st->elem_size);
        if (!new_data)
        {
            print_stack_error(StackError::RESIZE_ERROR);
            return EXIT_FAILURE;
        }
        st->data = new_data;
        st->capacity = new_capacity;
        return EXIT_SUCCESS;
    }
    
    if (st->size > 0 && st->capacity > STACK_CAPACITY && st->capacity >= 2 * st->size)
    {
        size_t new_capacity = st->capacity / 2;
        void *new_data = realloc(st->data, new_capacity * st->elem_size);
        if (!new_data)
        {
            print_stack_error(StackError::RESIZE_ERROR);
            return EXIT_SUCCESS;
        }
        st->data = new_data;
        st->capacity = new_capacity;
    }

    return EXIT_SUCCESS;
}

int stack_push(struct stack *st, const void *elem)
{   
    assert(st && st->data && elem);

    if (resize_stack(st) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    void *top_ptr = (char *) st->data + st->size * st->elem_size;
    memcpy(top_ptr, elem, st->elem_size);
    ++st->size;

    return EXIT_SUCCESS;
}

int stack_pop(struct stack *st, void *elem)
{
    assert(st && st->data && elem);

    if (stack_empty(st))
    {
        print_stack_error(StackError::EMPTY_STACK);
        return EXIT_FAILURE;
    }

    st->size--;  
    void *top_ptr = (char *) st->data + st->size * st->elem_size; 
    memcpy(elem, top_ptr, st->elem_size);

    resize_stack(st);

    return EXIT_SUCCESS;
}

int stack_top(struct stack *st, void *elem)
{
    assert(st && st->data && elem);

    if (stack_empty(st))
    {
        print_stack_error(StackError::EMPTY_STACK);
        return EXIT_FAILURE;
    }

    void *top_ptr = (char *) st->data + (st->size - 1) * st->elem_size;
    memcpy(elem, top_ptr, st->elem_size);

    return EXIT_SUCCESS;
}

int stack_empty(struct stack const *st)
{
    assert(st);
    return st->size == 0;
}

struct stack *stack_delete(struct stack *st)
{
    if (!st) return nullptr; 

    free(st->data);
    free(st);
    return nullptr;
}

void stack_print(struct stack const *st, void (*pf)(void const*st))
{
    assert(st);
    
    if (!st->data || st->size == 0) {
        puts("[]");
        return;
    }

    putchar('[');
    
    char *elem_to_print = (char *) st->data;
    for (size_t i = 0; i < st->size - 1; i++)
    {
        pf(elem_to_print);
        printf(", ");
        elem_to_print += st->elem_size;
    }
    pf(elem_to_print);
    puts("]");
}

/*
static void print_double(void const *st) {
   printf("%lld", *(long long *)st);
}

int main() {
    struct stack *st = stack_new(sizeof(long long));
    if (!st) {
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < 10; i++) {
        long long tmp = (long long)i * i;  // ИСПРАВЛЕНО: правильный тип
        if (stack_push(st, &tmp) != EXIT_SUCCESS) {
            printf("Failed to push %d\n", i);
            break;
        }
    }
    
    stack_print(st, print_double);
    st = stack_delete(st);
    
    return EXIT_SUCCESS;
}*/