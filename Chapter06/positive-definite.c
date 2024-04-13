/* this program determines whether or not a square matrix is positive definite */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

bool is_positive_definite(double **matrix, int m);

int main() 
{
	double **matrix;
	int m;

	//output instructions forr the user to stdout
	printf("This program will determine if the matrix inputted is positive definite.\n");

	printf("Please enter the size of your square matrix: ");
	scanf("%d", &m);

	// clear input buffer
	clear_buffer();

	// ensure proper matrix size
	if (m < 1) {
		fprintf(stderr, "\nImproper matrix size. Terminating program.\n");
		FAILURE;
	}

	// allocate space for matrix
	matrix = matrix_mem_alloc(m, m);
	if (matrix == NULL) {
		mem_failure();
	}

	// accept user input 
	user_input_matrix(matrix, m, m);
	
	// clear input buffer
	clear_buffer();

	// take guass elimination of matrix
	gauss_elimination(matrix, m, m, m, 1);

	// see if matrix positive definite
	bool positive_definite = is_positive_definite(matrix, m);

	if (positive_definite) {
		printf("\nThe matrix inputted is positive definite.\n");
	}
	else {
		printf("\nThe matrix inputted is not positive definite.\n");
	}

	free_matrix(matrix, m);

	return 0;
}

bool is_positive_definite(double **matrix, int m)
{
	// check to see if the pivots are all greater than zero
	for (int i = 0; i < m; ++i) {
		if (matrix[i][i] <= 0) {
			return false;
		}
	}

	return true;
}
