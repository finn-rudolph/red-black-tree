#include <stddef.h>
#include <stdbool.h>

#define RB_RED 0
#define RB_BLACK 1

typedef bool (*rbtree_compare)(void const *const a, void const *const b);

typedef struct RbNode RbNode;
struct RbNode;

typedef struct RbTree RbTree;
struct RbTree;

RbTree *rbtree_create(size_t value_size, rbtree_compare compare);

void rbtree_destroy(RbTree *const t);

RbNode *rbtree_insert(RbTree *const t, void const *const restrict value);

void rbtree_delete(RbTree *const t, void const *const restrict value);

RbNode *rbtree_find(RbTree const *const t, void const *const restrict value);