#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct list
{
    int data;
    list *next;
};

struct list *list_new(int elem);
struct list *list_insert(struct list *head, int elem);
struct list *list_find(struct list *head, int elem);
struct list *list_erase(struct list *head, int elem);
struct list *list_insert_after(struct list *head, struct list *where, struct list *what);
struct list *list_insert_before(struct list *head, struct list *where, struct list *what);
struct list *list_delete(struct list *head);
struct list *list_next(struct list *curr);
void list_print(struct list const *head);

struct list *list_new(int elem)
{
    list *head = (list *) calloc(1, sizeof(list)) ;
    if (!head)
        return nullptr;

    head->data = elem;
    head->next = nullptr;

    return head;
}

struct list *list_insert(struct list *head, int elem)
{
    assert(head);

    struct list *new_node = list_new(elem);
    
    if (!new_node) return head;
    if (!head)     return new_node;
    else
    {
        list *list_iter = head;
        while (list_iter->next)
            list_iter = list_iter->next;
        
        list_iter->next = new_node;
    }

    return head;
}

struct list *list_find(struct list *head, int elem)
{
    list *list_iter = head;
    while (list_iter)
    {
        if (list_iter->data == elem)
            return list_iter;

        list_iter = list_iter->next;
    }
    return nullptr;
}

struct list *list_erase(struct list *head, int elem)
{
    if (!head) return nullptr;
 
    if (head->data == elem)
    {
        list *head_to_free = head;
        head = head->next;
        free(head_to_free);
        return head;
    }

    list *list_iter = head;
    while (list_iter->next)
    {
        if (list_iter->next->data == elem)
        {
            struct list *node_to_free = list_iter->next;
            list_iter->next = list_iter->next->next;
            free(node_to_free);
            return head;
        }

        list_iter = list_iter->next;
    }
    
    return head;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what)
{
    if (!what)  return head;

    if (!head)
    {
        what->next = nullptr;
        return what;
    }

    if (!where) return head;

    what->next  = where->next;
    where->next = what;

    return head;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what)
{
    if (!what) return head;

    if (!head)
    {
        what->next = nullptr;
        return what;
    }

    if (!where) return head;
    
    if (head == where)
    {
        what->next = head;
        return what;
    }

    struct list *curr = head;
    while (curr && curr->next != where)
        curr = curr->next;
    
    if (!curr) return head;
    
    curr->next = what;
    what->next = where;
    return head;
}

struct list *list_delete(struct list *head)
{
    while (head) 
    {
        struct list *next_free = head->next;
        free(head);
        head = next_free;
    }

    return nullptr;
}

struct list *list_next(struct list *curr)
{
    if (!curr) return nullptr;

    return curr->next;
}

void list_print(struct list const *head)
{
    if (!head) 
    {
        puts("[]");
        return;
    }

    putchar('[');

    list list_iter = *head;
    while (list_iter.next)
    {
        printf("%d, ", list_iter.data);
        list_iter = *list_iter.next;
    }
    printf("%d]\n", list_iter.data);
}
/*
int main() 
{
    struct list *head = list_new(0);
    head = list_insert(head, 1);
    list_print(head);
    head = list_delete(head);
    list_print(head);
}*/