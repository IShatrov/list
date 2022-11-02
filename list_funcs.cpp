#include "list.h"

#define MARK_AS_FREE(index)           \
lst->data[index].prev = EMPTY_PREV;   \
lst->data[index].next = lst->free;    \
lst->free = index;                    \


void list_ctor(my_list *lst, ssize_t lst_size, char create_dump)
{
    assert(lst);
    assert(lst_size > 0);

    lst->size = lst_size;
    if(create_dump)
    {
        lst->log = fopen("lst_log.txt", "w");
        assert(lst->log);
    }

    lst->data = (lst_data*)calloc(lst_size, sizeof(lst_data));
    assert(lst->data);

    lst->data[0].next = ZERO_ELMT;     //make zero cell point to itself
    lst->data[0].prev = ZERO_ELMT;

    for(int i = 1; i < lst_size; i++) //mark all cells as empty
    {
        lst->data[i].next = i + 1;
        lst->data[i].prev = EMPTY_PREV;
    }
    lst->data[lst_size - 1].next = ZERO_ELMT; //last elem point to zero

    lst->tail = ZERO_ELMT;
    lst->free = 1;
    lst->is_linear = 1;

    list_assert(lst);

    return;
}

void list_dtor(my_list *lst)
{
    assert(lst);
    list_assert(lst);

    for(int i = 0; i < lst->size; i++)
    {
        lst->data[i].next = ELMT_POISON;
        lst->data[i].prev = ELMT_POISON;
    }

    lst->size = INDEX_POISON;
    lst->tail = INDEX_POISON;
    lst->free = INDEX_POISON;

    fclose(lst->log);
    lst->log = NULL;

    free(lst->data);
    lst->data = NULL;

    return;
}

ssize_t list_insert_after(my_list *lst, lst_elem value, ssize_t index)
{
    assert(lst);
    assert(index >= 0);
    assert(lst->data[index].prev != EMPTY_PREV); //not allow to insert after empty cell
    list_assert(lst);

    if(!lst->free) list_realloc(lst, 2*lst->size);

    if(!(lst->is_linear && index == lst->tail)) lst->is_linear = 0;

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
    list_assert(lst);

    if(!lst->free) list_realloc(lst, 2*lst->size);

    ssize_t ans = list_insert_after(lst, value, lst->tail);

    return ans;
}

void list_del(my_list *lst, ssize_t index)
{
    assert(lst);
    assert(index > 0);
    assert(lst->data[index].prev != EMPTY_PREV);
    list_assert(lst);

    if(!(lst->is_linear && index == lst->tail)) lst->is_linear = 0;

    lst->data[lst->data[index].prev].next = lst->data[index].next;

    lst->data[lst->data[index].next].prev = lst->data[index].prev;

    lst->tail = lst->data[0].prev;

    MARK_AS_FREE(index);

    list_assert(lst);

    return;
}

void list_realloc(my_list *lst, ssize_t new_size)
{
    assert(lst);
    assert(new_size > lst->size);
    list_assert(lst);

    lst->data = (lst_data*)realloc(lst->data, new_size*sizeof(lst_data));
    assert(lst->data);

    ssize_t index = lst->size;
    for(index = lst->size; index < new_size; index++)
    {
        MARK_AS_FREE(index);
    }

    lst->size = new_size;

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

    if(lst->is_linear) return;

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
    new_data[i].next = ZERO_ELMT;

    lst->tail = i;
    lst->free = ++i;

    for(; i< lst->size; i++)
    {
        new_data[i].next = i + 1;
        new_data[i].prev = EMPTY_PREV;
    }

    new_data[i - 1].next = ZERO_ELMT;

    free(lst->data);
    lst->data = new_data;

    lst->is_linear = 1;

    list_assert(lst);

    return;
}

char check_lin(my_list *lst)
{
    return lst->is_linear;
}
