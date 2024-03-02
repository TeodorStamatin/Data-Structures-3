#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct vectorstruct vectorstruct;
struct vectorstruct {
	int *v;
};

typedef struct bst_node_t bst_node_t;
struct  bst_node_t {
	bst_node_t *left;

	bst_node_t *right;

	void *data;
};

typedef struct bst_tree_t bst_tree_t;
struct bst_tree_t {
	bst_node_t  *root;

	size_t data_size;

	int	(*cmp)(void *key1, void *key2, int level, int k);
};

bst_node_t *__bst_node_create(void *data, int k);

bst_tree_t *bst_tree_create(size_t data_size, int (*cmp_f)(void *, void *, int
, int));

void bst_tree_insert(bst_tree_t *bst_tree, void *data, int k);

bst_node_t *__bst_tree_remove(bst_node_t *bst_node, void *data
, size_t data_size, int (*cmp)(void *, void *, int, int));

void bst_tree_remove(bst_tree_t *bst_tree, void *data);

void __bst_tree_free(bst_node_t *bst_node, void (*free_data)(void *));

void bst_tree_free(bst_tree_t *bst_tree, void (*free_data)(void *));

void __bst_tree_print_inorder(bst_node_t *bst_node, int k);

void bst_tree_print_inorder(bst_tree_t *bst_tree, int k);

int bst_cmp_numbers(void *key1, void *key2, int level, int k);

int compare_vectors(void *a, void *b, int k);

void bst_NN(bst_tree_t *tree, int k);

void __bst_tree_find_nearest_node(bst_node_t *bst_node, int *vector, int k
, double *min_dist, bst_node_t **nearest_node);

void __bst_tree_find_all_nearest_nodes(bst_node_t *bst_node, int *vector, int k
, double min_dist, int *num_vectors, int **vectors, int max_vectors);

double __euclidean_distance(int *vector1, int *vector2, int k);

void __bst_RS(bst_node_t *node, int *start, int *end, int k);

void bst_RS(bst_tree_t *bst_tree, int *start, int *end, int k);
