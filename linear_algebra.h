/* File: linear_algebra.h
 * Author: Steve Ienciu
 * Date: January 19th, 2024
 * Description: Header file for functions that will be used often in linear algebra
 */

#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#define FAILURE exit(1)

/* Function: mem_failure
 * Description: provides error message if cannot allocate memory
 * Parameters: null
 */
void mem_failure(void);

/* Function: matrix_mem_alloc
 * Description: allocates memory for a matrix
 * Parameters:
 *	- m: number of rows in given vector
 *	- n: number of columns in given vector
 * Returns: address of the first element of the matrix
 */
double **matrix_mem_alloc(int m, int n);

/* Function: matrix_mem_realloc
 * Description: reallocates memory for a matrix
 * Paramters:
 *	- m_new: new number of rows for matrix
 *	- n_new: new number of columns for matrix
 * Returns: address of the first element of the new matrix
 */
double **matrix_mem_realloc(double **old_matrix, int m_new, int n_new);

/* Function: user_input_matrix
 * Description: takes input from matrix to fill matrix
 * Parameters:
 *	- matrix: matrix being dealt with 
 *  - m: number of rows of given matrix
 * 	- n: number of columns of given matrix
 */
void user_input_matrix(double **matrix, int m, int n);

/* Function: swap_row
 * Description: swaps rows of matrix to ensure have nonzero pivot; if applicable
 * Parameters:
 * 	- matrix: matrix being dealt with
 *	- m: number of row in given matrix
 *	- n: number of columns in given matrix
 */
void swap_row(double **matrix, int m, int n);

/* Function: is_inverse
 * Description: checks to see if have a full column rank and full row rank; only applicable to square matrices
 * Parameters:
 *	- matrix: matrix being dealt with 
 *	- m: number of rows of given matrix which also equal to number of columns
 * Return: if given matrix has an inverse
 */
bool is_inverse(double **matrix, int m);

/*
 * Function: switch_pivot_column
 * Description: switches column pivot to nonzero value
 * Parameters: 
 *	- matrix: matrix being dealt with 
 *	- row_index: current row being manipulated
 * 	- column_index: current column being checked for nonzero element
 *	- n2: max number of pivot columns
 */
void switch_pivot_column(double **matrix, int row_index, int *column_index, int n2);

/*
 * Function: gauss_elimination
 * Description: performs Gauss elimination on given matrix
 * Parameters:
 *	- matrix: matrix being dealt with 
 *	- m: number of rows of given matrix
 *	- n1: number of column of given matrix
 *	- n2: subset of n1, will typically be the same value as n2
 */
void gauss_elimination(double **matrix, int m, int n1, int n2);

/* Function: jordan_elimination
 * Description: performs Jordan elimination on given matrix
 * Parameters: 
 *	- matrix: matrix being dealt with 
 *	- m: number of rows of given matrix
 *	- n1: number of columns of given matrix
 *	- n2: subset of n1, will rtpically be the same value as n2
 * 	- flag: set to 1 if want to divide the row by its pivot or 0 if want to leave as is
 */
void jordan_elimination(double **matrix, int m, int n1, int n2, int flag);

/* Function: free_matrix
 * Description: deallocates memory of matrix
 * Parameters:
 *	- matrix: matrix being dealt with 
 *  - m: number of rows of given matrix
 */
void free_matrix(double **matrix, int m);

#endif
