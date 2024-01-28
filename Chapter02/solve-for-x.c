/* program that solves for x in Ax=b */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void user_input_output(double **matrix, int m);
void print_answer(double **matrix, int m);

int main()
{
	double **matrix;
	int m;
	bool isInverse;

	printf("\nThis program will take your matrix and find x in Ax=b.\n");
	printf("Please enter size of square matrix: ");
	scanf("%d", &m);

	if (m < 1) {
		fprintf(stderr, "Invalid size of matrix.\n");
		FAILURE;
	}

	// allocate memory for user inputted matrix
	matrix = matrix_mem_alloc(m, m + 1);

	// accept user input for matrix
	user_input_matrix(matrix, m, m);

	// accept user input for vector b
	user_input_output(matrix, m);

	// perform Gaussian elimination
	gauss_elimination(matrix, m, m + 1, m, 1);

	// check is matrix has inverse
	isInverse = is_inverse(matrix, m);
	
	// do following depending on if matrix has inverse
	if (!isInverse) {
		printf("\nThe inputted matrix does not have only one solution.\n");
	}
	else {
		// perform Jordan elimination
		jordan_elimination(matrix, m, m + 1, m, 1);

		// divide row by the pivot columns element
		divide_by_pivot(matrix, m, m + 1);

		// print the answer for the x vector
		print_answer(matrix, m);
	}
	// deallocate memory used for matrix
	free_matrix(matrix, m);

	return 0;
}

void user_input_output(double **matrix, int m)
{
	char ch;

	// flush input buffer
	while ((ch = getchar()) != '\n');

	printf("\nPlease enter the elements of the vector b:\n");
	for (int i = 0; i < m; ++i) {
		scanf("%lf", &matrix[i][m]);
	}
}

void print_answer(double **matrix, int m)
{
	double answer = 0;

	printf("\nThe vector x has the following answer:\n");
	for (int i = 0; i < m; ++i) {
		answer = matrix[i][m] / matrix[i][i];
		printf("x%d=%.2lf\n", i + 1, answer);
	}
}
