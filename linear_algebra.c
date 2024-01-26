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

void clear_buffer(void)
{
	char ch;

	while ((ch = getchar()) != '\n');
}

double **matrix_mem_alloc(int m, int n)
{
	int counter = 0;
	bool isAllocated = true;

	double **matrix = malloc(m * sizeof(double *));
	if (matrix == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < m; ++i) {
		(matrix)[i] = malloc(n * sizeof(double));
		// check if row can be allocated
		if ((matrix)[i] == NULL) {
			isAllocated = false;
			break;
		}
		++counter;
	}

	if (!isAllocated) {
		free_matrix(matrix, counter);
		return NULL;
	}

	return matrix;
}

double **matrix_mem_realloc(double **old_matrix, int m_new, int n_new)
{
	int counter = 0;
	bool isAllocated = true;

	double **new_matrix = realloc(old_matrix, m_new * sizeof(double *));
	if (new_matrix == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < m_new; ++i) {
		(new_matrix)[i] = realloc(new_matrix[i], n_new * sizeof(double));
		// check if row can be allocated
		if ((new_matrix)[i] == NULL) {
			isAllocated = false;
			break;
		}
		++counter;
	}

	// free memory if row not allocated
	if (!isAllocated) {
		free_matrix(new_matrix, counter);
		return NULL;
	}

	return new_matrix;
}

void user_input_matrix(double **matrix, int m, int n)
{
	printf("Pleae enter elements of %d x %d matrix:\n", m, n);
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

bool is_inverse(double **matrix, int m)
{
	bool isInverse = true;

	// check to see if after gauss elimination the pivots are nonzero
	for (int i = 0; i < m; ++i) {
		if (matrix[i][i] == 0) {
			isInverse = false;
			break;
		}
	}

	return isInverse;
}
 
bool is_symmetric(double **matrix, int m, int n) 
{
	bool isSymmetric = true;
	
	// check for symmetry
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (matrix[i][j] != matrix[j][i]) {
				isSymmetric = false;
				return isSymmetric;
			}
		}
	}

	return isSymmetric;
}

void transpose_matrix(double **original_matrix, double **transpose_matrix, int m, int n)
{
	// take transpose of matrix
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			transpose_matrix[j][i] = original_matrix[i][j];
		}
	}
}

void switch_pivot_column(double **matrix, int row_index, int *column_index, int n2)
{
	for (int i = *column_index; i < n2; ++i) {
		// move pivot over if have nonzero column in that row
		if (matrix[row_index][i] != 0) {
			*column_index = i;
				break;
		}
	}
}	

void gauss_elimination(double **matrix, int m, int n1, int n2)
{
	double multiplier = 0;
	int row_index = 0, column_index = 0;

	// perform gaussian elimination
	while (column_index != n2) {
		// swap rows to get non-zero pivot
		if (matrix[row_index][column_index] == 0) {
			swap_row(matrix, m, n1);
		}
		// runs if have column full of zero; switch pivot column
		if (matrix[row_index][column_index] == 0) {
			switch_pivot_column(matrix, row_index, &column_index, n2);
		}
		// if reach this stage, rest of the matrix is zero
		if (matrix[row_index][column_index] == 0) {
			break;
		}
		// start of pivot
		for (int j = row_index + 1; j < m; ++j) {
			multiplier = -(matrix[j][column_index] / matrix[row_index][column_index]);
			// logic to manipulate row in question
			for (int k = 0; k < n1; ++k) {
				matrix[j][k] += multiplier * matrix[row_index][k];
			}
		}
		++row_index;
		++column_index;
		// ensure that row_index does not equal m; can happen when n > m
		if (row_index == m) {
			break;
		}
	}
}

void jordan_elimination(double **matrix, int m, int n1, int n2, int flag)
{
	double multiplier = 0;
	int row_index = 0, column_index = 0;

	// locate the first pivot
	int count = 0;
	for (int i = 0; i < n2; ++i) {
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
	if (count == n2) {
		return;
	}

	// perform Jordan elimination
	while (column_index != n2) {
		// start of pivot
		switch_pivot_column(matrix, row_index, &column_index, n2);	
		// if reach this stage, rest of the matrix is zero
		if (matrix[row_index][column_index] == 0) {
			break;
		}
		for (int j = row_index; j > 0; --j) {
			multiplier = -(matrix[j - 1][column_index] / matrix[row_index][column_index]);
			for (int k = 0; k < n1; ++k) {
				// logic to manipulate row in question
				matrix[j - 1][k] += multiplier * matrix[row_index][k];
			}
		}
		++row_index;
		++column_index;
		// ensure that row_index does not equal m; can happen when n > m
		if (row_index == m) {
			break;
		}
	}
	
	// divide each element in the row by its pivot value
	if (flag) {
		for (int i = 0; i < m; ++i) {
			double divisor = 1 / matrix[i][i];
			for (int j = 0; j < n1; ++j) {
				matrix[i][j] *= divisor;
			}
		}
	}
}

void free_matrix(double **matrix, int m)
{
	for (int i = 0; i < m; ++i) {
		free(matrix[i]);
	}
	free(matrix);
}
