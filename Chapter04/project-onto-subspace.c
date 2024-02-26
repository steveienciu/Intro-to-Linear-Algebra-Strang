/* this program projects one line onto a subspace */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

int *column_basis(double **subspace, int *pivot_count, const int m, const int n);
void subspace_basis(double **subspace, double **subspace_copy, int *subspace_column_pivots, const int m, const int pivot_count);
void find_normal_equation(double **normal_equation, double **subspace_product, double **projection_product, const int pivot_count);
bool is_solution(double **error_vector, double **projection_vector, double **vectorb, double **subspace_copy, const int m, const int vecotr_column, const int pivot_count);
void output_projection_vector(double **projection_vector, const int m, const int vector_column);

int main()
{
	double **subspace, **subspace_copy, **vectorb;
	int m, n, *subspace_column_pivots, pivot_count = 0;
	bool is_sol;

	// output intructions for user to stdout
	printf("\nThis program will project one line onto a subspace.\n");

	// get the dimension of the vector and subspace
	printf("\nPlease enter the number of rows of the vectors: ");
	scanf("%d", &m);

	// ensure have valid dimension
	if (m < 2) {
		fprintf(stderr, "\nImproper vector dimension on line 29 in main.\n");
		FAILURE;
	}

	// clear input buffer
	clear_buffer();

	// allocate space for the vector to be projected
	vectorb = matrix_mem_alloc(m, 1);
	if (vectorb == NULL) {
		mem_failure();
	}

	// find out how many vectors are in the subspace
	printf("How many vectors are in the subspace to be projected on: ");
	scanf("%d", &n);
	if (n < 1) {
		fprintf(stderr, "\nImproper number of vectors in subspace on line 46 in main.\n");
	}

	// clear input buffer
	clear_buffer();

	// allocate space for the subspace
	subspace = matrix_mem_alloc(m, n);
	if (subspace == NULL) {
		free_matrix(vectorb, m);
		mem_failure();
	}

	// allocate space for subspace copy matrix which will be used to find column basis
	subspace_copy = matrix_mem_alloc(m, n);
	if (subspace_copy == NULL) {
		free_matrix(vectorb, m); free_matrix(subspace, m);
		mem_failure();
	}

	// accept user input for subspace
	user_input_matrix(subspace, m, n);
	// copy inputs in the subspace copy
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			subspace_copy[i][j] = subspace[i][j];
		}
	}

	// accept user input for vector
	user_input_matrix(vectorb, m, 1);

	// find gauss-jordan of matrix
	gauss_elimination(subspace_copy, m, n, n, 1);
	jordan_elimination(subspace_copy, m, n, n, 1);

	// find the column pivots indexes
	subspace_column_pivots = column_basis(subspace_copy, &pivot_count, m, n);
	if (subspace_column_pivots == NULL) {
		if (pivot_count != 0) {
			printf("\nZero subspace detected. Terminating program.\n");
			free_matrix(subspace, m); free_matrix(subspace_copy, m); free_matrix(vectorb, m);
			free(subspace_column_pivots);
			FAILURE;
		}
		else {
			free_matrix(subspace, m); free_matrix(subspace_copy, m); free_matrix(vectorb, m);
			mem_failure();
		}
	}

	if (pivot_count <= 1) {
		fprintf(stderr, "\nYou are projecting a line onto a line. Please enter a subspace. Terminating program.\n");
		free_matrix(subspace, m); free_matrix(subspace_copy, m); free_matrix(vectorb, m); 
		free(subspace_column_pivots);
		FAILURE;
	}

	// reallocate subspace_copy matrix to hold the column basis
	double **subspace_temp = matrix_mem_realloc(subspace_copy, m, pivot_count);
	if (subspace_temp == NULL) {
		free(subspace_column_pivots); free_matrix(subspace, m); free_matrix(subspace_copy, m); free_matrix(vectorb, m);
		mem_failure();
	}
	else {
		subspace_copy = subspace_temp;
	}

	// update subspace_copy with only the vectors in the subspace
	subspace_basis(subspace, subspace_copy, subspace_column_pivots, m, pivot_count);

	// no longer need these arrays
	free(subspace_column_pivots); free_matrix(subspace, m);

	// get the tranpose of the subspace 
	double **subspace_transpose = matrix_mem_alloc(pivot_count, m);
	if (subspace_transpose == NULL) {
		free_matrix(subspace_copy, m); free_matrix(vectorb, m);
		mem_failure();
	}
	transpose_matrix(subspace_copy, subspace_transpose, m, pivot_count);

	// find (A^T) * A
	double **subspace_product = matrix_mem_alloc(pivot_count, pivot_count);
	if (subspace_product == NULL) {
		free_matrix(subspace_copy, m); free_matrix(subspace_transpose, pivot_count); free_matrix(vectorb, m);
		mem_failure();
	}
	matrix_multiplication(subspace_product, subspace_transpose, subspace_copy, pivot_count, pivot_count, m);

	// find projection product (A^T) * b
	double **projection_product = matrix_mem_alloc(pivot_count, 1);
	if (projection_product == NULL) {
		free_matrix(subspace_copy, m); free_matrix(subspace_transpose, pivot_count);
		free_matrix(subspace_product, pivot_count); free_matrix(vectorb, m);
		mem_failure();
	}
	matrix_multiplication(projection_product, subspace_transpose, vectorb, pivot_count, 1, m);
	
	// no longer needed
	free_matrix(subspace_transpose, pivot_count);

	// allocate space for normal equation matrix
	double **normal_equation = matrix_mem_alloc(pivot_count, pivot_count + 1);
	if (normal_equation == NULL) {
		free_matrix(subspace_copy, m); free_matrix(subspace_product, pivot_count);
		free(vectorb); free_matrix(projection_product, pivot_count);
		mem_failure();
	}
	// find normal equation
	find_normal_equation(normal_equation, subspace_product, projection_product, pivot_count);

	// allocate space for scalar function
	double **scalars = matrix_mem_alloc(pivot_count, 1);
	if (scalars == NULL) {
		free_matrix(subspace_copy, m); free_matrix(subspace_product, pivot_count);
		free_matrix(vectorb, m); free_matrix(projection_product, pivot_count); free_matrix(normal_equation, pivot_count);
		mem_failure();
	}
	// find the value for the scalar function
	for (int i = 0; i < pivot_count; ++i) {
		scalars[i][0] = normal_equation[i][pivot_count];
	}

	// no longer needed
	free_matrix(normal_equation, pivot_count); free_matrix(projection_product, pivot_count);
	free_matrix(subspace_product, pivot_count);

	// find the projection vector 
	double **projection_vector = matrix_mem_alloc(m, 1);
	if (projection_vector == NULL) {
		free_matrix(vectorb, m); free_matrix(subspace_copy, m); free_matrix(scalars, pivot_count);
		mem_failure();
	}
	matrix_multiplication(projection_vector, subspace_copy, scalars, m, 1, pivot_count);

	// no longer needed
	free_matrix(scalars, pivot_count);

	// double check to see if answer is correct
	double **error_vector = matrix_mem_alloc(m, 1);
	if (error_vector == NULL) {
		free_matrix(projection_vector, m); free_matrix(vectorb, m); free_matrix(subspace_copy, m);
		mem_failure();
	}
	is_sol = is_solution(error_vector, projection_vector, vectorb, subspace_copy, m, 1, pivot_count);

	if (is_sol) {
		output_projection_vector(projection_vector, m, 1);
	}
	else {
		// should never reach this
		printf("There was an issue with the code. Terminating program.\n");
	}

	// free remaining matrices
	free_matrix(error_vector, m); free_matrix(subspace_copy, m);
	free_matrix(vectorb, m); free_matrix(projection_vector, m);

	return 0;
}

int *column_basis(double **subspace_copy, int *pivot_count, const int m, const int n)
{
	int *column_pivots, column_index = 0;

	// allocate space for max number of column pivots
	column_pivots = malloc(m * sizeof(int));
	// ensure memory was allocated properly
	if (column_pivots == NULL) {
		fprintf(stderr, "Memory was not allocated properly in column_basis.\n");
		return NULL;
	}
	// initialize elements to zero
	for (int i = 0; i < m; ++i) {
		column_pivots[i] = 0;
	}

	// find first pivot
	switch_pivot_column(subspace_copy, *pivot_count, &column_index, n);
	column_pivots[*pivot_count] = column_index;
	++(*pivot_count);

	// check to see if dealing with zero matrix
	if (column_index == 0 && subspace_copy[0][0] == 0) {
		return NULL;
	}

	// find all column pivots that are non zero
	while (*pivot_count != m && column_index != n - 1) {
		int temp = column_index;
		switch_pivot_column(subspace_copy, *pivot_count, &column_index, n);
		if (temp != column_index) {
			column_pivots[*pivot_count] = column_index;
			++(*pivot_count);
		}
		else {
			break;
		}
	}
	
	return column_pivots;
}

void subspace_basis(double **subspace, double **subspace_copy, int *subspace_column_pivots, const int m, const int pivot_count)
{
	// update subspace_copy with only the vectors in the subspace
	for (int i = 0; i < pivot_count; ++i) {
		int temp = subspace_column_pivots[i];
		for (int j = 0; j < m; ++j) {
			subspace_copy[j][i] = subspace[j][temp];
		}
	}
}

void find_normal_equation(double **normal_equation, double **subspace_product, double **projection_product, const int pivot_count)
{
	// number of columns for normal equation
	int tot_col = pivot_count + 1;

	// configure the normal equation matrix
	for (int i = 0; i < pivot_count; ++i) {
		for (int j = 0; j < tot_col; ++j) {
			if (j != pivot_count) {
				normal_equation[i][j] = subspace_product[i][j];
			}
			else {
				normal_equation[i][j] = projection_product[i][0];
			}
		}
	}

	// prepare normal equation to easily find the scalar function
	gauss_elimination(normal_equation, pivot_count, tot_col, pivot_count, 1);
	jordan_elimination(normal_equation, pivot_count, tot_col, pivot_count, 1);
	divide_by_pivot(normal_equation, pivot_count, tot_col);
}

bool is_solution(double **error_vector, double **projection_vector, double **vectorb, double **subspace_copy, const int m, const int vector_column, const int pivot_count)
{
	// get the projection to be the negative of itself
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < vector_column; ++j) {
			projection_vector[i][j] = (-1) * projection_vector[i][j];
		}
	}

	// find the error vector
	matrix_addition(error_vector, vectorb, projection_vector, m, vector_column);

	// see if error vector perpendicular to the subspace
	for (int i = 0; i < pivot_count; ++i) {
		double value = 0.0;
		for (int j = 0; j < m; ++j) {
			value += subspace_copy[j][i] * error_vector[j][vector_column - 1];
		}
		if (value != 0) {
			return false;
		}
	}

	return true;
}

void output_projection_vector(double **projection_vector, const int m, const int vector_column)
{
	// multiply projection by -1 to offset what was done in is_solution
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < vector_column; ++j) {
			projection_vector[i][j] = (-1) * projection_vector[i][j];
		}
	}

	// output projection vector
	printf("\nThe projection vector is (");
	for (int i = 0; i < m; ++i) {
		printf("%0.2lf", projection_vector[i][0]);
		if (i != m - 1) {
			printf(", ");
		}
	}
	printf(")\n");
}
