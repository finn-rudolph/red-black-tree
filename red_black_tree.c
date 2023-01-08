#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "red_black_tree.h"

struct RbTree
{
    RbNode *root, *nil;
    size_t value_size;
    rbtree_compare compare;
};

struct RbNode
{
    RbNode *parent, *left, *right;
    void *key;
    bool color;
};

RbNode *rbnode_create(
    size_t value_size, void const *const restrict value, RbNode *nil)
{
    RbNode *z = malloc(sizeof *z);
    z->left = z->right = z->parent = nil;
    z->key = malloc(value_size);
    z->color = RB_BLACK;
    memcpy(z->key, value, value_size);

    return z;
}

RbNode *rbnode_create_nil()
{
    RbNode *z = malloc(sizeof *z);
    z->color = RB_BLACK;
    z->key = 0;
    z->left = z->right = z->parent = z;
    return z;
}

void rbnode_destroy_subtree(RbTree const *const t, RbNode *x)
{
    assert(x != t->nil);

    if (x->left != t->nil)
        rbnode_destroy_subtree(t, x->left);
    if (x->right != t->nil)
        rbnode_destroy_subtree(t, x->right);

    free(x->key);
    free(x);
}

void rbnode_destroy_nil(RbNode *nil)
{
    free(nil);
}

void *rbnode_get_key(RbNode const *const x)
{
    return x->key;
}

RbTree *rbtree_create(size_t value_size, rbtree_compare compare)
{
    RbTree *t = malloc(sizeof *t);
    t->value_size = value_size;
    t->compare = compare;
    t->nil = rbnode_create_nil();
    t->root = t->nil;
    return t;
}

void rbtree_destroy(RbTree *const t)
{
    if (t->root != t->nil)
        rbnode_destroy_subtree(t, t->root);
    rbnode_destroy_nil(t->nil);
    free(t);
}

RbNode *rbtree_get_nil(RbTree const *const t)
{
    return t->nil;
}

void rbtree_left_rotate(RbTree *const t, RbNode *const x)
{
    RbNode *const y = x->right;

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

void rbtree_right_rotate(RbTree *const t, RbNode *const x)
{
    RbNode *const y = x->left;

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

RbNode *rbtree_min_node(RbTree const *const t, RbNode *root)
{
    while (root->left != t->nil)
        root = root->left;
    return root;
}

RbNode *rbtree_find(RbTree const *const t, void const *const restrict value)
{
    RbNode *x = t->root;

    while (x != t->nil)
    {
        if ((*t->compare)(value, x->key))
            x = x->left;
        else if ((*t->compare)(x->key, value))
            x = x->right;
        else
            break;
    }

    return x == t->nil ? 0 : x;
}

void rbtree_insert_fixup(RbTree *const t, RbNode *z)
{
    while (z->parent->color == RB_RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            RbNode *const y = z->parent->parent->right; // z's uncle
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
                    rbtree_left_rotate(t, z);
                }
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rbtree_right_rotate(t, z->parent->parent);
            }
        }
        else
        {
            RbNode *const y = z->parent->parent->left;
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
                    rbtree_right_rotate(t, z);
                }
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rbtree_left_rotate(t, z->parent->parent);
            }
        }
    }
}

void rbtree_insert_node(RbTree *const t, RbNode *const z)
{
    RbNode *y = t->nil, *x = t->root;

    while (x != t->nil)
    {
        y = x;
        if ((*t->compare)(z->key, x->key))
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == t->nil)
        t->root = z;
    else if ((*t->compare)(z->key, y->key))
        y->left = z;
    else
        y->right = z;

    z->left = t->nil;
    z->right = t->nil;
    z->color = RB_RED;

    rbtree_insert_fixup(t, z);
    t->root->color = RB_BLACK;
}

RbNode *rbtree_insert(RbTree *const t, void const *const restrict value)
{
    RbNode *const z = rbnode_create(t->value_size, value, t->nil);
    rbtree_insert_node(t, z);
    return z;
}

void rbtree_transplant(RbTree *const t, RbNode const *const u, RbNode *const v)
{
    if (u->parent == t->nil)
        t->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void rbtree_delete_fixup(RbTree *const t, RbNode *x)
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
                rbtree_left_rotate(t, x->parent);
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
                    rbtree_right_rotate(t, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->right->color = RB_BLACK;
                rbtree_left_rotate(t, x->parent);
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
                rbtree_right_rotate(t, x->parent);
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
                    rbtree_left_rotate(t, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->left->color = RB_BLACK;
                rbtree_right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }

    x->color = RB_BLACK;
}

void rbtree_delete_node(RbTree *const t, RbNode *const z)
{
    RbNode *y = z, *x = 0;
    bool y_ocolor = y->color;

    if (z->left == t->nil)
    {
        x = z->right;
        rbtree_transplant(t, z, z->right);
    }
    else if (z->right == t->nil)
    {
        x = z->left;
        rbtree_transplant(t, z, z->left);
    }
    else
    {
        y = rbtree_min_node(t, z->right);
        y_ocolor = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else
        {
            rbtree_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbtree_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_ocolor == RB_BLACK)
        rbtree_delete_fixup(t, x);
}

void rbtree_delete(RbTree *const t, void const *const restrict value)
{
    RbNode *const z = rbtree_find(t, value);
    if (z)
        rbtree_delete_node(t, z);
}
