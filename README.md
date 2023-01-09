# Type-Generic Red-Black Tree in C

To use the tree, include `red_black_tree.h`. The supported functionality is documented below. Note that the two objects `RbTree` and `RbNode` are not abstract, this allows declaring some functions as `inline`. However, the objects shall never be modified directly. The tree operations can be tested manually with the program in `main.c`.

**`RbTree *rb_create(size_t key_size, rb_compare_fn compare)`**

Creates a new Red-Black Tree object and returns an `RbTree *` needed for further operations on the tree. The size of the keys to be stored as well as a comparison function (same form as for `qsort`) must be provided.

**`void rb_destroy(RbTree *const t)`**

Must be called after using the Red-Black Tree.

**`RbNode *rb_nil(RbTree const *const t)`**

Returns the nil-node of the Red-Black Tree. This special node can be returned from `rb_find`, `rb_min`, `rb_max`, `rb_predecessor` and `rb_sucessor`. It signifies that the queried node does not exist. Doing anything else with a nil-node than comparing it with the return value of this function for equality is undefined behaviour.

**`RbNode *rb_insert(RbTree *const t, void const *const restrict key)`**

Inserts the key pointed to into the tree and returns the newly created node.

**`void rb_delete(RbTree *const t, void const *const restrict key)`**

Deletes the given key from the tree.

**`size_t rb_length(RbTree const *const t)`**

Returns the number of elements in the tree.

**`RbNode *rb_find(RbTree const *const t, void const *const restrict key)`**

Returns a pointer to the node containing the given key or the nil-node, if the key does not exist.

**`RbNode *rb_min(RbTree const *const t)`**

Returns the node with the least key. 

**`RbNode *rb_max(RbTree const *const t)`**

Returns the node with greatest key.

**`RbNode *rb_predecessor(RbTree const *const t, RbNode const *const node)`**

Returns the previous node in the order given by the comparison function.

**`RbNode *rb_successor(RbTree const *const t, RbNode const *const node)`**

Returns the next node in the order given by the comparison function.

**`void *rb_get_key(RbNode const *const node)`**

Used to get the key of a node.

**`void rb_print(RbTree const *const t, rb_print_fn print_key, size_t node_width)`**

Prints the tree structure of the Red-Black Tree to the terminal. The user must provide a function of type `rb_print_fn` (defined in `red_black_tree.h`) which receives a `void *` of a node key and prints some string representing that node to Standard Output. Exactly `node_width` characters must be printed and new lines are not allowed. The printing function should try to center it's content if it does not span the whole width. Red nodes are automatically printed red.