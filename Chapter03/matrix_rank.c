/* program to find the rank (dimension) of a matrix */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

int main() 
{
	double **matrix;
	int m, n, row_index = 0, column_index = 0;
	char ch;

	// output on screen to give user instructions
	printf("This program will find the rank of an inputted matrix.\n");

	printf("Please enter the number of rows: ");
	scanf("%d", &m);
	
	// ensure that input buffer is cleared
	clear_buffer();
	
	printf("Please enter the number of columns: ");
	scanf("%d", &n);

	// ensure that input buffer is cleared
	clear_buffer();

	// make sure not dealing with 1 x 1 matrix or less matrix
	if (m < 2 && n < 2) {
		fprintf(stderr, "Invalid matrix dimensions");
		FAILURE;
	}

	// allocate memory for the matrix
	matrix = matrix_mem_alloc(m, n);

	// return error if allocation failed
	if (matrix == NULL) {
		mem_failure();
	}

	// accept user input for matrix
	user_input_matrix(matrix, m, n);

	// perform Gauss elimination on matrix
	gauss_elimination(matrix, m, n, n);

	// perform Jordan elimination on matrix
	jordan_elimination(matrix, m, n, n, 0);

	// find the first pivot 
	switch_pivot_column(matrix, row_index, &column_index, n);
	++row_index;

	// check to see if dealing with zero vector
	if (column_index == 0 && matrix[0][0] == 0) {
		printf("The rank of the matrix is zero.\n");
		free_matrix(matrix, m);
		return 0;
	}

	if (m >= n) {
		while (row_index != m) {
			int temp = column_index;
			switch_pivot_column(matrix, row_index, &column_index, n);
			if (temp != column_index) {
				++row_index;
			}
			else {
				break;
			}
			// ensure that row_index does not equal m; can happen when n > m (not applicable here but just in case
			if (row_index == m) {
				break;
			}
		}
	}
	else if (m < n) {
		while (column_index != n) {
			int temp = column_index;
			switch_pivot_column(matrix, row_index, &column_index, n);
			if (temp != column_index) {
				++row_index;
			}
			else {
				break;
			}
			// ensure that row_index does not equal m; can happen when n > m
			if (row_index == m) {
				break;
			}
		}
	}

	// output rank of matrix
	printf("\nThe rank of the matrix is %d.\n", row_index);

	// deallocate memory from matrix
	free_matrix(matrix, m);

	return 0;
}
