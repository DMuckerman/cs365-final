// Matrix Vector Multiplication in Parallel
// Spring 2015
// Daniel Muckerman

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

typedef enum {false, true} bool;

#define num_threads 8

void print_array(int arr[], int n);
void mat_vec(int mat[], int vec[], int n);

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
	struct Work *work = p;

	for (int i = work->start; i < work->end; i++) {
		for(int j = 0; j < work->n; j++) {
			work->arr[i] = work->arr[i] + (work->mat[(i * work->n) + j] * work->vec[j]);
		}
	}

	return NULL;
}

int main(void) {

	unsigned int mat[1577536];
	unsigned int vec[1256];
	int n = 1256;
	unsigned int arr[1256];

	// Fill random matrix
	printf("Reticulating splines...");
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			mat[(i * n) + j] = rand() % 1000;
		}
		vec[i] = rand() % 1000;
	}
	printf("done\n");

	// Initialize structs for 2 threads
	struct Work *w = malloc(num_threads * sizeof(struct Work));
	for(int i = 0; i < num_threads; i++) {
		w[i].mat = mat;
		w[i].vec = vec;
		w[i].n = n;
		w[i].start = i * (n / num_threads);
		w[i].end = (i + 1) * (n / num_threads);
	}

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
