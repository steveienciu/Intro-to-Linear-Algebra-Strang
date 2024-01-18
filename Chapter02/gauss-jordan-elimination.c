/* this will only handle guass-jordan matrix elimination of square matrices */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FAILURE exit(1)

void mem_failure(void);
void mem_alloc(double ***matrix, int m, int n);
void user_input(double **matrix, int m, int n);
void gaussian_elimination(double **matrix, int m, int n);
void jordan_elimination(double **matrix, int m, int n);
void print_matrix(double **matrix, int m, int n);
void free_matrix(double **matrix, int m);

int main()
{
	double **matrix;
	int m;

	printf("\nThis program will take your matrix and perform Guass-Jordan elimination on it.\n");
	printf("Please enter size of square matrix: ");
	scanf("%d", &m);

	if (m < 1) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for user inputted matrix
	mem_alloc(&matrix, m, m);

	// accept user input 
	user_input(matrix, m, m);

	// perform Gaussian elimination
	gaussian_elimination(matrix, m, m);

	// perform Jordan elimination
	jordan_elimination(matrix, m, m);

	// output Gauss-Jordan elimination matrix
	print_matrix(matrix, m, m);

	// deallocate memory used for matrix
	free_matrix(matrix, m);

	return 0;
}

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

void user_input(double **matrix, int m, int n)
{
	printf("Pleae enter elements of %d x %d matrix:\n", m, m);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%lf", &matrix[i][j]);
		}
	}
}

void gaussian_elimination(double **matrix, int m, int n)
{
	double multiplier = 0;
	
	// swap zero pivots with nonzero value below it 
	for (int i = 0; i < m; ++i) {
		// check if pivot is zero
		if (matrix[i][i] == 0 && (i + 1) < m) {
			for (int j = i + 1; j < m; ++j) {
				if (matrix[j][i] != 0) {
					for (int k = 0; k< n; ++k) {
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

	// perform gaussian elimination
	int row_index = 1;
	for (int i = 0; i < m - 1; ++i) {
		// start of pivot
		for (int j = row_index; j < m; ++j) {
			multiplier = -(matrix[j][i] / matrix[i][i]);
			// logic to manipulate row in question
			for (int k = 0; k < n; ++k) {
				matrix[j][k] += multiplier * matrix[i][k];
			}
		}
		++row_index;
	}
}

void jordan_elimination(double **matrix, int m, int n)
{
	double multiplier = 0;
	int row_index = m - 1;

	for (int i = 0; i < m - 1; ++i) {
		for (int j = row_index; j > 0; --j) {
			multiplier = -(matrix[j - 1][row_index] / matrix[row_index][row_index]);
			for (int k = 0; k < n; ++k) {
				matrix[j - 1][k] += multiplier * matrix[row_index][k];
			}
		}
		--row_index;
	}
}
	
void print_matrix(double **matrix, int m, int n)
{
	printf("\nGauss-Jordan elimination of the matrix is:\n");
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%5.2lf ", matrix[i][j]);
		}
		printf("\n");
	}
}

void free_matrix(double **matrix, int m)
{
	for (int i = 0; i < m; ++i) {
		free(matrix[i]);
	}
	free(matrix);
}
