/* program which finds row basis or columns basis of a given matrix */
/* could probably make it more interactive but didn't want to */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "linear_algebra.h"

void column_basis(double **matrix_original, double **matrix_copy, int m, int n);
void row_basis(double **matrix_original, double **matrix_copy, int m, int n);

int main()
{
	double **matrix_original, **matrix_copy;
	int m, n;
	char picker;

	// output on stdout what this program does
	printf("\nThis program will find the column or row basis of a matrix\n");
	
	// get number of rows of matrix
	printf("Please enter the number of rows: ");
	scanf("%d", &m);

	// clear input buffer
	clear_buffer();

	// get number of columns of matrix
	printf("Please enter the number of columns: ");
	scanf("%d", &n);

	// clear input buffer
	clear_buffer();

	// make sure not dealing with 1 x 1 matrix or less
	if (m < 2 && n < 2) {
		fprintf(stderr, "Invalid matrix dimensions.\n");
		FAILURE;
	}

	// allocate memory for the original matrix
	matrix_original = matrix_mem_alloc(m, n + 1);

	// return error if allocation failes
	if (matrix_original == NULL) {
		mem_failure();
	}

	// allocate memory for the copy matrix
	matrix_copy = matrix_mem_alloc(m, n + 1);
	
	// return error if allocation failed
	if (matrix_copy == NULL) {
		free_matrix(matrix_original, m);
		mem_failure();
	}

	// accept user input for original matrix
	user_input_matrix(matrix_original, m, n);
	// index the rows to make finding row basis easier
	for (int i = 0; i < m; ++i) {
		matrix_original[i][n] = i;
	}

	// copy the values of the original matrix into the copy matrix
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n + 1; ++j) {
			matrix_copy[i][j] = matrix_original[i][j];
		}
	}
	
	// clear input buffer
	clear_buffer();

	// find gauss-jordan of matrix
	gauss_elimination(matrix_copy, m, n + 1, n, 0);
	jordan_elimination(matrix_copy, m, n + 1, n, 0);

	// user decides whether they want column basis or row basis 
	printf("\nEnter 'c' to obtain the column basis or 'r' to obtain the row basis: ");
	scanf(" %c", &picker);
	picker = tolower(picker);

	// logic for finding row basis or column basus 
	switch (picker) {
		// find column basis
		case 'c':
			column_basis(matrix_original, matrix_copy, m, n);
			break;
		// find row basis
		case 'r':
			row_basis(matrix_original, matrix_copy, m, n);
			break;
		default:
			printf("\nImproper character. Exiting program.\n");
			break;
	}

	free_matrix(matrix_original, m); free_matrix(matrix_copy, m);
	
	return 0;
}

void column_basis(double **matrix_original, double **matrix_copy, int m, int n)
{
	int *column_pivots, column_index = 0, row_index = 0;

	// allocate space for max number of column pivots
	column_pivots = malloc(n * sizeof(int));
	// ensure memory was allocated properly 
	if (column_pivots == NULL) {
		fprintf(stderr, "Memory was not allocated properly in column_basis.\n");
		return;
	}
	// initialize elements to zero
	for (int i = 0; i < n; ++i) {
		column_pivots[i] = 0;
	}

	// find first pivot
	switch_pivot_column(matrix_copy, row_index, &column_index, n);
	column_pivots[row_index] = column_index;
	++row_index;

	// check to see if dealing with zero matrix
	if (column_index == 0 && matrix_copy[0][0] == 0) {
		printf("\nThe basis vector of this matrix is zero.\n");
		return;
	}
	
	// find all column pivots that are non zero
	while (row_index != m && column_index != n - 1) {
		int temp = column_index;
		switch_pivot_column(matrix_copy, row_index, &column_index, n);
		if (temp != column_index) {
			column_pivots[row_index] = column_index;
			++row_index;
		}
		else {
			break;
		}
	}

	// print the column basis vectors
	printf("\nThe column basis vectors are:\n");
	for (int i = 0; i < row_index; ++i) {
		printf("(");
		column_index = column_pivots[i];
		for (int k = 0; k < m; ++k) {
			printf("%0.2lf", matrix_original[k][column_index]);
			if (k != m - 1) {
				printf(", ");
			}
		}
		printf(")\n");
	}
	
	free(column_pivots);
}

void row_basis(double **matrix_original, double **matrix_copy, int m, int n)
{
	int *row_pivots, column_index = 0, row_index = 0, count = 0;

	// allocate space for max number of column pivots
	row_pivots = malloc(m * sizeof(int));
	// ensure memory was allocated properly
	if (row_pivots == NULL) {
		fprintf(stderr, "Memory was not allocated properly in row_basis.\n");
		return;
	}
	// initialize elements to zero
	for (int i = 0; i < m; ++i) {
		row_pivots[i] = 0;
	}

	// find first pivot
	switch_pivot_column(matrix_copy, row_index, &column_index, n);
	row_pivots[row_index] = matrix_original[row_index][n];
	++row_index;

	// check to see if dealing with zero matrix
	if (column_index == 0 && matrix_copy[0][0] == 0) {
		printf("The basis vector of this matrix is zero.\n");
		return;
	}

	// find all the row pivots that are non zero
	while (row_index != m && column_index != n - 1) {
		int temp = column_index;
		switch_pivot_column(matrix_copy, row_index, &column_index, n);
		if (temp != column_index) {
			row_pivots[row_index] = matrix_copy[row_index][n];
			++row_index;
		}
		else {
			break;
		}
	}

	// print the row basis vectors
	printf("\nThe row basis vectors are:\n");
	for (int i = 0; i < row_index; ++i) {
		printf("(");
		int index = row_pivots[i];
		for (int k = 0; k < n; ++k) {
			printf("%0.2lf", matrix_original[index][k]);
			if (k != n - 1) {
				printf(", ");
			}
		}
		printf(")\n");
	}
	
	free(row_pivots);
}
