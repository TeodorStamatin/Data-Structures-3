#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "bst.h"

#define MAX_LEN 256
#define MAX_COM 16

int main(void)
{
	int k;
	bst_tree_t *bst_tree = bst_tree_create(sizeof(vectorstruct)
	, bst_cmp_numbers);

	char command[MAX_COM];
	while (1) {
		scanf("%s", command);
		if (strncmp(command, "LOAD", 4) == 0) {
			char file[MAX_LEN];
			scanf("%s", file);
			FILE *f = fopen(file, "r");
			if (!f) {
				printf("Invalid file\n");
				return -1;
			}
			int n;
			fscanf(f, "%d %d", &n, &k);
			for (int i = 0; i < n; i++) {
				vectorstruct *vector = malloc(sizeof(vectorstruct));
				vector->v = malloc(k * sizeof(int));
				int var;
				for (int j = 0; j < k; j++) {
					fscanf(f, "%d", &var);
					vector->v[j] = var;
				}
				bst_tree_insert(bst_tree, (void *)vector, k);
				free(vector->v);
				vector->v = NULL;
				free(vector);
				vector = NULL;
			}
			fclose(f);
		}
		if (strncmp(command, "NN", 2) == 0)
			bst_NN(bst_tree, k);
		if (strncmp(command, "EXIT", 4) == 0) {
			bst_tree_free(bst_tree, free);
			break;
		}
		if (strncmp(command, "RS", 2) == 0) {
			int *start = malloc(k * sizeof(int));
			int *end = malloc(k * sizeof(int));
			for (int i = 0; i < k; i++)
				scanf("%d %d", &start[i], &end[i]);

			bst_RS(bst_tree, start, end, k);

			free(start);
			free(end);
		}
	}
	return 0;
}
