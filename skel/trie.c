#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));

	node->frequency = 0;
	node->end_of_word = 0;
	node->n_children = 0;
	node->children = (trie_node_t **)calloc(trie->alphabet_size
	, sizeof(trie_node_t *));
	trie->nr_nodes++;

	return node;
}

trie_t *trie_create(int alphabet_size, char *alphabet)
{
	trie_t *trie = (trie_t *)malloc(sizeof(trie_t));

	trie->size = 0;
	trie->alphabet_size = alphabet_size;
	trie->alphabet = alphabet;
	trie->nr_nodes = 0;
	trie->root = trie_create_node(trie);

	return trie;
}

void trie_insert(trie_t *trie, char *key)
{
	trie_node_t *current = trie->root;
	int key_len = strlen(key);

	for (int i = 0; i < key_len; i++) {
		int index = key[i] - 'a';

		if (!current->children[index]) {
			current->n_children++;
			current->children[index] = trie_create_node(trie);
		}

		current = current->children[index];
	}

	current->end_of_word = 1;
	current->frequency++;
}

void __trie_autocorrect(trie_node_t *node, char *word, char *frame
, int frame_lvl, int len, int cross, int k, int *ok)
{
	if (!node || !word || cross > k || frame_lvl > len)
		return;

	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i]) {
			frame[frame_lvl] = 'a' + i;
			if (word[0] == 'a' + i)
				__trie_autocorrect(node->children[i], word + 1, frame
				, frame_lvl + 1, len, cross, k, ok);
			else
				__trie_autocorrect(node->children[i], word + 1, frame
				, frame_lvl + 1, len, cross + 1, k, ok);
		}
	}

	frame[frame_lvl] = '\0';

	if (node->end_of_word && frame_lvl == len) {
		printf("%s\n", frame);
		*ok = 1;
	}
}

void trie_autocorrect(trie_t *trie, char *word, int k)
{
	trie_node_t *curr = trie->root;

	int len = strlen(word);
	int ok = 0;
	char *frame = malloc(256);
	memset(frame, 0, 256);
	frame[len] = '\0';

	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (curr->children[i]) {
			frame[0] = 'a' + i;
			if (word[0] == 'a' + i)
				__trie_autocorrect(curr->children[i], word + 1, frame, 1
				, len, 0, k, &ok);
			else
				__trie_autocorrect(curr->children[i], word + 1, frame, 1
				, len, 1, k, &ok);
		}
	}

	if (!ok)
		printf("No words found\n");

	free(frame);
}

void __trie_autocomplete1(trie_node_t *curr, char *frame, int frame_lvl
, char *frame2, int *ok)
{
	if (!curr)
		return;

	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (curr->children[i]) {
			frame[frame_lvl] = 'a' + i;
			__trie_autocomplete1(curr->children[i], frame, frame_lvl + 1
			, frame2, ok);
		}
	}

	frame[frame_lvl] = '\0';

	if (curr->end_of_word) {
		if (frame2[0] == '\0') {
			memcpy(frame2, frame, frame_lvl);
			*ok = 1;
		} else {
			if (strcmp(frame, frame2) < 0)
				strcpy(frame2, frame);
		}
	}
}

void __trie_autocomplete2(trie_node_t *curr, char *frame, int frame_lvl
, char *frame2, int *ok)
{
	if (!curr)
		return;

	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (curr->children[i]) {
			frame[frame_lvl] = 'a' + i;
			__trie_autocomplete2(curr->children[i], frame, frame_lvl + 1
			, frame2, ok);
		}
	}

	frame[frame_lvl] = '\0';

	if (curr->end_of_word) {
		if (frame2[0] == '\0') {
			memcpy(frame2, frame, frame_lvl);
			*ok = 1;
		} else {
			if (strlen(frame) < strlen(frame2))
				strcpy(frame2, frame);
		}
	}
}

void __trie_autocomplete3(trie_node_t *curr, char *frame, int frame_lvl
, char *frame2,  int *freq, int *ok)
{
	if (!curr)
		return;

	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (curr->children[i]) {
			frame[frame_lvl] = 'a' + i;
			__trie_autocomplete3(curr->children[i], frame, frame_lvl + 1
			, frame2, freq, ok);
		}
	}

	frame[frame_lvl] = '\0';
	if (curr->end_of_word) {
		if (frame2[0] == '\0') {
			strcpy(frame2, frame);
			*freq = curr->frequency;
			*ok = 1;
		} else if (*freq < curr->frequency) {
			strcpy(frame2, frame);
			*freq = curr->frequency;
		} else if (*freq == curr->frequency)
			if (strcmp(frame, frame2) < 0)
				strcpy(frame2, frame);
	}
}

void trie_search(trie_t *trie, char *key)
{
	trie_node_t *current = trie->root;
	int key_len = strlen(key);

	for (int i = 0; i < key_len; i++) {
		int index = key[i] - 'a';

		if (!current->children[index])
			return;

		current = current->children[index];
	}

	if (!current && current->end_of_word)
		printf("%d\n", current->frequency);
}

void trie_autocomplete(trie_t *trie, char *prefix, int k)
{
	trie_node_t *curr = trie->root;

	int ok = 0, freq = 0;
	int len = strlen(prefix);

	char *frame = malloc(256);
	char *frame2 = malloc(256);
	memset(frame, 0, 256);
	memset(frame2, 0, 256);

	frame2[0] = '\0';
	frame[len] = '\0';

	for (int i = 0; i < len; i++) {
		if (curr->children[prefix[i] - 'a'] == NULL) {
			if (k == 0) {
				printf("No words found\n");
				printf("No words found\n");
				printf("No words found\n");
			} else {
				printf("No words found\n");
			}

			free(frame);
			free(frame2);
			return;
		}
		curr = curr->children[prefix[i] - 'a'];
		frame[i] = prefix[i];
	}
	switch (k) {
	case 1:
		__trie_autocomplete1(curr, frame, len, frame2, &ok);
		if (!ok)
			printf("No words found\n");
		else
			printf("%s\n", frame2);
		break;
	case 2:
		__trie_autocomplete2(curr, frame, len, frame2, &ok);
		if (!ok)
			printf("No words found\n");
		else
			printf("%s\n", frame2);
		break;
	case 3:
		__trie_autocomplete3(curr, frame, len, frame2, &freq, &ok);
		if (!ok)
			printf("No words found\n");
		else
			printf("%s\n", frame2);
		break;
	case 0:
		__trie_autocomplete1(curr, frame, len, frame2, &ok);
		if (!ok)
			printf("No words found1\n");
		else
			printf("%s\n", frame2);
		ok = 0;
		frame2[0] = '\0';
		__trie_autocomplete2(curr, frame, len, frame2, &ok);
		if (!ok)
			printf("No words found2\n");
		else
			printf("%s\n", frame2);
		ok = 0;
		frame2[0] = '\0';
		__trie_autocomplete3(curr, frame, len, frame2, &freq, &ok);
		if (!ok)
			printf("No words found3\n");
		else
			printf("%s\n", frame2);
		break;
	default:
		break;
	}

	free(frame);
	free(frame2);
}

void trie_remove_helper(trie_t *trie, trie_node_t *node, char *key
, size_t index)
{
	if (index == strlen(key)) {
		if (node->end_of_word == 1) {
			node->end_of_word = 0;
			trie->size--;
			node->frequency--;
			if (node->n_children == 0) {
				free(node->children);
				node->children = NULL;
			}
		}
		return;
	}
	if (!node->children[key[index] - 'a'])
		return;

	trie_node_t *curr = node->children[key[index] - 'a'];
	trie_remove_helper(trie, node->children[key[index] - 'a'], key, index + 1);
	if (curr->n_children == 0 && curr->end_of_word == 0) {
		free(node->children[key[index] - 'a']->children);
		node->children[key[index] - 'a']->children = NULL;

		free(node->children[key[index] - 'a']);
		node->children[key[index] - 'a'] = NULL;
		node->n_children--;
		trie->nr_nodes--;

		node->frequency--;
	}
}

void trie_remove(trie_t *trie, char *key)
{
	trie_node_t *node = trie->root;
	trie_remove_helper(trie, node, key, 0);
}

void __trie_free(trie_node_t *node)
{
	for (int i = 0; i < 26; i++) {
		if (!node->children[i])
			continue;

		__trie_free(node->children[i]);
	}

	free(node->children);
	node->children = NULL;

	free(node);
	node = NULL;
}

void trie_free(trie_t **p_trie)
{
	trie_t *trie = *p_trie;
	__trie_free(trie->root);
	free(trie);
	*p_trie = NULL;
}
