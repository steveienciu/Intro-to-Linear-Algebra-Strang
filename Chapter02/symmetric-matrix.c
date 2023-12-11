/* find the transpose of the matrix */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FAILURE exit(1)

void mem_failure(void);

int main()
{
	int **matrix, m;
	bool sym = true;

	printf("\nThis program will determine whether sqaure matrix entered is symmetric.\n");
	printf("Please enter the size of the square matrix: ");
	scanf("%d", &m);

	// check for proper input for size of matrix
	if (m < 2) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for matrix
	matrix = malloc(m * sizeof(int *));
	if (matrix == NULL) {
		mem_failure();
	}
	else {
		for (int i = 0; i < m; ++i) {
			matrix[i] = malloc(m * sizeof(int));
			if (matrix[i] == NULL) {
				mem_failure();
			}
		}
	}

	// obtain matrix elemenets from user
	printf("Please enter elements of matrix:\n");
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < m; ++j) {
			scanf("%d", &matrix[i][j]);
		}
	}

	// decide whether matrix is symmetric
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < m; ++j) {
			if (matrix[i][j] != matrix[j][i]) {
				sym = false;
				break;
			}
		}
		if (!sym) {
			break;
		}
	}
	
	// print result
	if (!sym) {
		printf("\nMatrix is not symmetric.\n");
	} 
	else {
		printf("\nMatrix is symmetric.\n");
	}

	// free memory allocated to matrix
	for (int i = 0; i < m; ++i) {
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}

void mem_failure(void)
{
	fprintf(stderr, "\nMatrix was not properly allocated.\n");
	FAILURE;
}
