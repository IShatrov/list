#include "list.h"

void list_ctor(my_list *lst, ssize_t lst_size, FILE *log)
{
    assert(lst);
    assert(lst_size > 0);

    lst->size = lst_size;
    lst->log = log;

    lst->data = (lst_data*)calloc(lst_size, sizeof(lst_data));
    assert(lst->data);

    lst->data[0].next = 0;     //make zero cell point to itself
    lst->data[0].prev = 0;

    for(int i = 1; i < lst_size; i++) //mark all cells as empty
    {
        lst->data[i].next = i + 1;
        lst->data[i].prev = -1;
    }
    lst->data[lst_size - 1].next = 0; //last elem point to zero

    lst->head = 0;
    lst->tail = 0;
    lst->free = 1;

    list_assert(lst);

    return;
}

void list_dtor(my_list *lst)
{
    assert(lst);

    list_assert(lst);

    for(int i = 0; i < lst->size; i++)
    {
        lst->data[i].next = -100;
        lst->data[i].prev = -100;
    }

    lst->size = -1;
    lst->head = -1;
    lst->tail = -1;
    lst->free = -1;
    lst->log = NULL;

    free(lst->data);

    return;
}

int list_insert_after(my_list *lst, lst_elem value, ssize_t index)
{
    assert(lst);
    assert(index >= 0);

    assert(lst->free > 0 && "Attempting to insert to full list\n");

    assert(lst->data[index].prev != -1); //not allow to insert after empty cell

    int free_cell = find_free_cell(lst);
    assert(free_cell > 0);

    lst->data[free_cell].value = value;

    if(index != 0)
    {
        lst->data[free_cell].next = lst->data[index].next;
        lst->data[free_cell].prev = index;

        if(index != lst->tail) lst->data[lst->data[index].next].prev = free_cell;
        else lst->tail = free_cell;

        lst->data[index].next = free_cell;
    }
    else
    {
        lst->data[free_cell].next = lst->head;
        lst->data[free_cell].prev = 0;

        lst->head = free_cell;

        if(index == lst->tail) lst->tail = free_cell;
    }

    list_assert(lst);

    return free_cell;
}

int list_insert_back(my_list *lst, lst_elem value)
{
    assert(lst);

    assert(lst->free > 0 && "Attempting to insert to full list\n");

    list_assert(lst);

    int ans = list_insert_after(lst, value, lst->tail);

    return ans;
}

void list_del(my_list *lst, ssize_t index)
{
    assert(lst);

    assert(index > 0);

    assert(lst->data[index].prev != -1);

    list_assert(lst);

    if(index != lst->head)
    {
        lst->data[lst->data[index].prev].next = lst->data[index].next;

        if(index != lst->tail) lst->data[lst->data[index].next].prev = lst->data[index].prev;
        else lst->tail = lst->data[index].prev;
    }
    else
    {
        lst->head = lst->data[index].next;

        lst->data[lst->data[index].next].prev = 0;

        if(lst->head == 0) lst->tail = 0;
    }

    lst->data[index].prev = -1;

    list_assert(lst);

    return;
}

int find_free_cell(my_list *lst)
{
    assert(lst);

    int ans = -1;

    for(int i = 0; i < lst->size; i++)
    {
        if(lst->data[i].prev == -1)
        {
            ans = i;
            break;
        }
    }

    return ans;
}
