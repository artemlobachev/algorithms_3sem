#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

struct set
{
    bool *data;
    size_t size;
    size_t capacity;
};

struct set *set_new(size_t capacity);
int set_insert(struct set *s, size_t elem);
int set_erase(struct set *s, size_t elem);
int set_find(struct set const *s, size_t elem);
struct set *set_delete(struct set *s);
int set_empty(struct set const *s);
ssize_t set_findfirst(struct set const *s, size_t start);
size_t set_size(struct set const *s);
void set_print(struct set const *s);


struct set *set_new(size_t capacity)
{
    if (capacity == 0) return nullptr;

    set *s = (set *) calloc(1, sizeof(set));
    if (!s)
        return nullptr;

    s->data = (bool *) calloc(capacity, sizeof(bool));
    if (!s->data)
    {
        free(s);
        return nullptr;
    }
    s->capacity  = capacity;
    s->size      = 0;

    return s;
}

int set_insert(struct set *s, size_t elem)
{
    assert(s);

    if (elem >= s->capacity) return 1;
    
    if (!s->data[elem])
    {
        s->size++;
        s->data[elem] = true;
    }

    return 0;
}

int set_erase(struct set *s, size_t elem)
{
    assert(s);

    if (elem >= s->capacity) return 1;
    
    if (s->data[elem])
    {
        s->size--;
        s->data[elem] = false;
    }

    return 0;
}

int set_find(struct set const *s, size_t elem)
{
    assert(s);

    if (elem >= s->capacity) return 0;
    
    return s->data[elem]; 
}

struct set *set_delete(struct set *s)
{
    if (!s) return NULL;

    free(s->data);
    free(s);

    return NULL;
}

int set_empty(struct set const *s)
{
    assert(s);
    
    return s->size == 0 ? 1 : 0;
}

ssize_t set_findfirst(struct set const *s, size_t start)
{
    assert(s);

    if (start >= s->capacity) return -1;
    if (set_empty(s)) return -1;

    for (size_t i = start; i < s->capacity; i++)
        if (s->data[i])
            return i;

    return -1;
}

size_t set_size(struct set const *s)
{
    assert(s);

    return s->size;
}

void set_print(struct set const *s)
{
    assert(s);

    if (set_empty(s))
    {
        puts("[]");
        return ;
    }

    putchar('[');

    for (size_t i = 0, j = 0; i < s->capacity; i++)
        if (s->data[i])
        {
            j++;
            if (j == s->size)
            {
                printf("%lu]\n", i);
                break;
            }
            printf("%lu, ", i);
        }
}

int main() {
    struct set *s = set_new(10000);
    set_insert(s, 1);
    set_insert(s, 9);
    set_insert(s, 3);
    set_insert(s, 0);
    set_insert(s, 9999);
    set_erase(s, 3);
    set_erase(s, 0);

    printf("%lu\n", set_size(s));

    set_print(s);
    assert(set_delete(s) == NULL);
}