/* this program projects one line onto another line */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_algebra.h"

void user_input_vector(double *vector, const int m);
void find_projected_vector(double *vectora, double *vectorb, const int m, double *num, double *denum, double *x);

int main()
{
	double *vectora, *vectorb, x = 0.0, num = 0.0, denum = 0.0;
	int m;

	// output instructions for user to stdout
	printf("\nThis program will project one line onto another line.\n");

	// get dimension of vector
	printf("\nPlease enter the number of rows of the vector: ");
	scanf("%d", &m);

	// ensure have valid dimension
	if (m < 2) {
		fprintf(stderr, "\nImproper vector dimension on line 22 in main.\n");
		FAILURE;
	}

	// clear input buffer 
	clear_buffer();

	// allocate space for the vector to be projected on
	vectora = malloc(m * sizeof(int));
	if (vectora == NULL) {
		mem_failure();
	}

	// allocate space for the vector to be projected
	vectorb = malloc(m * sizeof(int));
	if (vectorb == NULL) {
		free(vectora);
		mem_failure();
	}

	// gather inputs for the first vector
	printf("\nPlease enter the elements of the %d-D vector to be projected on\n", m);
	user_input_vector(vectora, m);

	// clear input buffer
	clear_buffer();

	// gather inputs for the second vector
	printf("Please enter the elements if the %d-D vector to project:\n", m);
	user_input_vector(vectorb, m);

	find_projected_vector(vectora, vectorb, m, &num, &denum, &x);

	return 0;
}

void user_input_vector(double *vector, int m)
{
	for (int i = 0; i < m; ++i) {
		scanf("%lf", &vector[i]);
	}
}

void find_projected_vector(double *vectora, double *vectorb, const int m, double *num, double *denum, double *x)
{
	// find how much vector to be projected on needs to be scaled
	for (int i = 0; i < m; ++i) {
		*num += vectora[i] * vectorb[i];
		*denum += vectora[i] * vectora[i];
	}
	if (*denum <= 0) {
		fprintf(stderr, "\nThe vector to be projected on has an invalid length on line 76 in find_projected_vector. Terminating program.\n");
		free(vectora); free(vectorb);
		FAILURE;
	}
	*x = *num / *denum;

	// find the projection vector 
	for (int i = 0; i < m; ++i) {
		vectora[i] = *x * vectora[i];
	}

	//output the porjection vector
	printf("\nThe projected vector is (");
	for (int i = 0; i < m; ++i) {
		printf("%0.2lf", vectora[i]);
		if (i != m - 1) {
			printf(", ");
		}
	}
	printf(")\n");
}
