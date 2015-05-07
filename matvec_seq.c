// Matrix Vector Multiplication
// Spring 2015
// Daniel Muckerman

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mat_vec(unsigned int mat[], unsigned int vec[], int n);
void print_array(unsigned int arr[], int n);

int main(void) {

	unsigned int mat[1577536];
	unsigned int vec[1256];
	int n = 1256;

	// Fill random matrix
	printf("Reticulating splines...");
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			mat[(i * n) + j] = rand() % 100;
		}
		vec[i] = rand() % 100;
	}
	printf("done\n");

	mat_vec(mat, vec, n);

	return 0;
}

// Calculate the resultant vector
// Based on the passed in matrix and vector of size n
void mat_vec(unsigned int mat[], unsigned int vec[], int n) {

	unsigned int arr[1256];

	// For every row in the array
	for (int i = 0; i < n; i++) {
		// For every column in the row
		for(int j = 0; j < n; j++) {
			// Update result vector with results of calculation
			arr[i] = arr[i] + (mat[(i * n) + j] * vec[j]);
		}
	}

	print_array(arr, n);
}

// Print the array passed to the function
void print_array(unsigned int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%u ", arr[i]);
	}
	printf("\n");
}
