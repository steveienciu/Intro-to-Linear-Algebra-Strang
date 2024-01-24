/* program that finds transpose of a user inputted matrix */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void print_matrix(double **transpose_m, int n, int m);

int main()
{
	double **original_m, **transpose_m;
	int m, n;

	printf("\nThis program will take your matrix and transpose it.\n");
	printf("Please enter size of matrix (row x column form): ");
	scanf("%d %d", &m, &n);

	if (m < 1 || n < 1) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for user inputted matrix
	original_m = matrix_mem_alloc(m, n);
	
	// return error message if matrix not properly allocated
	if (original_m == NULL ) {
		mem_failure();
	}

	// allocate memory for transpose matrix (flip m and n)
	transpose_m = matrix_mem_alloc(n, m);

	// return error message if matrix not properly allocated
	if (transpose_m == NULL) {
		free_matrix(original_m, m);
		mem_failure();
	}

	// accept user input for original matrix
	user_input_matrix(original_m, m, n);

	// take transpose of the original matrix
	transpose_matrix(original_m, transpose_m, m, n);

	// ptint the transposed matrix
	print_matrix(transpose_m, n, m);

	free_matrix(original_m, m);
	free_matrix(transpose_m, n);

	return 0;
}

void print_matrix(double **transpose_m, int n, int m)
{
	printf("\nThe transposed %d x %d matrix is:\n", n, m);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			printf("%5.2lf ", transpose_m[i][j]);
		}
		printf("\n");
	}
}
