#ifndef LIST
#define LIST

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

enum lst_err_codes
{
    BAD_SIZE          = 1<<0,
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

    ssize_t tail; //index of last added element

    ssize_t free;

    FILE* log;
};

typedef struct my_list_ my_list;

//! @param[in] lst Pointer to list.
//! @param[in] err_code Error code.
//! @brief Creates dump if log field of lst not NULL.
void list_dump(my_list *lst, int err_code);

//! @param[in] lst Pointer to list.
//! @return Returns 1 if no erros were detected.
//! @brief Checks lst for errors, calls list_dump() if an error was detected.
char list_assert(my_list *lst);

//! @param[in] lst Pointer to list.
//! @param[in] lst_size Size of buffer.
//! @param[in] log File to place dumps at, pass NULL to not create dumps.
//! @param[out] lst Initiallised list.
//! @brief Creates list.
void list_ctor(my_list *lst, ssize_t lst_size, FILE *log);

//! @param[in] lst Pointer to list.
//! @brief Destroys list.
void list_dtor(my_list *lst);

//! @param[in] lst Pointer to list.
//! @param[in] value Value to insert.
//! @param[in] index Physical address to insert after.
//! @return Returns physical address of inserted element.
//! @brief Inserts element after certain physical address.
ssize_t list_insert_after(my_list *lst, lst_elem value, ssize_t index);

//! @param[in] lst Pointer to list.
//! @param[in] value Value to insert.
//! @return Returns physical address of inserted element.
//! @brief Inserts element after last inserted element.
ssize_t list_insert_back(my_list *lst, lst_elem value);

//! @param[in] lst Pointer to list.
//! @param[in] index Physical address to delete.
//! @brief Deletes element.
void list_del(my_list *lst, ssize_t index);

//! @param[in] lst Pointer to list.
//! @param[in] num Logical number.
//! @return Returns physical address of element.
//! @brief Transforms logical number to physical.
ssize_t logic_to_phys(my_list *lst, ssize_t num);

//! @param[in] lst Pointer to list.
//! @brief Linearises list.
void linearise(my_list *lst);

#endif
