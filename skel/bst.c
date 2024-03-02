#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "bst.h"

bst_node_t *__bst_node_create(void *data, int k)
{
	bst_node_t *bst_node;

	bst_node = malloc(sizeof(*bst_node));

	if (!bst_node)
		return NULL;

	bst_node->left = NULL;
	bst_node->right = NULL;

	bst_node->data = malloc(sizeof(vectorstruct));

	vectorstruct *tmp = (vectorstruct *)bst_node->data;
	tmp->v = malloc(k * sizeof(int));
	for (int i = 0; i < k; i++)
		tmp->v[i] = ((vectorstruct *)data)->v[i];

	return bst_node;
}

bst_tree_t *bst_tree_create(size_t data_size, int (*cmp_f)(void *, void *
, int level, int k))
{
	bst_tree_t *bst_tree;

	bst_tree = malloc(sizeof(*bst_tree));
	if (!bst_tree)
		return NULL;

	bst_tree->root  = NULL;
	bst_tree->data_size = data_size;
	bst_tree->cmp   = cmp_f;

	return bst_tree;
}

void bst_tree_insert(bst_tree_t *bst_tree, void *data, int k)
{
	int rc;
	bst_node_t *root = bst_tree->root;
	bst_node_t *parent = NULL;
	bst_node_t *node = __bst_node_create(data, k);

	int level = 0;
	if (!root) {
		bst_tree->root = node;
		return;
	}

	while (root) {
		parent = root;
		rc = bst_tree->cmp(data, root->data, level++, k);

		if (rc < 0)
			root = root->left;
		else
			root = root->right;
	}

	if (bst_tree->cmp(data, parent->data, --level, k) < 0)
		parent->left = node;
	else
		parent->right = node;
}

void __bst_tree_free(bst_node_t *bst_node, void (*free_data)(void *))
{
	if (!bst_node)
		return;

	__bst_tree_free(bst_node->left, free_data);
	__bst_tree_free(bst_node->right, free_data);

	vectorstruct *vector = (vectorstruct *)bst_node->data;
	free(vector->v);
	free(bst_node->data);
	free(bst_node);
}

void bst_tree_free(bst_tree_t *bst_tree, void (*free_data)(void *))
{
	__bst_tree_free(bst_tree->root, free_data);
	bst_tree->root = NULL;
	free(bst_tree);
}

void __bst_tree_print_inorder(bst_node_t *bst_node, int k)
{
	if (!bst_node)
		return;

	__bst_tree_print_inorder(bst_node->left, k);
	vectorstruct *vector = (vectorstruct *)bst_node->data;

	for (int i = 0; i < k; i++)
		printf("%d ", vector->v[i]);
	printf("\n");

	__bst_tree_print_inorder(bst_node->right, k);
}

void bst_tree_print_inorder(bst_tree_t *bst_tree, int k)
{
	__bst_tree_print_inorder(bst_tree->root, k);
}

int bst_cmp_numbers(void *key1, void *key2, int level, int k)
{
	vectorstruct *vector1 = (vectorstruct *)key1;
	vectorstruct *vector2 = (vectorstruct *)key2;

	if (vector1->v[level % k] < vector2->v[level % k])
		return -1;
	else if (vector1->v[level % k] > vector2->v[level % k])
		return 1;
	else
		return 0;
}

int compare_vectors(void *a, void *b, int k)
{
	int *vector1 = *(int **)a;
	int *vector2 = *(int **)b;

	for (int i = 0; i < k; i++)
		if (vector1[i] < vector2[i])
			return -1;
		else if (vector1[i] > vector2[i])
			return 1;

	return 0;
}

void bst_NN(bst_tree_t *tree, int k)
{
	int *vector = malloc(k * sizeof(int));
	for (int i = 0; i < k; i++)
		scanf("%d", &vector[i]);

	double min_dist = -1;
	bst_node_t *nearest_node = NULL;

	__bst_tree_find_nearest_node(tree->root, vector, k, &min_dist
	, &nearest_node);

	if (nearest_node) {
		int max_vectors = 100;
		int num_vectors = 0;
		int **vectors = malloc(max_vectors * sizeof(int *));

		__bst_tree_find_all_nearest_nodes(tree->root, vector, k, min_dist
		, &num_vectors, vectors, max_vectors);

		qsort(vectors, num_vectors, sizeof(int *), compare_vectors);

		for (int i = 0; i < num_vectors; i++) {
			for (int j = 0; j < k; j++)
				printf("%d ", vectors[i][j]);
			printf("\n");
		}

		for (int i = 0; i < num_vectors; i++)
			free(vectors[i]);
		free(vectors);
	}

	free(vector);
}

void __bst_tree_find_nearest_node(bst_node_t *bst_node, int *vector, int k
, double *min_dist, bst_node_t **nearest_node)
{
	if (!bst_node)
		return;

	__bst_tree_find_nearest_node(bst_node->left, vector, k, min_dist
	, nearest_node);

	vectorstruct *current_vector = (vectorstruct *)bst_node->data;
	float dist = __euclidean_distance(current_vector->v, vector, k);

	if (*min_dist == -1 || dist < *min_dist) {
		*min_dist = dist;
		*nearest_node = bst_node;
	}

	__bst_tree_find_nearest_node(bst_node->right, vector, k, min_dist
	, nearest_node);
}

void __bst_tree_find_all_nearest_nodes(bst_node_t *bst_node, int *vector
, int k, double min_dist, int *num_vectors, int **vectors, int max_vectors)
{
	if (!bst_node)
		return;

	__bst_tree_find_all_nearest_nodes(bst_node->left, vector, k, min_dist
	, num_vectors, vectors, max_vectors);

	vectorstruct *current_vector = (vectorstruct *)bst_node->data;
	float dist = __euclidean_distance(current_vector->v, vector, k);

	if (dist == min_dist) {
		vectors[*num_vectors] = malloc(k * sizeof(int));
		memcpy(vectors[*num_vectors], current_vector->v, k * sizeof(int));
		(*num_vectors)++;
	}

	__bst_tree_find_all_nearest_nodes(bst_node->right, vector, k, min_dist
	, num_vectors, vectors, max_vectors);
}

double __euclidean_distance(int *vector1, int *vector2, int k)
{
	double dist_sq = 0;
	for (int i = 0; i < k; i++) {
		long diff = vector1[i] - vector2[i];
		dist_sq += diff * diff;
	}
	dist_sq = sqrt(dist_sq);
	return dist_sq;
}

void __bst_RS(bst_node_t *bst_node, int *start, int *end, int k)
{
	if (!bst_node)
		return;

	__bst_RS(bst_node->left, start, end, k);
	__bst_RS(bst_node->right, start, end, k);

	vectorstruct *vector = (vectorstruct *)bst_node->data;

	int i = 0;
	for (i = 0; i < k; i++) {
		if (start[i] > end[i]) {
			long aux = start[i];
			start[i] = end[i];
			end[i] = aux;
		}
		if (vector->v[i] < start[i] || vector->v[i] > end[i])
			break;
	}

	if (i == k) {
		for (i = 0; i < k; i++)
			printf("%d ", vector->v[i]);
		printf("\n");
	}
}

void bst_RS(bst_tree_t *bst_tree, int *start, int *end, int k)
{
	__bst_RS(bst_tree->root, start, end, k);
}
