#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define RB_RED 0
#define RB_BLACK 1

// return value: a < b -> negative, a == b -> 0, a > b -> positive
typedef int (*rb_compare_fn)(void const *const a, void const *const b);

typedef void (*rb_print_fn)(void const *const x);

typedef struct RbNode RbNode;
struct RbNode
{
    RbNode *parent, *left, *right;
    void *key;
    size_t subtree_size;
    bool color;
};

typedef struct RbTree RbTree;
struct RbTree
{
    size_t key_size;
    rb_compare_fn compare;
    RbNode *root, *nil;
};

RbTree *rb_create(size_t key_size, rb_compare_fn compare);

void rb_destroy(RbTree *const t);

RbNode *rb_insert(RbTree *const t, void const *const restrict key);

void rb_delete(RbTree *const t, void const *const restrict key);

void rb_delete_all(RbTree *const t);

RbNode *rb_find(RbTree const *const t, void const *const restrict key);

RbNode *rb_lower_bound(RbTree const *const t, void const *const restrict key);

RbNode *rb_upper_bound(RbTree const *const t, void const *const restrict key);

RbNode *rb_predecessor(RbTree const *const t, RbNode const *const node);

RbNode *rb_successor(RbTree const *const t, RbNode const *const node);

size_t rb_count_less(RbTree const *const t, void const *const restrict key);

size_t rb_count_less_eq(RbTree const *const t, void const *const restrict key);

RbNode *rb_find_ith(RbTree const *const t, size_t i);

void rb_print(
    RbTree const *const t, rb_print_fn print_key, size_t node_width);

inline RbNode *rb_min(RbTree const *const t)
{
    RbNode *x = t->root;
    while (x->left != t->nil)
        x = x->left;
    return x;
}

inline RbNode *rb_max(RbTree const *const t)
{
    RbNode *x = t->root;
    while (x->right != t->nil)
        x = x->right;
    return x;
}

inline RbNode *rb_nil(RbTree const *const t)
{
    return t->nil;
}

inline size_t rb_length(RbTree const *const t)
{
    return t->root->subtree_size;
}

inline void *rb_get_key(RbNode const *const node)
{
    return node->key;
}
