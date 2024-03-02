#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "trie.h"

#define MAX_LEN 256
#define MAX_COM 16

int main(void)
{
	char alphabet[] = ALPHABET;

	trie_t *trie = trie_create(ALPHABET_SIZE, alphabet);

	char command[MAX_COM];
	while (1) {
		scanf("%s", command);
		if (strncmp(command, "INSERT", 6) == 0) {
			char word[MAX_LEN];
			scanf("%s", word);
			trie_insert(trie, word);
		}
		if (strncmp(command, "LOAD", 4) == 0) {
			char file[MAX_LEN];
			scanf("%s", file);
			FILE *f = fopen(file, "r");
			if (!f) {
				printf("Error opening file\n");
				continue;
			}

			char word[MAX_LEN];
			while (fscanf(f, "%s", word) != EOF)
				trie_insert(trie, word);
			fclose(f);
		}
		if (strncmp(command, "AUTOCORRECT", 11) == 0) {
			char word[MAX_LEN];
			int k;
			scanf("%s %d", word, &k);
			trie_autocorrect(trie, word, k);
		}
		if (strncmp(command, "AUTOCOMPLETE", 12) == 0) {
			char word[MAX_LEN];
			int k;
			scanf("%s %d", word, &k);
			trie_autocomplete(trie, word, k);
		}
		if (strncmp(command, "REMOVE", 6) == 0) {
			char word[MAX_LEN];
			scanf("%s", word);
			trie_remove(trie, word);
		}
		if (strncmp(command, "EXIT", 4) == 0) {
			trie_free(&trie);
			break;
		}
		if (strncmp(command, "search", 6) == 0) {
			char word[MAX_LEN];
			scanf("%s", word);
			trie_search(trie, word);
		}
	}
	return 0;
}
