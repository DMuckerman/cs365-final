// Matrix Vector Multiplication in Parallel
// Spring 2015
// Daniel Muckerman

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

typedef enum {false, true} bool;

#define num_threads 2

void print_array(int arr[], int n);

// Struct for data for each thread
struct Work {
	unsigned int arr[1256];
	unsigned int *mat;
	unsigned int *vec;
	int n;
	int start;
	int end;
};

// Calculate the resultant vector
// Based on the passed in matrix and vector of size n
void *worker(void *p) {
	// Initialized Work struct from passed in pointer
	struct Work *work = p;

	// For every row in the chunk
	for (int i = work->start; i < work->end; i++) {
		// For every column in the row
		for(int j = 0; j < work->n; j++) {
			// Update result vector with results of calculation
			work->arr[i] = work->arr[i] + (work->mat[(i * work->n) + j] * work->vec[j]);
		}
	}

	return NULL;
}

int main(void) {

	// Declare necessary variables
	unsigned int mat[1577536];
	unsigned int vec[1256];
	int n = 16;
	unsigned int arr[1256];
	clock_t begin, end;
	double time_spent;

	// Run through the various trials
	while (n < 2048) {

		// Clear average runtime for current size
		double trials = 0;

		// Run ten trials
		for(int k = 0; k < 10; k++) {
			// Fill random matrix
			//printf("Reticulating splines...");
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					mat[(i * n) + j] = rand() % 1000;
				}
				vec[i] = rand() % 1000;
			}
			//printf("done\n");

			// Initialize structs for 2 threads
			struct Work *w = malloc(num_threads * sizeof(struct Work));
			for(int i = 0; i < num_threads; i++) {
				w[i].mat = mat;
				w[i].vec = vec;
				w[i].n = n;
				w[i].start = i * (n / num_threads);
				w[i].end = (i + 1) * (n / num_threads);
			}

			// Ensure last thread covers remainder of matrix
			if (w[num_threads].end < n) {
				w[num_threads].end = n;
			}

			// Start the clock
			begin = clock();

			// Initialize 2 threads and run the worker function on them
			pthread_t thr[num_threads];
			for (int i = 0; i < num_threads; i++) {
				pthread_create(&thr[i], NULL, worker, &w[i]);
			}

			// Wait for threads to finish
			for (int i = 0; i < num_threads; i++) {
				pthread_join(thr[i], NULL);
				//printf("Thread %i finished\n", i);
			}

			// Combine results
			for (int i = 0; i < num_threads; i++) {
				for (int j = 0; j < (n / num_threads); j++) {
					arr[(i * (n / num_threads)) + j] += w[i].arr[(i * (n / num_threads)) + j];
				}
			}

			// Print resultant vector
			//print_array(arr, n);

			// TIME!
			end = clock();
			time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			trials += time_spent;

			/*
			// Check results
			unsigned int temp[1256];

			// For every row in the array
			for (int i = 0; i < n; i++) {
				// For every column in the row
				for(int j = 0; j < n; j++) {
					// Update result vector with results of calculation
					temp[i] = temp[i] + (mat[(i * n) + j] * vec[j]);
				}
			}

			bool same = true;
			for (int i = 0; i < n; i++) {
				if (temp[i] != arr[i]) {
					same = false;
				}
			}

			if (same) {
				printf("Passed! Parallel matches sequential!\n");
			} else {
				printf("Failed... Parallel and sequential differ\n");
			}*/
		}

		printf("%i - %f\n", n, trials / 10.0);

		// Double n for next iteration
		n *= 2;
		if (n == 2048) {
			n = 1256;
		}
	}

	return 0;
}

// Print the array passed to the function
void print_array(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%u ", arr[i]);
	}
	printf("\n");
}
