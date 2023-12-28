/* this will only handle matrix elimination of square matrices */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FAILURE exit(1)

void mem_failure(void);
void mem_alloc(double ***matrix, int m);
void user_input(double **matrix, int m);
void gaussian_elimination(double **matrix, int m);
void print_matrix(double **matrix, int m);
void free_matrix(double **matrix, int m);

int main()
{
	double **matrix;
	int m;

	printf("\nThis program will take your matrix and perform Guassian elimination on it.\n");
	printf("Please enter size of square matrix: ");
	scanf("%d", &m);

	if (m < 1) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for user inputted matrix
	mem_alloc(&matrix, m);

	// accept user input 
	user_input(matrix, m);

	// perform Gaussian elimination
	gaussian_elimination(matrix, m);

	// output Gauss elimination matrix
	print_matrix(matrix, m);

	// deallocate memory used for matrix
	free_matrix(matrix, m);

	return 0;
}

void mem_failure(void)
{
	fprintf(stderr, "\nMatrix was not properly allocated.\n");
	FAILURE;
}

void mem_alloc(double ***matrix, int m)
{
	int counter = 0;
	bool isAllocated = true;

	*matrix = malloc(m * sizeof(double *));
	if (*matrix == NULL)
	{
		mem_failure();
	}
	for (int i = 0; i < m; ++i) {
		(*matrix)[i] = malloc(m * sizeof(double));
		if ((*matrix)[i] == NULL) {
			isAllocated = false;
			break;
		}
		++counter;
	}

	if (!isAllocated) {
		free_matrix(*matrix, counter);
		mem_failure();
	}
}

void user_input(double **matrix, int m)
{
	printf("Pleae enter elements of %d x %d matrix:\n", m, m);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < m; ++j) {
			scanf("%lf", &matrix[i][j]);
		}
	}
}

void gaussian_elimination(double **matrix, int m)
{
	int count = 0, i = 0, j = 0, s = 0;
	double multiplier = 0;

	for (int k = 0; k < m; ++k) {
		count += k;
	}

	for (int k = 0; k < count; ++k) {
		multiplier = -(matrix[s + 1][j] / matrix[i][i]);
		for (int l = 0; l < m; ++l) {
			matrix[s + 1][l] += multiplier * matrix[i][l];
		}
		++s;
		if ((s + 1) == m) {
			++j;
			++i;
			s = i;
		}
	}
}

void print_matrix(double **matrix, int m)
{
	printf("\nGauss elimination of the matrix is:\n");
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < m; ++j) {
			printf("%5.2lf ", matrix[i][j]);
		}
		printf("\n");
	}
}

void free_matrix(double **matrix, int m)
{
	for (int i = 0; i < m; ++i) {
		free(matrix[i]);
	}
	free(matrix);
}
