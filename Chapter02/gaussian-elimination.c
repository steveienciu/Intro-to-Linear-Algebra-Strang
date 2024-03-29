/* this will only handle matrix elimination of square matrices */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void print_matrix(double **matrix, int m, int n);

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
	matrix = matrix_mem_alloc(m, m);

	// return error message if matrix not allocated properly 
	if (matrix == NULL) {
		mem_failure();
	}

	// accept user input 
	user_input_matrix(matrix, m, m);

	// perform Gaussian elimination
	gauss_elimination(matrix, m, m, m, 1);

	// output Gauss elimination matrix
	print_matrix(matrix, m, m);

	// deallocate memory used for matrix
	free_matrix(matrix, m);

	return 0;
}

void print_matrix(double **matrix, int m, int n)
{
	printf("\nGauss elimination of the matrix is:\n");
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%5.2lf ", matrix[i][j]);
		}
		printf("\n");
	}
}
