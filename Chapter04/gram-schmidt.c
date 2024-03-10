/* program that uses Gram-Schmidt to turn vectors into orthonormal vectors */
/* could accept dependent vectors as well but already done something like that before and takes a lot of logic to complete */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "linear_algebra.h"

// struct to hold values of the input and orthonormal vector
struct vectors {
	double *input_vector;
	double *ortho_vector;
};

// struct to hold the temporary projection vectors
struct gram_schmidt {
	double *gs_vector;
};

struct vectors *vector_malloc(int m, int n);
void user_input_vector(struct vectors *data, int m, int n);
struct gram_schmidt *gs_malloc(int m, int count);
void find_ortho_vectors(struct vectors *data, int m, int n);
void print_results(struct vectors *data, int m, int n);
void free_vector(struct vectors *data, int n);
void free_gs(struct gram_schmidt *project, int count);

int main()
{
	int m, n;

	// output instructions for user to stdout 
	printf("\nThis program will find the orthonormal equivalent of your independent vectors.\n");

	printf("Please enter the dimension for the independent vectors: ");
	scanf("%d", &m);

	// clear input buffer 
	clear_buffer();

	printf("Please enter the number of independent vectors: ");
	scanf("%d", &n);

	// check for valid dimensions
	if (m < 2 || n < 2) {
		fprintf(stderr, "\nHave invalid dimensions. Terminating program.\n");
		FAILURE;
	}

	// check to see if impossible to have independent vectors inputted
	if (n > m) {
		fprintf(stderr, "\nThe vectors inputted are not independent. Terminating program.\n");
		FAILURE;
	}

	// clear input buffer 
	clear_buffer();

	// allocate space for the user input vectors
	struct vectors *data = vector_malloc(m, n);
	if (data == NULL) {
		mem_failure();
	}

	// get the users vectors in the struct
	user_input_vector(data, m, n);

	// use the gram-schmidt process to find the orthogonal vectors
	find_ortho_vectors(data, m, n);

	// print the orthonormal vectors 
	print_results(data, m, n);

	// free the struc
	free_vector(data, n);

	return 0;
}

struct vectors *vector_malloc(int m, int n)
{
	// allocate space for the structure
	struct vectors *data = malloc(n * sizeof(struct vectors));
	if (data == NULL) {
		fprintf(stderr, "\nMemory allocation failed in vector_malloc.\n");
		return NULL;
	}

	// allocate space for input vector
	for (int i = 0; i < n; ++i) {
		data[i].input_vector = malloc(m * sizeof(double));
		if (data[i].input_vector == NULL) {
			fprintf(stderr, "\nMemory allocation failed in vector_malloc.\n");
			// deallocate memory for input vector if fails
			for (int j = 0; j < i; ++j) {
				free(data[j].input_vector);
			}
			free(data);
			return NULL;
		}
	}

	// allocate space for output vector 
	for (int i = 0; i < n; ++i) {
		data[i].ortho_vector = malloc(m * sizeof(double));
		if (data[i].ortho_vector == NULL) {
			fprintf(stderr, "\nMemory allocation failed in vector_malloc.\n");
			// deallocate memory for output vector if fails
			for (int j = 0; j < i; ++j) {
				free(data[j].ortho_vector);
			}
			for (int j = 0; j < n; ++j) {
				free(data[j].input_vector);
			}
			free(data);
			return NULL;
		}
	}

	return data;
}

void user_input_vector(struct vectors *data, int m, int n)
{
	printf("\n");
	for (int i = 0; i < n; ++i) {
		printf("Please enter the elements of vector %d:\n", i + 1);
		for (int j = 0; j < m; ++j) {
			scanf("%lf", &data[i].input_vector[j]);
		}
	}
}

struct gram_schmidt *gs_malloc(int m, int count)
{
	// allocate space for the structure 
	struct gram_schmidt *data = malloc(count * sizeof(struct gram_schmidt));
	if (data == NULL) {
		fprintf(stderr, "\nMemory allocation failed in gs_malloc.\n");
		return NULL;
	}

	// allocate space for the array element in the structure 
	for (int i = 0; i < count; ++i) {
		data[i].gs_vector = malloc(m * sizeof(double));
		if (data[i].gs_vector == NULL) {
			fprintf(stderr, "\nMemory allocation failed in gs_malloc.\n");
			// deallocate memory for successful allocations
			for (int j = 0; j < i; ++j) {
				free(data[i].gs_vector);
			}
			free(data);
			return NULL;
		}
	}

	return data;
}

void find_ortho_vectors(struct vectors *data, int m, int n)
{
	// have the first vector be the anchor for the orthonormal vectors
	for (int i = 0; i < m; ++i) {
		data[0].ortho_vector[i] = data[0].input_vector[i];
	}
	
	// go through gram-schmidt process
	for (int i = 1; i < n; ++i) {
		// allocate memory for the projection vectors
		struct gram_schmidt *project = gs_malloc(m, i);
		if (project == NULL) {
			free_vector(data, n);
			mem_failure();
		}
		for (int j = 0; j < i; ++j) {
			// find where the projected vector will end up (scalar factor)
			double num = 0.0, denum = 0.0;
			for (int k = 0; k < m; ++k) {
				num += data[j].ortho_vector[k] * data[i].input_vector[k];
				denum += data[j].ortho_vector[k] * data[j].ortho_vector[k];
			}
			// if denum zero, have a zero vector meaning vectors not independent 
			if (denum == 0) {
				fprintf(stderr, "\nDivide by zero in find_ortho_vectors. Terminating program.\n");
				free_vector(data, n); free_gs(project, i);
				FAILURE;
			}
			double result = num / denum;

			// find the projection vector
			for (int k = 0; k < m; ++k) {
				project[j].gs_vector[k] = data[j].ortho_vector[k] * result;
			}

			// find the orthogonal vector (the error vector)
			if (j == 0) {
				for (int k = 0; k < m; ++k) {
					data[i].ortho_vector[k] = data[i].input_vector[k] - project[j].gs_vector[k];
				}
			}
			else {
				for (int k = 0; k < m; ++k) {
					data[i].ortho_vector[k] = data[i].ortho_vector[k] - project[j].gs_vector[k];
				}
			}
		}
		// free up space so can reuse, if applicable 
		free_gs(project, i);
	}
}

void print_results(struct vectors *data, int m, int n)
{
	// see if have zero row 
	for (int i = 0; i < n; ++i) {
		int count = 0;
		for (int j = 0; j < m; ++j) {
			if ((abs(data[i].ortho_vector[j])) == 0) {
				++count;
			}
		}
		if (count == m) {
			fprintf(stderr, "\nInputted vectors are not independent. Terminating program.\n");
			free_vector(data, n);
			FAILURE;
		}
	}

	// print the results 
	printf("\nThe orthonormal vectors are\n");
	for (int i = 0; i < n; ++i) {
		// find the lenth of the vector
		double unit = 0.0;
		for (int j = 0; j < n; ++j) {
			unit += data[i].ortho_vector[j] * data[i].ortho_vector[j];
		}
		// multiply vector by this factor to obtain the orthonormal vector
		unit = 1 / (sqrt(unit));

		// print the orthonormal vector
		printf("q%d=%0.2lf*(", i + 1, unit);
		for (int j = 0; j < m; ++j) {
			if (j != m - 1) {
				printf("%lf, ", data[i].ortho_vector[j]);
			}
			else {
				printf("%lf)\n", data[i].ortho_vector[j]);
			}
		}
	}
}

void free_vector(struct vectors *data, int n)
{
	// incase deallocation has already been done
	if (data == NULL) {
		return;
	}

	// free memory
	for (int i = 0; i < n; ++i) {
		free(data[i].input_vector);
		free(data[i].ortho_vector);
	}

	free(data);
}

void free_gs(struct gram_schmidt *project, int count)
{
	// incase deallocation has already been done
	if (project == NULL) {
		return;
	}

	// free memory
	for (int i = 0; i < count; ++i) {
		free(project[i].gs_vector);
	}

	free(project);
}
