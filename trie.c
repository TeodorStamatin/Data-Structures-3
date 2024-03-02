#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

trie_node_t* trie_create_node(trie_t * trie) {

    trie_node_t* node = (trie_node_t*) malloc(sizeof(trie_node_t));
    
    node->frequency = 0;
    node->end_of_word = 0;
    node->n_children = 0;
    node->children = (trie_node_t**) calloc(trie->alphabet_size, sizeof(trie_node_t*));
    trie->nNodes++;

    return node;
}

trie_t* trie_create(int alphabet_size, char* alphabet, void (*free_value_cb)(void*)) {

    trie_t* trie = (trie_t*) malloc(sizeof(trie_t));

    trie->size = 0;
    trie->alphabet_size = alphabet_size;
    trie->alphabet = alphabet;
    trie->free_value_cb = free_value_cb;
    trie->nNodes = 0;
    trie->root = trie_create_node(trie);

    return trie;
}

void trie_insert(trie_t* trie, char* key) {

    trie_node_t* current = trie->root;
    int key_len = strlen(key);

    for (int i = 0; i < key_len; i++) {
        int index = key[i] - 'a';

        if (current->children[index] == NULL) {
            current->n_children++;
            current->children[index] = trie_create_node(trie);
        }

        current = current->children[index];
    }

    current->end_of_word = 1;
	current->frequency++;
}

void __trie_autocorrect(trie_node_t* node, char* word, char* frame, int frame_lvl, int len, int cross, int k, int* ok) {

    if (node == NULL || word == NULL || cross > k || frame_lvl > len)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(node->children[i]) {
            frame[frame_lvl] = 'a' + i;
            if(word[0] == 'a' + i)
				__trie_autocorrect(node->children[i], word + 1, frame, frame_lvl + 1, len, cross, k, ok);
			else
				__trie_autocorrect(node->children[i], word + 1, frame, frame_lvl + 1, len, cross + 1, k, ok);
        }
    }

    frame[frame_lvl] = '\0';

    if(node->end_of_word && frame_lvl == len) {
        printf("%s\n", frame);
        *ok = 1;
    }

}

void trie_autocorrect(trie_t* trie, char* word, int k) {

    trie_node_t* curr = trie->root;
    int len = strlen(word);
    int ok = 0;
    char *frame = malloc(256);
    memset(frame, 0, 256);
    frame[len] = '\0';
	 for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(curr->children[i]) {
            frame[0] = 'a' + i;
            if(word[0] == 'a' + i)
				__trie_autocorrect(curr->children[i], word + 1, frame, 1, len, 0, k, &ok);
			else
				__trie_autocorrect(curr->children[i], word + 1, frame, 1, len, 1, k, &ok);
        }
    }

    if (!ok)
        printf("No words found\n");

    free(frame);
}

void __trie_autocomplete1(trie_node_t *curr, char *frame, int frame_lvl, char *frame2, int *ok) {
    
    if (curr == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(curr->children[i]) {
            frame[frame_lvl] = 'a' + i;
            __trie_autocomplete1(curr->children[i], frame, frame_lvl + 1, frame2, ok);
        }
    }

    frame[frame_lvl] = '\0';

    if(curr->end_of_word) {
        if(frame2[0] == '\0') {
            memcpy(frame2, frame, frame_lvl);
            *ok = 1;
        }
        else if(strcmp(frame, frame2) < 0)
            strcpy(frame2, frame);
    }

}

void __trie_autocomplete2(trie_node_t *curr, char *frame, int frame_lvl, char *frame2, int *ok) {
    
    if (curr == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(curr->children[i]) {
            frame[frame_lvl] = 'a' + i;
            __trie_autocomplete2(curr->children[i], frame, frame_lvl + 1, frame2, ok);
        }
    }

    frame[frame_lvl] = '\0';

    if(curr->end_of_word) {
        if(frame2[0] == '\0') {
            memcpy(frame2, frame, frame_lvl);
            *ok = 1;
        }
        else if(strlen(frame) < strlen(frame2))
            strcpy(frame2, frame);
    }

}

void __trie_autocomplete3(trie_node_t *curr, char *frame, int frame_lvl, char *frame2,  int *freq, int *ok) {
    
    if (curr == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(curr->children[i]) {
            frame[frame_lvl] = 'a' + i;
            __trie_autocomplete3(curr->children[i], frame, frame_lvl + 1, frame2, freq, ok);
        }
    }

    frame[frame_lvl] = '\0';
    
    if(curr->end_of_word) {
        if(frame2[0] == '\0') {
            strcpy(frame2, frame);
            *freq = curr->frequency;
            *ok = 1;
        }
        else if(*freq < curr->frequency) {
            strcpy(frame2, frame);
            *freq = curr->frequency;
        }
    }

}

void trie_autocomplete(trie_t* trie, char* prefix, int k) {

    trie_node_t* curr = trie->root;
    int ok = 0, freq = 0;
    int len = strlen(prefix);
    char *frame = malloc(256);
    char *frame2 = malloc(256);
    memset(frame, 0, 256);
    memset(frame2, 0, 256);
    frame2[0] = '\0';
    frame[len] = '\0';
    for(int i = 0; i < len; i++) {
        if(curr->children[prefix[i] - 'a'] == NULL) {
            printf("No words found\n");
            return;
        }
        curr = curr->children[prefix[i] - 'a'];
        frame[i] = prefix[i];
    }
    switch (k)
    {
    case 1:
        __trie_autocomplete1(curr, frame, len, frame2, &ok);
        if(!ok)
            printf("No words found\n");
        else
            printf("%s\n", frame2);
        break;
    case 2:
        __trie_autocomplete2(curr, frame, len, frame2, &ok);
        if(!ok)
            printf("No words found\n");
        else
            printf("%s\n", frame2);
        break;
    case 3:
        __trie_autocomplete3(curr, frame, len, frame2, &freq, &ok);
        if(!ok)
            printf("No words found\n");
        else
            printf("%s\n", frame2);
        break; 
    case 0:
        __trie_autocomplete1(curr, frame, len, frame2, &ok);
        if(!ok)
            printf("No words found\n");
        else
            printf("%s\n", frame2);
        ok = 0;
        frame2[0] = '\0';
        __trie_autocomplete2(curr, frame, len, frame2, &ok);
        if(!ok)
            printf("No words found\n");
        else
            printf("%s\n", frame2);
        ok = 0;
        frame2[0] = '\0';
        __trie_autocomplete3(curr, frame, len, frame2, &freq, &ok);
        if(!ok)
            printf("No words found\n");
        else
            printf("%s\n", frame2);
        break; 
    default:
        break;
    }

    free(frame);
    free(frame2);
}

int remove_word(trie_t* trie, trie_node_t *curr, char* key, int k)
{
    int n;
    if (key[k]) {
        if (!curr->children[key[k] - 'a'])
            return -1;
        n = remove_word(trie, curr->children[key[k] - 'a'], key, k+1);
    }
    if (!key[k] && !curr->end_of_word)
        return -1;
    if (!key[k]) {
        curr->end_of_word = 0;
        return curr->n_children;
    }
    if (!n) {
        free(curr->children[key[k] - 'a']->children);
        free(curr->children[key[k] - 'a']);
        curr->children[key[k] - 'a'] = NULL;
        curr->n_children--;
        trie->nNodes--;
    }
    return curr->n_children;
}

void trie_remove(trie_t* trie, char* key) {
    int k = 0;
    trie_node_t *curr = trie->root;
    int n = remove_word(trie, curr, key, k);
}

void __trie_free(trie_node_t* node, void (*free_value_cb)(void*)) {

    for (int i = 0; i < 26; i++) {
        if(node->children[i] == NULL) {
            continue;
        }
        __trie_free(node->children[i], free_value_cb);
    }

    free(node->children);
    node->children = NULL;

    free(node);
    node = NULL;
}

void trie_free(trie_t** pTrie) {
    trie_t* trie = *pTrie;
    __trie_free(trie->root, trie->free_value_cb);
    free(trie);
    *pTrie = NULL;
}
