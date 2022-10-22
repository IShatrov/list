#include "list.h"

#define PRINT_ERR(code, message) if((err_code & code) == code) fprintf(lst->log, "ERROR: %s\n", #message)

void list_dump(my_list *lst, int err_code)
{
    assert(lst);

    if(lst->log)
    {
        for(int i = 0; i < lst->size; i++) printf("%3d", i);

        fprintf(lst->log, "\ndata:\n");

        for(int i = 0; i < lst->size; i++) printf("%3c", lst->data[i].value);

        fprintf(lst->log, "\nnext:\n");

        for(int i = 0; i < lst->size; i++) printf("%3d", lst->data[i].next);

        fprintf(lst->log, "\nprev:\n");

        for(int i = 0; i < lst->size; i++) printf("%3d", lst->data[i].prev);

        fprintf(lst->log, "\nsize: %d, head: %d, tail: %d, free: %d\n",
            lst->size, lst->head, lst->tail, lst->free);

        PRINT_ERR(BAD_SIZE, BAD SIZE);
        PRINT_ERR(BAD_HEAD, BAD HEAD);
        PRINT_ERR(BAD_TAIL, BAD TAIL);
        PRINT_ERR(BAD_FREE, BAD FREE);

        PRINT_ERR(ZERO_ELMT_CHANGED, ZERO ELEMENT CHANGED);

        PRINT_ERR(WRONG_PREV, PREVIOUS ELEMENT MISMATCH);
        PRINT_ERR(WRONG_PREV_EMPTY, EMPTY CELL PREV NOT -1);

        fputc('\n', lst->log);
        printf("==============================\n");
        fputc('\n', lst->log);
    }

    return;
}

char list_assert(my_list *lst)
{
    assert(lst);

    int err_code = 0;

    if(lst->size <= 0) err_code |= BAD_SIZE;
    if(lst->head < 0) err_code |= BAD_HEAD;
    if(lst->tail < 0) err_code |= BAD_TAIL;
    if(lst->free < 0) err_code |= BAD_FREE;

    if(lst->data[0].prev != 0) err_code |= ZERO_ELMT_CHANGED;
    if(lst->data[0].next != 0) err_code |= ZERO_ELMT_CHANGED;

    ssize_t check = lst->head;
    int i = 0;
    for(; i < lst->size; i++)
    {
        if(lst->data[lst->data[check].next].prev && check != lst->data[lst->data[check].next].prev)
        {
            err_code |= WRONG_PREV;

            break;
        }

        if(!(check = lst->data[check].next)) break;
    }

    check = lst->free;
    for(; i < lst->size; i++)
    {
        if(check && lst->data[check].prev != -1)
        {
            err_code |= WRONG_PREV_EMPTY;

            break;
        }

        if(!(check = lst->data[check].next)) break;
    }

    if(err_code) list_dump(lst, err_code);

    return !err_code;
}
