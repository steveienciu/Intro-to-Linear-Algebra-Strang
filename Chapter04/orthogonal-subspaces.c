/* program that finds if two subspaces are orthogonal to each other */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

bool is_orthogonal(double **matrix1, double **matrix2, int m1, int m2, int n1, int n2);

int main()
{
	double **matrix1, **matrix2;
	int m1, m2, n1, n2;
	bool isOrthogonal;
	FILE *error_log;

	// open up error log file (curious to see how this works)
	error_log = fopen("/home/toddjohn/log/linear_algerba/error_log.txt", "a");
	if (error_log == NULL) {
		fprintf(stderr, "Error opening file: %s\n", "/var/log/linear_algebra/error_log.txt");
		FAILURE;
	}

	// output instructions for user to stdout
	printf("\nThis program will determine whether two subspaces are orthogonal.\n");

	// get specs for the first matrix
	printf("\nPlease enter the number of rows for the first matrix: ");
	scanf("%d", &m1);
	clear_buffer();
	printf("Please enter the number of columns for the first matrix: ");
	scanf("%d", &n1);
	clear_buffer();
	
	// check to see if dimensions within the proper range
	if (m1 < 1 || n1 < 1) {
		fprintf(error_log, "severity=error; program=orthogonal-subspaces; reason=improper matrix1 dimension line 33 in main()\n");
		fprintf(stderr, "Invalid.\n");
		fclose(error_log);
		FAILURE;
	}

	// allocate memory for the first matrix
	matrix1 = matrix_mem_alloc(m1, n1);
	if (matrix1 == NULL) {
		fprintf(error_log, "severity=error; program=orthogonal-subspaces; reason=matrix1 failed to allocate on line 41 in main()\n");
		fclose(error_log);
		mem_failure();
	}

	// get specs for the second matrix
	printf("\nPlease enter the number of rows for the second matrix: ");
	scanf("%d", &m2);
	clear_buffer();
	printf("Please enter the number of columns for the second matrix: ");
	scanf("%d", &n2);
	clear_buffer();

	// check to see if dimensions within the proper range
	if (m2 < 1 || n2 < 1) {
		free_matrix(matrix1, m1);
		fprintf(error_log, "severity=error; program=orthogonal-subspaces; reason=improper matrix2 dimensions line 56 in main()\n");
		fprintf(stderr, "Invalid\n");
		fclose(error_log);
		FAILURE;
	}

	if (m1 != m2) {
		free_matrix(matrix1, m1); free_matrix(matrix2, m2);
		fprintf(error_log, "severity=error; program=orthognal-space; reason=dimensions for the two matrices do not match up line 67 in main()\n");
		fclose(error_log);
		FAILURE;
	}

	// allocate memory for the second matrix
	matrix2 = matrix_mem_alloc(m2, n2);
	if (matrix2 == NULL) {
		free_matrix(matrix1, m1);
		fprintf(error_log, "severity=error; program=orhtogonal-subspaces; reason=matrix2 failed to alloacte on line 64 in main()\n");
		fclose(error_log);
		mem_failure();
	}

	// get elements for the first matrix
	printf("\n");
	user_input_matrix(matrix1, m1, n1);

	// clear input buffer
	clear_buffer();

	// get elements for the second matrix
	printf("\n");
	user_input_matrix(matrix2, m2, n2);

	isOrthogonal = is_orthogonal(matrix1, matrix2, m1, m2, n1, n2);

	if (isOrthogonal) {
		printf("\nThe two subspaces are orthogonal.\n");
	}
	else {
		printf("\nThe two subspaces are not orthogonal.\n");
	}

	// free the matrices
	free_matrix(matrix1, m1); free_matrix(matrix2, m1);
	// close the file 
	fclose(error_log);

	return 0;
}

bool is_orthogonal(double **matrix1, double **matrix2, int m1, int m2, int n1, int n2)
{
	for (int i = 0; i < n1; ++i) {
		for (int j = 0; j < n2; ++j) {
			int value = 0;
			for (int k = 0; k < m1; ++k) {
				value += matrix1[k][i] * matrix2[k][j];
			}
			if (value != 0) {
				return false;
			}
		}
	}

	return true;
}
