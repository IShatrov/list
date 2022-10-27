#include "list.h"

void list_ctor(my_list *lst, ssize_t lst_size, FILE *log, char create_pretty_dump)
{
    assert(lst);
    assert(lst_size > 0);

    lst->size = lst_size;
    lst->log = log;
    lst->create_pretty_dump = create_pretty_dump;

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
    lst->tail = -1;
    lst->free = -1;
    lst->log = NULL;

    free(lst->data);

    return;
}

ssize_t list_insert_after(my_list *lst, lst_elem value, ssize_t index)
{
    assert(lst);
    assert(index >= 0);

    assert(lst->free > 0 && "Attempting to insert to full list\n");

    assert(lst->data[index].prev != -1); //not allow to insert after empty cell

    ssize_t free_cell = lst->free;
    assert(free_cell > 0);
    lst->free = lst->data[free_cell].next;

    lst->data[free_cell].value = value;

    lst->data[free_cell].next = lst->data[index].next;
    lst->data[free_cell].prev = index;

    lst->data[lst->data[index].next].prev = free_cell;
    lst->data[index].next = free_cell;

    lst->tail = lst->data[0].prev;

    list_assert(lst);

    return free_cell;
}

ssize_t list_insert_back(my_list *lst, lst_elem value)
{
    assert(lst);

    assert(lst->free > 0 && "Attempting to insert to full list\n");

    list_assert(lst);

    ssize_t ans = list_insert_after(lst, value, lst->tail);

    return ans;
}

void list_del(my_list *lst, ssize_t index)
{
    assert(lst);

    assert(index > 0);

    assert(lst->data[index].prev != -1);

    list_assert(lst);

    lst->data[lst->data[index].prev].next = lst->data[index].next;

    lst->data[lst->data[index].next].prev = lst->data[index].prev;

    lst->tail = lst->data[0].prev;
    lst->data[index].prev = -1;
    lst->data[index].next = lst->free;
    lst->free = index;

    list_assert(lst);

    return;
}

ssize_t logic_to_phys(my_list *lst, ssize_t num)
{
    assert(lst);

    assert(num >= 0);

    list_assert(lst);

    ssize_t current = 0, i = 0;
    for(i = 0; i < num; i++)
    {
        current = lst->data[current].next;

        if(!current) return -1;
    }

    return current;
}

void linearise(my_list *lst)
{
    assert(lst);

    list_assert(lst);

    lst_data *new_data = (lst_data*)calloc(lst->size, sizeof(lst_data));
    assert(new_data);

    int i = 0;
    ssize_t current = 0;
    for(i = 0; i < lst->size; i++)
    {
        new_data[i].value = lst->data[current].value;
        new_data[i].next = i + 1;
        new_data[i].prev = i - 1;

        if(!(current = lst->data[current].next)) break;
    }

    new_data[0].prev = i;
    new_data[i].next = 0;

    lst->tail = i;
    lst->free = ++i;

    for(; i< lst->size; i++)
    {
        new_data[i].next = i + 1;
        new_data[i].prev = -1;
    }

    new_data[i - 1].next = 0;

    free(lst->data);
    lst->data = new_data;

    list_assert(lst);

    return;
}
