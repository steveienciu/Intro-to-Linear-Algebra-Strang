/* this will find the inverse matrix, if applicable, of a sqaure matrix */
/* matrix inversion only applicable to sqaure matrices */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FAILURE exit(1)
#define IDENTITY_MATRIX_FACTOR 2 // increase size of matrix by factor of 2 to allocate space for identity matrix which transforms into the inverse

void mem_failure(void);
void mem_alloc(double ***matrix, int m, int n);
void user_input(double **matrix, int m, int n);
void gaussian_elimination(double **matrix, int m, int n);
bool is_inverse(double **matrix, int m);
void jordan_elimination(double **matrix, int m, int n);
void print_matrix(double **matrix, int m, int n);
void free_matrix(double **matrix, int m);

int main()
{
	double **matrix;
	int m;
	bool isInverse;

	printf("\nThis program will take your matrix and perform Guassian elimination on it.\n");
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

	// does matrix have an inverse
	isInverse = is_inverse(matrix, m);

	// do following depending on if matrix has an inverse
	if (!isInverse) {
		printf("\nThe inputted matrix does not have an inverse.\n");
	}
	else {
		// perform Jordan elimination
		jordan_elimination(matrix, m, m);
		
		// print inverse matrix
		print_matrix(matrix, m, m);
	}

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
		(*matrix)[i] = malloc(n * IDENTITY_MATRIX_FACTOR * sizeof(double));
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
	// first half of matrix; user inputted matrix
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%lf", &matrix[i][j]);
		}
	}
	// second half of matrix; identity matrix
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i == j) {
				matrix[i][j + n] = 1.00;
			}
			else {
				matrix[i][j + n] = 0.00;
			}
		}
	}
}

void gaussian_elimination(double **matrix, int m, int n)
{
	int count = 0, i = 0, j = 0, s = 0;
	double multiplier = 0;
	
	// count number of iterations for Gaussian elimination
	for (int k = 0; k < m; ++k) {
		count += k;
	}
	
	// swap zero pivots with nonzero value below it 
	for (int a = 0; a < m; ++a) {
		// check if pivot is zero
		if (matrix[a][a] == 0 && (a + 1) < m) {
			for (int b = a + 1; b < m; ++b) {
				if (matrix[b][a] != 0) {
					for (int c = 0; c < n * IDENTITY_MATRIX_FACTOR; ++c) {
						// swap the rows
						double temp = matrix[a][c];
						matrix[a][c] = matrix[b][c];
						matrix[b][c] = temp;
					}
					break;
				}
			}
		}
	}
	
	// perform Gaussian elimination
	for (int k = 0; k < count; ++k) {
		multiplier = -(matrix[s + 1][j] / matrix[i][i]);
		for (int l = 0; l < n * IDENTITY_MATRIX_FACTOR; ++l) {
			matrix[s + 1][l] += multiplier * matrix[i][l];
		}
		++s;
		if ((s + 1) == m) {
			++j;
			++i;
			s = i;
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

void jordan_elimination(double **matrix, int m, int n)
{
	int count = 0, i = 0, j = 0, s = 0;
	double multiplier = 0;
	
	// count number of iterations for Jordan elimination
	for (int k = 0; k < m; ++k) {
		count += k;
	}

	// set variables up for the Jordan elimination
	s = m - 1;
	i = s;
	j = s; // j = n - 1 if have recatangular matrix
	// Jordan elimination
	for (int k = 0; k < count; ++k) {
		multiplier = -(matrix[s - 1][j] / matrix[i][i]);
		for (int l = n * IDENTITY_MATRIX_FACTOR; l > 0; --l) {
			matrix[s - 1][l - 1] += multiplier * matrix[i][l - 1];
		}
		--s;
		if (s == 0) {
			--j;
			--i;
			s = i;
		}
	}
	
	// divide row by the inverse of the pivot to turn user matrix into identity matrix
	for (int k = 0; k < m; ++k) {
		double divisor = 1 / matrix[k][k];
		for (int l = 0; l < n * IDENTITY_MATRIX_FACTOR; ++l) {
			matrix[k][l] *= divisor;
		}
	}
}
	
void print_matrix(double **matrix, int m, int n)
{
	printf("\nThe inverse matrix is:\n");
	for (int i = 0; i < m; ++i) {
		for (int j = n; j < n * IDENTITY_MATRIX_FACTOR; ++j) {
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
