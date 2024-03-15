/* program that finds the determinants by multiplying the pivots of a square matrix */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

int main()
{
	double **matrix, determinant;
	int m;

	// output instructions for the user to stdout 
	printf("\nThis program will find the determinant of your square matrix.\n");

	printf("Please enter the size of your square matrix: ");
	scanf("%d", &m);

	// clear input buffer
	clear_buffer();

	// ensure proper matrix size
	if (m < 1) {
		fprintf(stderr, "\nImproper matrix size. Terminating program.\n");
		FAILURE;
	}

	// allocate space for the matrix
	matrix = matrix_mem_alloc(m, m);
	if (matrix == NULL) {
		mem_failure();
	}

	// accept user input 
	user_input_matrix(matrix, m, m);

	// clear input buffer
	clear_buffer();

	// take the gauss elimination of the matrix
	gauss_elimination(matrix, m, m, m, 1);

	// find the determinant 
	determinant = find_determinant(matrix, m);

	// print determinant 
	printf("The determinant of the matrix is %0.2lf.\n", determinant);

	// deallocate memory
	free_matrix(matrix, m);

	return 0;
}
