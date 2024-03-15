/* program that finds the uknowns using Cramer's rule */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void find_unknowns(double **coeff_matrix, double **output_matrix, int m);

int main()
{
	double **coeff_matrix, **output_matrix;
	int m;

	// output instructions for user to stdout
	printf("\nThis program will find the unknowns using Cramer's rule.\n");

	printf("Please enter the dimensions for the square matrix: ");
	scanf("%d", &m);

	// clear input buffer
	clear_buffer();

	// check for valid dimensions
	if (m < 2) {
		fprintf(stderr, "\nInvalid dimensions. Terminating program.\n");
	}

	// allocate space for the coefficient matrix
	coeff_matrix = matrix_mem_alloc(m, m);
	if (coeff_matrix == NULL) {
		mem_failure();
	}

	// allocate space for output matrix
	output_matrix = matrix_mem_alloc(m, 1);
	if (output_matrix == NULL) {
		free_matrix(coeff_matrix, m);
		mem_failure();
	}

	// get user input for coeff matrix
	user_input_matrix(coeff_matrix, m, m);

	// clear input buffer
	clear_buffer();

	// get user input for the output matrix
	user_input_matrix(output_matrix, m, 1);

	// clear input buffer
	clear_buffer();

	find_unknowns(coeff_matrix, output_matrix, m);

	return 0;
}

void find_unknowns(double **coeff_matrix, double **output_matrix, int m)
{
	double determinant = 0.0;

	// allocate space for determinats of the unknown matrices
	double *unknown = malloc(m * sizeof(double));
	if (unknown == NULL) {
		free_matrix(coeff_matrix, m); free_matrix(output_matrix, m);
		fprintf(stderr, "\nMemory allocation failure in find_unkowns on line 67. Terminating program.\n");
		FAILURE;
	}

	// find the determinants of the unknowns
	for (int i = 0; i < m; ++i) {
		// allocate space for the unknown determinant matrix
		double **determinant_matrix = matrix_mem_alloc(m, m);
		if (determinant_matrix == NULL) {
			free_matrix(coeff_matrix, m); free_matrix(output_matrix, m); free(unknown);
			fprintf(stderr, "\nMemory allocation failure in find_unknowns on line 77. Terminating program.\n");
			FAILURE;
		}

		// set up matrix with the coefficient matrix
		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < m; ++k) {
				determinant_matrix[j][k] = coeff_matrix[j][k];
			}
		}

		// swap the proper column with the output matrix, fully setting up the determinant matrix
		for (int j = 0; j < m; ++j) {
			determinant_matrix[j][i] = output_matrix[j][0];
		}

		// take gauss of the determinant matrix
		gauss_elimination(determinant_matrix, m, m, m, 1);

		double temp = find_determinant(determinant_matrix, m);
		// add the determinant into the array
		unknown[i] = temp;

		free_matrix(determinant_matrix, m);
	}

	// take gauss of the coefficient matrix
	// not sure if would be better to do this before or after finding the determinants of the unknown matrices
	gauss_elimination(coeff_matrix, m, m, m, 1);

	// find determinant of coefficient matrix
	determinant = find_determinant(coeff_matrix, m);
	// cannot have coefficient matrix with a zero determinant 
	if (determinant == 0) {
		fprintf(stderr, "\nCoefficient matrix has dependent vectors. Terminating program.\n");
		free_matrix(coeff_matrix, m); free_matrix(output_matrix, m); free(unknown);
		FAILURE;
	}

	// print the results of the unknowns
	printf("The unknowns are ");
	for (int i = 0; i < m; ++i) {
		double temp = unknown[i] / determinant;
		printf("x%d=%0.2lf", i + 1, temp);
		if (i != m - 1) {
			printf(", ");
		}
	}
	printf("\n");

	free(unknown); free_matrix(coeff_matrix, m); free_matrix(output_matrix, m);
}
