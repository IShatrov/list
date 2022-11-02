#include "list.h"

#define PRINT_ERR(code, message) if((err_code & code) == code) fprintf(log, "ERROR: %s\n", #message)

#define PRINT_ERRORS                                    \
    PRINT_ERR(BAD_SIZE, BAD SIZE);                      \
    PRINT_ERR(BAD_TAIL, BAD TAIL);                      \
    PRINT_ERR(BAD_FREE, BAD FREE);                      \
                                                        \
    PRINT_ERR(WRONG_PREV, PREVIOUS ELEMENT MISMATCH);   \
    PRINT_ERR(WRONG_PREV_EMPTY, EMPTY CELL PREV NOT -1);\

#define PRINT_CONTENT(specifier, val) for(int i = 0; i < lst->size; i++) fprintf(log, specifier, val)

#define LOG_PRINT(string) fprintf(log, string)

void list_dump(my_list *lst, int err_code)
{
    assert(lst);

    FILE *log = lst->log;

    if(lst->log)
    {
        PRINT_CONTENT("%3d", i);

        LOG_PRINT("\ndata:\n");

        PRINT_CONTENT("%3c", lst->data[i].value);

        LOG_PRINT("\nnext:\n");

        PRINT_CONTENT("%3d", lst->data[i].next);

        LOG_PRINT("\nprev:\n");

        PRINT_CONTENT("%3d", lst->data[i].prev);

        fprintf(log, "\nsize: %d, tail: %d, free: %d, is_linear: %d\n",
            lst->size, lst->tail, lst->free, lst->is_linear);

        PRINT_ERRORS;

        fputc('\n', log);
        LOG_PRINT("===================================\n");
        fputc('\n', log);
    }

    if(lst->create_pretty_dump) list_pretty_dump(lst, err_code);

    return;
}

void list_pretty_dump(my_list *lst, int err_code)
{
    assert(lst);

    FILE *gr_viz = NULL;
    gr_viz = fopen("graphviz_code.txt", "w");
    assert(gr_viz);

    fprintf(gr_viz, "digraph dump\n{\n"
                    "\trankdir = LR;\n"
                    "\tnode[shape = \"record\", style = \"rounded\"];\n\n"
                    "\thead[shape = \"circle\", style = \"filled\", fillcolor = \"#FC5E53\"]\n"
                    "\thead->0\n\n"
            );

    ssize_t current = 0;
    int i = 0;
    for(; i < lst->size; i++)
    {
        fprintf(gr_viz, "\t%d[label = \"%d | val: %d | next: %d | prev: %d\"];\n", current, current,
            lst->data[current].value, lst->data[current].next, lst->data[current].prev);

        fprintf(gr_viz, "\t%d->%d[color = \"red\"];\n", current, lst->data[current].next);
        fprintf(gr_viz, "\t%d->%d[color = \"blue\"];\n\n", current, lst->data[current].prev);

        if(!(current = lst->data[current].next)) break; //stop if reached 0
    }

    current = lst->free;
    fprintf(gr_viz, "\tfree[shape = \"circle\", style = \"filled\", fillcolor = \"#FC5E53\"]\n");
    fprintf(gr_viz, "\tfree->%d\n\n", current);

    for(; i < lst->size; i++)
    {
        fprintf(gr_viz, "\t%d[label = \"%d | val: %d | next: %d | prev: %d\"];\n", current, current,
            lst->data[current].value, lst->data[current].next, lst->data[current].prev);

        fprintf(gr_viz, "\t%d->%d[color = \"red\"];\n", current, lst->data[current].next);

        if(!(current = lst->data[current].next)) break; //stop if reached 0
    }

    putc('}', gr_viz);

    fclose(gr_viz);

    system("dot.exe -T png -o dump_pic.png graphviz_code.txt");

    FILE *log = NULL;
    log = fopen("list_dump.htm", "w");
    assert(log);

    fprintf(log, "<pre>\n");

    PRINT_ERRORS;

    fprintf(log, "size = %d, is_linear = %d\n", lst->size, lst->is_linear);
    fprintf(log, "<img src=dump_pic.png>");

    fclose(log);

    return;
}

char list_assert(my_list *lst)
{
    assert(lst);

    int err_code = 0;

    if(lst->size <= 0) err_code |= BAD_SIZE;
    if(lst->tail < 0) err_code |= BAD_TAIL;
    if(lst->free < 0) err_code |= BAD_FREE;

    ssize_t check = 0;
    int i = 0;
    for(; i < lst->size; i++)
    {
        if(lst->data[lst->data[check].next].prev && check != lst->data[lst->data[check].next].prev)
        {
            err_code |= WRONG_PREV;

            break;
        }

        if(!(check = lst->data[check].next)) break; //stop if reached 0
    }

    check = lst->free;
    for(; i < lst->size; i++)
    {
        if(check && lst->data[check].prev != EMPTY_PREV)
        {
            err_code |= WRONG_PREV_EMPTY;

            break;
        }

        if(!(check = lst->data[check].next)) break; //stop if reached 0
    }

    if(err_code) list_dump(lst, err_code);

    return !err_code;
}

