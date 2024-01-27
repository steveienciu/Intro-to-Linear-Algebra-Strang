/* program which finds the product of two matrices */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void print_matrix(double **matrix_sum, int m1, int n1);

int main()
{
	double **matrix_product, **matrix1, **matrix2;
	int m1, m2, n1, n2;

	// output to stdout to let user know what this program will do
	printf("\nThis program will take the sum of two matrices.\n");

	// accept number of rows and columns for the first matrix
	printf("\nPlease enter the number of rows and columns for the first matrix: ");
	scanf("%d %d", &m1, &n1);

	// ensure that input buffer is cleared
	clear_buffer();

	// accept number of rows and columns for the second matrix
	printf("Please enter the number of rows and columns for the second matrix: ");
	scanf("%d %d", &m2, &n2);

	// ensure that input buffer is cleared
	clear_buffer();

	// check to see if dimensions are proper
	if (n1 != m2) {
		fprintf(stderr, "Invalid matrix dimensions\n");
		FAILURE;
	}

	// allocate memory for all matrices
	matrix1 = matrix_mem_alloc(m1, n1);
	if (matrix1 == NULL) {
		mem_failure();
	}
	matrix2 = matrix_mem_alloc(m2, n2);
	if (matrix2 == NULL) {
		free_matrix(matrix1, m1);
		mem_failure();
	}
	matrix_product = matrix_mem_alloc(m1, n2);
	if (matrix_product == NULL) {
		free_matrix(matrix1, m1);
		free_matrix(matrix2, m2);
		mem_failure();
	}

	// accept user input for elements of the first matrix
	user_input_matrix(matrix1, m1, n1);
	printf("\n");
	// clear input buffer
	clear_buffer();
	// accept user input for elements of the second matrix
	user_input_matrix(matrix2, m2, n2);
	printf("\n");
	// clear input buffer
	clear_buffer();

	// perform addition on the two matrices
	matrix_multiplication(matrix_product, matrix1, matrix2, m1, n2, n1);

	// print sum of the two matrices
	printf("The product of the two matrices is:\n");
	print_matrix(matrix_product, m1, n2);

	// deallocate memory for the matrices
	free_matrix(matrix1, m1); free_matrix(matrix2, m2); free_matrix(matrix_product, m1);

	return 0;
}

void print_matrix(double **matrix_sum, int m1, int n1)
{
	for (int i = 0; i < m1; ++i) {
		for (int j = 0; j < n1; ++j) {
			printf("%5.2lf ", matrix_sum[i][j]);
		}
		printf("\n");
	}
}
