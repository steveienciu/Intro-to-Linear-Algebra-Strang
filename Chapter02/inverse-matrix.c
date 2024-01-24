/* this will find the inverse matrix, if applicable, of a sqaure matrix */
/* matrix inversion only applicable to sqaure matrices */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void print_matrix(double **matrix, int m, int n);

int main()
{
	double **matrix;
	int m;
	bool isInverse;

	printf("\nThis program will take your matrix and inverse it.\n");
	printf("Please enter size of square matrix: ");
	scanf("%d", &m);

	if (m < 1) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for user inputted matrix
	matrix = matrix_mem_alloc(m, m);

	// check to see if allocation was successful
	if (matrix == NULL) {
		mem_failure();
	}

	// accept user input 
	user_input_matrix(matrix, m, m);

	// reallocate matrix to fit in identity matrix
	double **new_matrix = matrix_mem_realloc(matrix, m, m * 2);

	if (new_matrix != NULL) {
		// incase reallocation was not done in place; will make easier to free
		matrix = new_matrix;
	}
	else {
		// only need to free one matrix is allocation failed because they have the same address
		mem_failure();
	}

	// add second half of matrix which is the identity matrix
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < m; ++j) {
			if (i == j) {
				new_matrix[i][j + m] = 1.00;
			}
			else {
				new_matrix[i][j + m] = 0.00;
			}
		}
	}

	// perform Gaussian elimination
	gauss_elimination(new_matrix, m, m * 2, m);

	// does matrix have an inverse
	isInverse = is_inverse(new_matrix, m);

	// do following depending on if matrix has an inverse
	if (!isInverse) {
		printf("\nThe inputted matrix does not have an inverse.\n");
	}
	else {
		// perform Jordan elimination
		jordan_elimination(new_matrix, m, m * 2, m, 1);
		
		// print inverse matrix
		print_matrix(new_matrix, m, m * 2);
	}

	// deallocate memory used for matrix
	free_matrix(new_matrix, m);

	return 0;
}

void print_matrix(double **matrix, int m, int n)
{
	printf("\nThe inverse matrix is:\n");
	for (int i = 0; i < m; ++i) {
		for (int j = m; j < n; ++j) {
			printf("%5.2lf ", matrix[i][j]);
		}
		printf("\n");
	}
}
