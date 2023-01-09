#include <stdio.h>
#include <string.h>

#include "red_black_tree.h"

int int_less(void const *const a, void const *const b)
{
    return *(int *)a - *(int *)b;
}

void int_print(void const *const x)
{
    printf("%4d", *(int *)x);
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
        else if (!strcmp(command, "length"))
        {
            printf("%zu\n", rb_length(t));
        }
        else if (!strcmp(command, "print"))
        {
            RbNode *node = rb_min(t);

            while (node != rb_nil(t))
            {
                printf("%d ", *(int *)rb_get_key(node));
                node = rb_successor(t, node);
            }
            putchar('\n');
        }
        else if (!strcmp(command, "print-reverse"))
        {
            RbNode *node = rb_max(t);

            while (node != rb_nil(t))
            {
                printf("%d ", *(int *)rb_get_key(node));
                node = rb_predecessor(t, node);
            }
            putchar('\n');
        }
        else if (!strcmp(command, "print-tree"))
        {
            rb_print(t, int_print, 4);
        }
        else if (!strcmp(command, "quit"))
            break;
    }

    rb_destroy(t);
}