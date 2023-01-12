#include <stdio.h>
#include <string.h>

#include "red_black_tree.h"

int int_less(void const *const a, void const *const b)
{
    return *(int *)a - *(int *)b;
}

void int_print(void const *const x)
{
    size_t const size = 4;
    char buffer[size];
    memset(buffer, 0, size);
    sprintf(buffer, "%d", *(int *)x);
    for (size_t i = 0; i < (size - strlen(buffer)) / 2; i++)
        putchar(' ');
    printf("%s", buffer);
    for (size_t i = 0; i < (size - strlen(buffer) + 1) / 2; i++)
        putchar(' ');
}

int main()
{
    RbTree *t = rb_create(sizeof(int), int_less);
    char command[64];

    while (1)
    {
        scanf("%s", command);
        int x;

        if (!strcmp(command, "insert"))
        {
            scanf("%d", &x);
            rb_insert(t, &x);
        }
        else if (!strcmp(command, "delete"))
        {
            scanf("%d", &x);
            rb_delete(t, &x);
        }
        else if (!strcmp(command, "find"))
        {
            scanf("%d", &x);
            if (rb_find(t, &x) != rb_nil(t))
                printf("Is in the tree.\n");
            else
                printf("Is not in the tree\n");
        }
        else if (!strcmp(command, "rank"))
        {
            scanf("%d", &x);
            printf("%zu\n", rb_rank(t, &x));
        }
        else if (!strcmp(command, "find-ith"))
        {
            scanf("%d", &x);
            printf("%d\n", *(int *)rb_get_key(rb_find_ith(t, x)));
        }
        else if (!strcmp(command, "length"))
        {
            printf("%zu\n", rb_length(t));
        }
        else if (!strcmp(command, "print"))
        {
            rb_print(t, int_print, 4);
        }
        else if (!strcmp(command, "quit"))
            break;
    }

    rb_destroy(t);
}