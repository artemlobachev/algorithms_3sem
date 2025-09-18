#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

static const size_t STD_CAPACITY = 64;

struct queue 
{
    size_t size;
    size_t capacity;
    size_t elem_size;
    size_t head;   // индекс головы
    size_t tail;   // индекс хвоста (следующая свободная позиция)
    void *data;
};

int adapt_queue(struct queue *q);
struct queue *queue_new(size_t elem_size);
int queue_push(struct queue *q, const void *elem);
int queue_pop(struct queue *q, void *elem);
int queue_empty(struct queue const *q);
struct queue *queue_delete(struct queue *q);
void queue_print(struct queue const *q, void (*pf)(void const *data));

struct queue *queue_new(size_t elem_size) 
{
    if (elem_size == 0) return nullptr;

    queue *q = (queue *) calloc(1, sizeof(queue));
    if (!q) return nullptr;

    q->data = malloc(STD_CAPACITY * elem_size);
    if (!q->data) 
    {
        free(q);
        return nullptr;
    }

    q->elem_size = elem_size;
    q->size = 0;
    q->capacity = STD_CAPACITY;
    q->head = 0;
    q->tail = 0;

    return q;
}

int queue_push(struct queue *q, const void *elem) 
{
    assert(q && q->data && elem);

    if (adapt_queue(q) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    memcpy((char*)q->data + q->tail * q->elem_size, elem, q->elem_size);
    q->tail = (q->tail + 1) % q->capacity;
    ++q->size;

    return EXIT_SUCCESS;
}

int queue_pop(struct queue *q, void *elem) 
{
    assert(q && elem);

    if (queue_empty(q))
        return EXIT_FAILURE;

    memcpy(elem, (char*)q->data + q->head * q->elem_size, q->elem_size);
    q->head = (q->head + 1) % q->capacity;
    --q->size;

    return EXIT_SUCCESS;
}

int queue_empty(struct queue const *q) 
{
    assert(q);
    return q->size == 0;
}

struct queue *queue_delete(struct queue *q) 
{
    if (!q) 
        return nullptr;

    free(q->data);
    free(q);

    return nullptr;
}

void queue_print(struct queue const *q, void (*pf)(void const *data)) 
{
    assert(q);

    if (!q->data || q->size == 0) 
    {
        puts("[]");
        return;
    }

    printf("[");
    for (size_t i = 0; i < q->size; i++) 
    {
        size_t idx = (q->head + i) % q->capacity;
        pf((char*)q->data + idx * q->elem_size);
        if (i + 1 < q->size) printf(", ");
    }
    puts("]");
}

int adapt_queue(struct queue *q) 
{
    assert(q);

    if (q->size < q->capacity)
        return EXIT_SUCCESS; 

    size_t new_capacity = q->capacity * 2;
    void *new_data = calloc(new_capacity, q->elem_size);
    if (!new_data)
        return EXIT_FAILURE;

    if (q->head < q->tail) 
    {
        memcpy(new_data,
               (char*)q->data + q->head * q->elem_size,
               q->size * q->elem_size);
    } 
    else 
    {
        size_t first_part = q->capacity - q->head;
        memcpy(new_data,
               (char*)q->data + q->head * q->elem_size,
               first_part * q->elem_size);
        memcpy((char*)new_data + first_part * q->elem_size,
               q->data,
               q->tail * q->elem_size);
    }

    free(q->data);
    q->data = new_data;
    q->capacity = new_capacity;
    q->head = 0;
    q->tail = q->size;

    return EXIT_SUCCESS;
}