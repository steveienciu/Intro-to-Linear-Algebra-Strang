/* find if the matrix is symmetric */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

int main()
{
	double **matrix;
	int m;
	bool isSymmetric = true;

	printf("\nThis program will determine whether sqaure matrix entered is symmetric.\n");
	printf("Please enter the size of the square matrix: ");
	scanf("%d", &m);

	// check for proper input for size of matrix
	if (m < 2) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}
	
	// allocate memory for the matrix
	matrix = matrix_mem_alloc(m, m);

	// return error message if matrix not allocated properly
	if (matrix == NULL) {
		mem_failure();
	}

	// get user-input for matrix elements
	user_input_matrix(matrix, m, m);

	// decide whether matrix is symmetric
	isSymmetric = is_symmetric(matrix, m, m);
	
	// print result
	if (!isSymmetric) {
		printf("\nMatrix is not symmetric.\n");
	} 
	else {
		printf("\nMatrix is symmetric.\n");
	}

	// free memory allocated to matrix
	free_matrix(matrix, m);

	return 0;
}
