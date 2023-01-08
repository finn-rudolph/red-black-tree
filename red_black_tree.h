#include <stddef.h>
#include <stdbool.h>

#define RB_RED 0
#define RB_BLACK 1

typedef int (*rbtree_compare)(void const *const a, void const *const b);

typedef struct RbTree RbTree;
struct RbTree;

typedef struct RbNode RbNode;
struct RbNode;

RbTree *rbtree_create(size_t value_size, rbtree_compare compare);

void rbtree_destroy(RbTree *const t);

RbNode *rbtree_nil(RbTree const *const t);

RbNode *rbtree_insert(RbTree *const t, void const *const restrict value);

void rbtree_delete(RbTree *const t, void const *const restrict value);

size_t rbtree_length(RbTree const *const t);

RbNode *rbtree_find(RbTree const *const t, void const *const restrict value);

RbNode *rbtree_min(RbTree const *const t);

RbNode *rbtree_max(RbTree const *const t);

RbNode *rbtree_predecessor(RbTree const *const t, RbNode const *const node);

RbNode *rbtree_successor(RbTree const *const t, RbNode const *const node);

void *rbtree_get_key(RbNode const *const node);
