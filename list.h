#ifndef LIST
#define LIST

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

enum lst_err_codes
{
    BAD_SIZE          = 1<<0,
    BAD_HEAD          = 1<<1,
    BAD_TAIL          = 1<<2,
    BAD_FREE          = 1<<3,

    ZERO_ELMT_CHANGED = 1<<4,

    WRONG_PREV        = 1<<5,

    WRONG_PREV_EMPTY  = 1<<6,
};

typedef char lst_elem;

struct lst_data_
{
    lst_elem value;

    int next;

    int prev;
};

typedef struct lst_data_ lst_data;

struct my_list_
{
    ssize_t size;

    lst_data* data;

    ssize_t head;

    ssize_t tail;

    ssize_t free;

    FILE* log;
};

typedef struct my_list_ my_list;

void list_dump(my_list *lst, int err_code);

//! @return Returns 1 if all OK
char list_assert(my_list *lst);

void list_ctor(my_list *lst, ssize_t lst_size, FILE *log);

void list_dtor(my_list *lst);

ssize_t list_insert_after(my_list *lst, lst_elem value, ssize_t index);

ssize_t list_insert_back(my_list *lst, lst_elem value);

void list_del(my_list *lst, ssize_t index);

#endif
