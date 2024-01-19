// declares basic functions that will be used often 

#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#define FAILURE exit(1)

// spit out error if cannot allocate space 
void mem_failure(void);

// allocate space for matrix
void mem_alloc(double ***matrix, int m, int n);

// accept user input to fill the matrix
void user_input_matrix(double **matrix, int m, int n);

// swap rows to ensure have nonzero pivot; not always feasible 
void swap_row(double **matrix, int m, int n);

// perform gauss elimination on the matrix
void gauss_elimination(double **matrix, int m, int n);

// perform jordan elimination on the matrix
void jordan_elimination(double **matrix, int m, int n);

// deallocate memory matrix uses
void free_matrix(double **matrix, int m);

#endif
