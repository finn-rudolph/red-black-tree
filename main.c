#include <stdio.h>
#include <string.h>

#include "red_black_tree.h"

bool int_less(void const *const a, void const *const b)
{
    return *(int *)a < *(int *)b;
}

int main()
{
    RbTree *t = rbtree_create(sizeof(int), int_less);
    char command[64];

    while (1)
    {
        scanf("%s", command);
        int x;

        if (!strcmp(command, "insert"))
        {
            scanf("%d", &x);
            rbtree_insert(t, &x);
        }
        else if (!strcmp(command, "delete"))
        {
            scanf("%d", &x);
            rbtree_delete(t, &x);
        }
        else if (!strcmp(command, "find"))
        {
            scanf("%d", &x);
            if (rbtree_find(t, &x))
                printf("Is in the tree.\n");
            else
                printf("Is not in the tree\n");
        }
        else if (!strcmp(command, "quit"))
            break;
    }

    rbtree_destroy(t);
}