/* program which finds all the solutions of */

#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

#define OUTPUT_COLUMN_SPACE 1 

struct nullspace {
	double *null_matrix;
};

void user_input_output(double **matrix, int m, int n);
bool is_solution(double **matrix, int m, int n);
int *find_pivot(double **matrix, int *column_pivot_index, int m, int n);
void print_single_solution(double **matrix, int n);
void find_free_variable(int *column_pivot_index, int *free_index, int pivot_count, int free_count, int n);
struct nullspace *nullspace_matrix_alloc(int n, int free_count);
void print_complete_solution(double **matrix, struct nullspace *data, int m, int n, int free_count, int pivot_count, int *free_index, int *column_pivot_index);
void free_nullspace(struct nullspace *data, int free_count);

int main()
{
	double **matrix;
	int m, n;
	bool isSolution = true;

	// output instructions for user to stdout
	printf("\nThis program will find the complete solution of a Ax = b matrix relationship.\n");
 
 	printf("\nPlease enter the number of rows for the coefficient matrix: ");
	scanf("%d", &m);

	// clear input buffer
	clear_buffer();

	printf("Please enter the number of columns for the coefficient matrix: ");
	scanf("%d", &n);

	// combine number of coefficient and output columns into one variable 
	int total_column = n + OUTPUT_COLUMN_SPACE;

	// clear input buffer
	clear_buffer();

	// allocate memory for matrix which combines coefficient matrix and output matrix
	matrix = matrix_mem_alloc(m, total_column);
	if (matrix == NULL) {
		mem_failure();
	}

	// accept user input for the coefficient part of matrix
	user_input_matrix(matrix, m, n);

	// clear input buffer
	clear_buffer();

	// accept user input for the output part of matrix
	user_input_output(matrix, m, n);

	// clear input buffer
	clear_buffer();

	// take gauss elimination of the matrix
	gauss_elimination(matrix, m, total_column, n, 1);

	// take jordan elimination of matrix
	jordan_elimination(matrix, m, total_column, n, 1);

	// are there any solutions (have no solutions if have zero row of coefficients but output for that row is nonzero
	isSolution = is_solution(matrix, m, total_column);

	if (!isSolution) {
		printf("\nThere is no solution.\n");
	}
	else {
		// divide row by their pivots; will make easier to find answer
		divide_by_pivot(matrix, m, total_column);

		// array which contains the pivot column indexes
		int *column_pivot_index;
		// add all the column indexes into the array
		column_pivot_index = find_pivot(matrix, column_pivot_index, m, n);

		// find number of free variables
		int pivot_count = 0;
		for (int i = 0; i < n; ++i) {
			if (i != 0 && column_pivot_index[i] == 0) {
				break;
			}
			++pivot_count;
		}
		// reallocate memory for array of column pivot indexes
		int *temp = realloc(column_pivot_index, pivot_count * sizeof(int));
		if (temp == NULL) {
			free_matrix(matrix, m); free(column_pivot_index);
			mem_failure();
		}
		else {
			column_pivot_index = temp;
		}

		// allocate space for number of free variables
		int free_count = n - pivot_count;

		// have and column rank; only one solution
		if (free_count == 0) {
			print_single_solution(matrix, n); 
			free(column_pivot_index);
		}
		// have infinite number of solutions
		else {
			// allovate sapce for free variable indexes array
			int *free_index = malloc(free_count * sizeof(int));
			// ensure that space is allocated properly 
			if (free_index == NULL) {
				free_matrix(matrix, m); free(column_pivot_index);
				mem_failure();
			}
			find_free_variable(column_pivot_index, free_index, pivot_count, free_count, n);
			
			// allocate memory for struct
			struct nullspace *data = nullspace_matrix_alloc(n, free_count);
			// check for proper allocation
			if (data == NULL) {
				free_matrix(matrix, m); free(column_pivot_index); free(free_index);
				FAILURE;
			}

			print_complete_solution(matrix, data, m, n, free_count, pivot_count, free_index, column_pivot_index); 
			// free memory for the arrays
			free(free_index); free(column_pivot_index); free_nullspace(data, free_count);
		}
	}

	// deallocate memory for the matrix
	free_matrix(matrix, m); 
	
	return 0;
}

void user_input_output(double **matrix, int m, int n) 
{
	printf("Please enter the elements of the %d x 1 output vector b:\n", m);
	for (int i = 0; i < m; ++i) {
		scanf("%lf", &matrix[i][n]);
	}
}

bool is_solution(double **matrix, int m, int n)
{
	bool isSolution = true, is_zero_row = false;
	int count = 0;

 	for (int i = 0; i < m; ++i) {
		// see if have row of zero's, not including the output column
		for (int j = 0; j < n - 1; ++j) {
			if (matrix[i][j] == 0) {
				++count;
			}
		}
		// check to see if output vector has zero element 
		if (count == n - 1 && matrix[i][n - 1] != 0) {
			isSolution = false;
		}
		count = 0;
	}

	return isSolution;
}

int *find_pivot(double **matrix, int *column_pivot_index, int m, int n)
{
	int column_index = 0, row_index = 0;

	// allocate space for max number of column pivots
	column_pivot_index = malloc(n * sizeof(int));
	// ensure memory was allocated properly 
	if (column_pivot_index == NULL) {
		fprintf(stderr, "\nMemory was not allocated properly in find_pivot.\n");
		free_matrix(matrix, m);
		FAILURE;
	}
	// initialize elements to zero
	for (int i = 0; i < n; ++i) {
		column_pivot_index[i] = 0;
	}

	// find first pivot
	switch_pivot_column(matrix, row_index, &column_index, n);
	column_pivot_index[row_index] = column_index;
	++row_index;

	// check to see if dealing with zero matrix

	// find all column_pivots that are non zero
	while (row_index != m && column_index != n - 1) {
		int temp = column_index;
		switch_pivot_column(matrix, row_index, &column_index, n);
		if (temp != column_index) {
			column_pivot_index[row_index] = column_index;
			++row_index;
		}
		else {
			break;
		}
	}

	return column_pivot_index;
}

void print_single_solution(double **matrix, int n)
{
	printf("\nThe solution is:\n");
	printf("(");
	for (int i = 0; i < n; ++i) {
		printf("%0.2lf", matrix[i][n]);
		if (i != n - 1) {
			printf(", ");
		}
	}
	printf(")");
}

void find_free_variable(int *column_pivot_index, int *free_index, int pivot_count, int free_count, int n)
{
	bool is_pivot = true;
	int index = 0;
	// can use some other searching algo here; will change up later
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < pivot_count; ++j) {
			if (column_pivot_index[j] == i) {
				is_pivot = true;
				break;
			}
			// ran if index does not have pivot value
			else {
				is_pivot = false;
			}
		}
		// add that index to the array
		if (!is_pivot) {
			free_index[index] = i;
			++index;
		}
		if (index == free_count) {
			break;
		}
	}
}

struct nullspace *nullspace_matrix_alloc(int n, int free_count)
{
	struct nullspace *data = malloc(free_count * sizeof(struct nullspace));
	// allocate space for the structure
	if (data == NULL) {
		fprintf(stderr, "\nMemory allocation failed in nullspace_matrix_alloc.\n");
		return NULL;
	}

	// allocate space for the array element in the structure 
	for (int i = 0; i < free_count; ++i) {
		data[i].null_matrix = malloc(n * sizeof(double));
		if (data[i].null_matrix == NULL) {
			fprintf(stderr, "\nMemory allocation failed in nullspace_matrix_alloc.\n");
			// deallocate memory for structure and array in structure
			for (int j = 0; j < i; ++j) {
				free(data[j].null_matrix);
			}
			free(data);
			return NULL;
		}
	}

	return data;
}

void print_complete_solution(double **matrix, struct nullspace *data, int m, int n, int free_count, int pivot_count, int *free_index, int *column_pivot_index)
{
	// allocate space for anchor array
	double *anchor_solution = malloc(n * sizeof(double));
	// make sure memory allocated properly 
	if (anchor_solution == NULL) {
		fprintf(stderr, "\nMemory allocation failure in print_complete_solution\n");
		free_matrix(matrix, m); free(free_index); free(column_pivot_index); free_nullspace(data, free_count);
		FAILURE;
	}
	// intialize elements to zero
	for (int i = 0; i < n; ++i) {
		anchor_solution[i] = 0;
	}
	// add the elements for the anchor matrix
	for (int i = 0; i < pivot_count; ++i) {
		int temp = column_pivot_index[i];
		anchor_solution[temp] = matrix[i][n];
	}
	// output the anchoring solution
	printf("\nThe complete solution is:\n");
	printf("(");
	for (int i = 0; i < n; ++i) {
		printf("%0.2lf", anchor_solution[i]);
		if (i != n - 1) {
			printf(", ");
		}
	}
	printf(")");
	// free memory for anchor array
	free(anchor_solution);

	// get nullspace vectors partially set up; have the free variables set up
	for (int i = 0; i < free_count; ++i) {
		for (int j = 0; j < n; ++j) {
			if (free_index[i] == j) {
				data[i].null_matrix[j] = 1.00;
			}
			else {
				data[i].null_matrix[j] = 0.00;
			}
		}
	}

	// get nullspace vectors fully set up
	for (int i = 0; i < free_count; ++i) {
		for (int j = pivot_count; j > 0; --j) {
			int temp = 0;
			for (int k = 0; k < n; ++k) {
				temp += matrix[j - 1][k] * data[i].null_matrix[k];
				if (temp != 0) {
					int index = column_pivot_index[j - 1];
					data[i].null_matrix[index] = -temp;
				}
			}
		}
	}

	// print values of nullspace vector(s)
	for (int i = 0; i < free_count; ++i) {
		printf(" + x%d(", free_index[i] + 1);
		for (int j = 0; j < n; ++j) {
			printf("%0.2lf", data[i].null_matrix[j]);
			if (j != n - 1) {
				printf(", ");
			}
		}
		printf(")");
	}
	printf("\n");
}

void free_nullspace(struct nullspace *data, int free_count)
{
	// incase deallocation has already been done
	if (data == NULL) {
		return;
	}

	// free memory for the nullspaces
	for (int i = 0; i < free_count; ++i) {
		free(data[i].null_matrix);
	}

	free(data);
}
