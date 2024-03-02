#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
    int frequency;

    /* 1 if current node marks the end of a word, 0 otherwise */
    int end_of_word;

    trie_node_t** children;
    int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
    trie_node_t* root;
    
    /* Number of keys */
    int size;

    /* Generic Data Structure */
    int data_size;

    /* Trie-Specific, alphabet properties */
    int alphabet_size;
    char* alphabet;

    /* Callback to free value associated with key, should be called when freeing */
    void (*free_value_cb)(void*);

    /* Optional - number of nodes, useful to test correctness */
    int nNodes;
};

trie_node_t* trie_create_node(trie_t * trie);

trie_t* trie_create(int alphabet_size, char* alphabet, void (*free_value_cb)(void*));

void trie_insert(trie_t* trie, char* key);

void __trie_autocorrect(trie_node_t* node, char* word, char* frame, int frame_lvl, int len, int cross, int k, int* ok);

void trie_autocorrect(trie_t* trie, char* word, int k);

void __trie_autocomplete1(trie_node_t *curr, char *frame, int frame_lvl, char *frame2, int *ok);

void __trie_autocomplete2(trie_node_t *curr, char *frame, int frame_lvl, char* frame2, int *ok);

void __trie_autocomplete3(trie_node_t *curr, char *frame, int frame_lvl, char *frame2,  int *freq, int *ok);

void trie_autocomplete(trie_t* trie, char* prefix, int k);

int remove_word(trie_t* trie, trie_node_t *curr, char* key, int k);

void trie_remove(trie_t* trie, char* key);

void __trie_free(trie_node_t* node, void (*free_value_cb)(void*));

void trie_free(trie_t** pTrie);