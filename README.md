# Type-Generic Red-Black Tree in C

To use the tree, include `red_black_tree.h`. Standard Red-Black Tree operations as well as order-statistics operations are supported, as documented below. The tree operations can be tested in the terminal with the program in `main.c`.

## Creation and destruction

**`RbTree *rb_create(size_t key_size, rb_compare_fn compare)`**

Creates a new Red-Black Tree object and returns an `RbTree *` needed for further operations on the tree. The size of the keys to be stored as well as a comparison function must be provided. When the comparison function receives pointers to two keys `a` and `b`, the return value must be negative, if `a` is less than `b`, zero, if `a` equals `b`, and positive, if `a` is greater than `b` (same form as for `qsort`). Note that `RbTree` is not abstract (as well as `RbNode`), this allows declaring some functions as inline. However, the objects shall never be modified directly.

**`void rb_destroy(RbTree *const t)`**

Must be called after using the Red-Black Tree.

## Insertion and Deletion

**`RbNode *rb_insert(RbTree *const t, void const *const restrict key)`**

Inserts the key pointed to into the tree and returns a pointer to the newly created node. Inserting a key twice creates two separate nodes in the tree.

**`void rb_delete(RbTree *const t, void const *const restrict key)`**

Deletes one occurence of the given key from the tree.

**`void rb_delete_all(RbTree *const t)`**

Deletes all nodes from the tree, but leaves the tree itself valid. Calling this on any valid Red-Black Tree `t` puts `t` in a state as if it was just returned from `rb_create`.

## Querying the Red-Black Tree

**`size_t rb_length(RbTree const *const t)`**

Returns the number of elements in the tree.

**`RbNode *rb_nil(RbTree const *const t)`**

Returns the nil-node of the Red-Black Tree. This special node can be returned from `rb_find`, `rb_min`, `rb_max`, `rb_predecessor` and `rb_successor`. It signifies that the queried node does not exist. Doing anything else with a nil-node than comparing it with the return value of this function for equality is undefined behaviour.

**`void *rb_get_key(RbNode const *const node)`**

Get the key of a node.

**`RbNode *rb_find(RbTree const *const t, void const *const restrict key)`**

Returns a pointer to the node containing the given key or the nil-node, if the key does not exist.

**`RbNode *rb_lower_bound(RbTree const *const t, void const *const restrict key)`**

Returns a pointer to the first node with key greater or equal than the given key.

**`RbNode *rb_upper_bound(RbTree const *const t, void const *const restrict key)`**

Returns a pointer to the first node with key greater than the given key.

**`RbNode *rb_min(RbTree const *const t)`**

Returns the node with least key. 

**`RbNode *rb_max(RbTree const *const t)`**

Returns the node with greatest key.

**`RbNode *rb_predecessor(RbTree const *const t, RbNode const *const node)`**

Returns the previous node in the order given by the comparison function, or the nil-node, if the given node is the least in the tree.

**`RbNode *rb_successor(RbTree const *const t, RbNode const *const node)`**

Returns the next node in the order given by the comparison function, or the nil-node, if the given node is the greatest in the tree.

## Order-Statistics Operations

**`size_t rb_count_less(RbTree const *const t, void const *const restrict key)`**

Returns the number of elements in the tree less than the given key. (Or, the index of the first occurence of `key` in a sorted array of all elements.)

**`size_t rb_count_less_eq(RbTree const *const t, void const *const restrict key)`**

Returns the number of elements in the tree less than or equal to the given key. (Or, the index of the element after the last occurence of `key` in a sorted array of all elements.)

**`RbNode *rb_find_ith(RbTree const *const t, size_t i)`**

Finds a node having exactly i less or equal elements in the tree. (Or, equivalently, the i-th element in a sorted array of all elements in the tree.)

## Printing the Red-Black Tree to the terminal

**`void rb_print(RbTree const *const t, rb_print_fn print_key, size_t node_width)`**

Prints the tree structure of the Red-Black Tree to the terminal. The user must provide a function of type `rb_print_fn` (defined in `red_black_tree.h`) which receives a `void *` of a node key and prints some string representing that node to Standard Output. Exactly `node_width` characters must be printed and new lines are not allowed. The printing function should try to center it's content if it does not span the whole width. Red nodes are automatically printed red.