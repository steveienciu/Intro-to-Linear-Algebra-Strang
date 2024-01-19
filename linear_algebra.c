// definitions for all functions in the header file 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void mem_failure(void)
{
	fprintf(stderr, "\nMatrix was not properly allocated.\n");
	FAILURE;
}

void mem_alloc(double ***matrix, int m, int n)
{
	int counter = 0;
	bool isAllocated = true;

	*matrix = malloc(m * sizeof(double *));
	if (*matrix == NULL)
	{
		mem_failure();
	}
	for (int i = 0; i < n; ++i) {
		(*matrix)[i] = malloc(n * sizeof(double));
		// check if row can be allocated
		if ((*matrix)[i] == NULL) {
			isAllocated = false;
			break;
		}
		++counter;
	}

	// free memory if row not allocated
	if (!isAllocated) {
		free_matrix(*matrix, counter);
		mem_failure();
	}
}

void user_input_matrix(double **matrix, int m, int n)
{
	char ch;

	// flush input buffer
	while ((ch = getchar()) != '\n');

	printf("Pleae enter elements of %d x %d matrix:\n", m, m);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%lf", &matrix[i][j]);
		}
	}
}

void swap_row(double **matrix, int m, int n)
{
	// swap zero pivots with nonzero value below it
	for (int i = 0; i < m; ++i) {
		// check if pivot is zero
		if (matrix[i][i] == 0 && (i + 1) < m) {
			for (int j = i + 1; j < m; ++j) {
				if (matrix[j][i] != 0) {
					for (int k = 0; k < n; ++k) {
						// swap the rows
						double temp = matrix[i][k];
						matrix[i][k] = matrix[j][k];
						matrix[j][k] = temp;
					}
					break;
				}
			}
		}
	}
}

void gauss_elimination(double **matrix, int m, int n)
{
	double multiplier = 0;
	int row_index = 0, column_index = 0;

	// perform gaussian elimination
	while (column_index != n) {
		// swap rows to get non-zero pivot
		if (matrix[row_index][column_index] == 0) {
			swap_row(matrix, m, n);
		}
		// runs if have column full of zero; switch pivot column
		if (matrix[row_index][column_index] == 0) {
			for (int i = 1; i < n; ++i) {
				if (matrix[row_index][i] != 0) {
					column_index = i;
					break;
				}
			}
		}
		// if reach this stage, rest of the matrix is zero
		if (matrix[row_index][column_index] == 0) {
			break;
		}
		// start of pivot
		for (int j = row_index + 1; j < m; ++j) {
			multiplier = -(matrix[j][column_index] / matrix[row_index][column_index]);
			// logic to manipulate row in question
			for (int k = 0; k < n; ++k) {
				matrix[j][k] += multiplier * matrix[row_index][k];
			}
		}
		++row_index;
		++column_index;
	}
}

void jordan_elimination(double **matrix, int m, int n)
{
	double multiplier = 0;
	int row_index = 0, column_index = 0;

	// locate the first pivot
	int count = 0;
	for (int i = 0; i < n; ++i) {
		// see if dealing with zero matrix
		if (matrix[row_index][i] == 0) {
			++count;
		}
		// finds the first pivot column
		else {
			++row_index;
			column_index = i;
			break;
		}
	}

	// dealing with zero matrix if this is true
	if (count == n) {
		return;
	}

	// perform Jordan elimination
	while (column_index != n) {
		// start of pivot
		for (int i = column_index; i < n; ++i) {
			if (matrix[row_index][i] != 0) {
				column_index = i;
				break;
			}
		}
		// if reach this stage, rest of the matrix is zero
		if (matrix[row_index][column_index] == 0) {
			break;
		}
		for (int j = row_index; j > 0; --j) {
			multiplier = -(matrix[j - 1][column_index] / matrix[row_index][column_index]);
			for (int k = 0; k < n; ++k) {
				// logic to manipulate row in question
				matrix[j - 1][k] += multiplier * matrix[row_index][k];
			}
		}
		++row_index;
		++column_index;
	}
}

void free_matrix(double **matrix, int m)
{
	for (int i = 0; i < m; ++i) {
		free(matrix[i]);
	}
	free(matrix);
}
