#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	int frequency;

	int end_of_word;

	trie_node_t **children;
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t *root;

	int size;

	int data_size;

	int alphabet_size;

	char *alphabet;

	int nr_nodes;
};

trie_node_t *trie_create_node(trie_t *trie);

trie_t *trie_create(int alphabet_size, char *alphabet);

void trie_insert(trie_t *trie, char *key);

void __trie_autocorrect(trie_node_t *node, char *word, char *frame
, int frame_lvl, int len, int cross, int k, int *ok);

void trie_search(trie_t *trie, char *key);

void trie_autocorrect(trie_t *trie, char *word, int k);

void __trie_autocomplete1(trie_node_t *curr, char *frame, int frame_lvl
, char *frame2, int *ok);

void __trie_autocomplete2(trie_node_t *curr, char *frame, int frame_lvl
, char *frame2, int *ok);

void __trie_autocomplete3(trie_node_t *curr, char *frame, int frame_lvl
, char *frame2,  int *freq, int *ok);

void trie_autocomplete(trie_t *trie, char *prefix, int k);

void trie_remove_helper(trie_t *trie, trie_node_t *node, char *key
, size_t index);

void trie_remove(trie_t *trie, char *key);

void __trie_free(trie_node_t *node);

void trie_free(trie_t **p_trie);
