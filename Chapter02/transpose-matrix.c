/* program that finds transpose of a user inputted matrix */

#include <stdio.h>
#include <stdlib.h>

#define FAILURE exit(1)

void mem_failure(void);
void mem_alloc(int ***matrix, int m, int n);
void user_input(int **matrix, int m, int n);
void transpose(int **original_m, int **tranpose_m, int m, int n);
void free_matrix(int **matrix, int m, int n);

int main()
{
	int **original_m, **transpose_m, m, n;

	printf("\nThis program will take your matrix and transpose it.\n");
	printf("Please enter size of matrix (row x column form): ");
	scanf("%d %d", &m, &n);

	if (m < 1 || n < 1) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for user inputted matrix
	mem_alloc(&original_m, m, n);
	// allocate memory for transpose matrix (flip m and n)
	mem_alloc(&transpose_m, n, m); // need to properly free original matrix if allocation for transpose matrix fails

	// accept user input for oringinal matrix
	user_input(original_m, m, n);

	// take transpose of the original matrix
	transpose(original_m, transpose_m, m, n);

	// free memory allocated to matrices
	free_matrix(original_m, m, n);
	free_matrix(transpose_m, n, m);

	return 0;
}

void mem_failure(void)
{
	fprintf(stderr, "\nMatrix was not properly allocated.\n");
	FAILURE;
}

void mem_alloc(int ***matrix, int m, int n)
{
	*matrix = malloc(m * sizeof(int *));
	if (*matrix == NULL)
	{
		mem_failure();
	}
	for (int i = 0; i < m; ++i) {
		(*matrix)[i] = malloc(n * sizeof(int));
		if ((*matrix)[i] == NULL) {
			mem_failure();
		}
	}
}

void user_input(int **matrix, int m, int n)
{
	printf("Pleae enter elements of %d x %d matrix:\n", m, n);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%d", &matrix[i][j]);
		}
	}
}

void transpose(int **original_m, int **transpose_m, int m, int n)
{
	printf("\nThe %d x %d transpose matrix is:\n", n, m);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			transpose_m[j][i] = original_m[i][j];
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			printf("%d ", transpose_m[i][j]);
		}
		printf("\n");
	}
}

void free_matrix(int **matrix, int m, int n)
{
	for (int i = 0; i < m; ++i) {
		free(matrix[i]);
	}
	free(matrix);
}
