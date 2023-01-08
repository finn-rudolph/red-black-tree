#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define RB_RED 0
#define RB_BLACK 1

typedef int (*rb_compare_fn)(void const *const a, void const *const b);

typedef struct RbTree RbTree;
struct RbTree;

typedef struct RbNode RbNode;
struct RbNode;

RbTree *rb_create(size_t value_size, rb_compare_fn compare);

void rb_destroy(RbTree *const t);

RbNode *rb_nil(RbTree const *const t);

RbNode *rb_insert(RbTree *const t, void const *const restrict value);

void rb_delete(RbTree *const t, void const *const restrict value);

size_t rb_length(RbTree const *const t);

RbNode *rb_find(RbTree const *const t, void const *const restrict value);

RbNode *rb_min(RbTree const *const t);

RbNode *rb_max(RbTree const *const t);

RbNode *rb_predecessor(RbTree const *const t, RbNode const *const node);

RbNode *rb_successor(RbTree const *const t, RbNode const *const node);

void *rb_get_key(RbNode const *const node);
