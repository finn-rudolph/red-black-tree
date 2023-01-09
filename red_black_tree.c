#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "red_black_tree.h"

// Ensure emission of inline functions in red_black_tree.h.
RbNode *rb_min(RbTree const *const t);

RbNode *rb_max(RbTree const *const t);

RbNode *rb_nil(RbTree const *const t);

size_t rb_length(RbTree const *const t);

void *rb_get_key(RbNode const *const node);

RbNode *rb_create_node(
    size_t key_size, void const *const restrict key, RbNode *nil)
{
    RbNode *z = malloc(sizeof *z);
    z->left = z->right = z->parent = nil;
    z->key = malloc(key_size);
    z->color = RB_BLACK;
    memcpy(z->key, key, key_size);

    return z;
}

void rb_destroy_node(RbNode *const x)
{
    free(x->key);
    free(x);
}

void rb_destroy_subtree(RbTree const *const t, RbNode *x)
{
    assert(x != t->nil);

    if (x->left != t->nil)
        rb_destroy_subtree(t, x->left);
    if (x->right != t->nil)
        rb_destroy_subtree(t, x->right);

    free(x->key);
    free(x);
}

RbTree *rb_create(size_t key_size, rb_compare_fn compare)
{
    RbTree *t = malloc(sizeof *t);
    t->key_size = key_size;
    t->length = 0;
    t->compare = compare;

    t->nil = malloc(sizeof *t->nil);
    t->nil->left = t->nil->right = t->nil->parent = t->nil;
    t->nil->key = 0;
    t->nil->color = RB_BLACK;
    t->root = t->nil;

    return t;
}

void rb_destroy(RbTree *const t)
{
    if (t->root != t->nil)
        rb_destroy_subtree(t, t->root);
    free(t->nil);
    free(t);
}

void rb_left_rotate(RbTree *const t, RbNode *const x)
{
    RbNode *const restrict y = x->right;

    x->right = y->left;
    if (y->left != t->nil)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == t->nil)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rb_right_rotate(RbTree *const t, RbNode *const x)
{
    RbNode *const restrict y = x->left;

    x->left = y->right;
    if (y->right != t->nil)
        y->right->parent = x;

    y->parent = x->parent;
    if (x->parent == t->nil)
        t->root = y;
    else if (x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}

RbNode *rb_min_in_subtree(RbTree const *const t, RbNode *root)
{
    while (root->left != t->nil)
        root = root->left;
    return root;
}

RbNode *rb_max_in_subtree(RbTree const *const t, RbNode *root)
{
    while (root->right != t->nil)
        root = root->right;
    return root;
}

RbNode *rb_find(RbTree const *const t, void const *const restrict key)
{
    RbNode *x = t->root;

    while (x != t->nil)
    {
        int const c = (*t->compare)(key, x->key);
        if (c < 0)
            x = x->left;
        else if (c > 0)
            x = x->right;
        else
            break;
    }

    return x;
}

RbNode *rb_predecessor(RbTree const *const t, RbNode const *node)
{
    if (node->left != t->nil)
        return rb_max_in_subtree(t, node->left);

    RbNode *y = node->parent;
    while (y != t->nil && node == y->left)
    {
        node = y;
        y = y->parent;
    }

    return y;
}

RbNode *rb_successor(RbTree const *const t, RbNode const *node)
{
    if (node->right != t->nil)
        return rb_min_in_subtree(t, node->right);

    RbNode *y = node->parent;
    while (y != t->nil && node == y->right)
    {
        node = y;
        y = y->parent;
    }

    return y;
}

void rb_insert_fixup(RbTree *const t, RbNode *z)
{
    while (z->parent->color == RB_RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            RbNode *const restrict y = z->parent->parent->right; // z's uncle
            if (y->color == RB_RED)
            {
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rb_left_rotate(t, z);
                }
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rb_right_rotate(t, z->parent->parent);
            }
        }
        else
        {
            RbNode *const restrict y = z->parent->parent->left;
            if (y->color == RB_RED)
            {
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rb_right_rotate(t, z);
                }
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rb_left_rotate(t, z->parent->parent);
            }
        }
    }
}

void rb_insert_node(RbTree *const t, RbNode *const z)
{
    t->length++;
    RbNode *y = t->nil, *x = t->root;

    while (x != t->nil)
    {
        y = x;
        if ((*t->compare)(z->key, x->key) < 0)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == t->nil)
        t->root = z;
    else if ((*t->compare)(z->key, y->key) < 0)
        y->left = z;
    else
        y->right = z;

    z->left = t->nil;
    z->right = t->nil;
    z->color = RB_RED;

    rb_insert_fixup(t, z);
    t->root->color = RB_BLACK;
}

RbNode *rb_insert(RbTree *const t, void const *const restrict key)
{
    RbNode *const z = rb_create_node(t->key_size, key, t->nil);
    rb_insert_node(t, z);
    return z;
}

void rb_transplant(RbTree *const t, RbNode const *const u, RbNode *const v)
{
    if (u->parent == t->nil)
        t->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void rb_delete_fixup(RbTree *const t, RbNode *x)
{
    while (x != t->root && x->color == RB_BLACK)
    {
        if (x == x->parent->left)
        {
            RbNode *w = x->parent->right; // x's sibling

            if (w->color == RB_RED)
            {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rb_left_rotate(t, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == RB_BLACK && w->right->color == RB_BLACK)
            {
                w->color = RB_RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == RB_BLACK)
                {
                    w->left->color = RB_BLACK;
                    w->color = RB_RED;
                    rb_right_rotate(t, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->right->color = RB_BLACK;
                rb_left_rotate(t, x->parent);
                x = t->root;
            }
        }
        else
        {
            RbNode *w = x->parent->left;

            if (w->color == RB_RED)
            {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rb_right_rotate(t, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == RB_BLACK && w->left->color == RB_BLACK)
            {
                w->color = RB_RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == RB_BLACK)
                {
                    w->right->color = RB_BLACK;
                    w->color = RB_RED;
                    rb_left_rotate(t, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->left->color = RB_BLACK;
                rb_right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }

    x->color = RB_BLACK;
}

void rb_delete_node(RbTree *const t, RbNode *const z)
{
    t->length--;
    RbNode *y = z, *x = 0;
    bool y_ocolor = y->color;

    if (z->left == t->nil)
    {
        x = z->right;
        rb_transplant(t, z, z->right);
    }
    else if (z->right == t->nil)
    {
        x = z->left;
        rb_transplant(t, z, z->left);
    }
    else
    {
        y = rb_min_in_subtree(t, z->right);
        y_ocolor = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else
        {
            rb_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rb_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_ocolor == RB_BLACK)
        rb_delete_fixup(t, x);
}

void rb_delete(RbTree *const t, void const *const restrict key)
{
    RbNode *const z = rb_find(t, key);
    if (z != t->nil)
    {
        rb_delete_node(t, z);
        rb_destroy_node(z);
    }
}
