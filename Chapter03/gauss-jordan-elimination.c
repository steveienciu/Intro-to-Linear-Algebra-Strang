/* this gauss-jordan program also handles matrix elimination of non sqaure matrices */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void print_matrix(double **matrix, int m, int n);

int main()
{
	double **matrix;
	int m, n;
	
	// output on stdout to give user instructions
	printf("\nThis program will take your matrix and perform Guass-Jordan elimination on it.\n");

	printf("Please enter the number of rows: ");
	scanf("%d", &m);

	// clear input buffer
	clear_buffer();

	printf("Please enter the number of columns: ");
	scanf("%d", &n);

	if (m < 1 && n < 1) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for user inputted matrix
	matrix = matrix_mem_alloc(m, n);

	// return failure message if allocation failed
	if (matrix == NULL) {
		mem_failure();
	}

	// accept user input
	user_input_matrix(matrix, m, n);

	// perform Gaussian elimination
	gauss_elimination(matrix, m, n, n, 1);

	// perform Jordan elimination
	jordan_elimination(matrix, m, n, n, 1);

	// output Gauss-Jordan elimination matrix
	print_matrix(matrix, m, n);

	// deallocate memory used for matrix
	free_matrix(matrix, m);

	return 0;
}

void print_matrix(double **matrix, int m, int n)
{
	printf("\nGauss-Jordan elimination of the matrix is:\n");
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%5.2lf ", matrix[i][j]);
		}
		printf("\n");
	}
}
