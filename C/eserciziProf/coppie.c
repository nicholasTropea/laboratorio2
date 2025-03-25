#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


// Global prototypes
void terminate(const char*);


// Couple struct and prototypes
typedef struct {
  int first;
  int second;
} couple;

int compareCouples(const void*, const void*);
void printCouples(const couple**, int);
void deleteCouples(couple**, int);

// Main
int main(int argc, char ** argv) {
    // Check if the input is correct, otherwise throw an error and show a usage message
    if (argc < 3 || argc % 2 == 0) {
        fprintf(stderr, "Error: You must input an even number of integers\nUsage: %s <int1> ... <int2n>\n", argv[0]);
        exit(1);
    }

    // Create the result array
    int arrSize = (argc - 1) / 2;
    couple ** arr = malloc(arrSize * sizeof(couple*));
    if (NULL == arr) terminate("Error: Malloc failed.\n");

    // Add the couples to the array
    for (int i = 0; i < arrSize; i++) {
        // Current couple element
        couple * el = malloc(sizeof(couple));
        if (NULL == el) terminate("Error: Malloc failed.\n");

        // Create the current couple
        el -> first = atoi(argv[(i * 2) + 1]);
        el -> second = atoi(argv[(i * 2) + 2]);

        // Push the couple to the array
        arr[i] = el;
    }

    // Sort the array
    qsort(arr, arrSize, sizeof(couple*), &compareCouples);

    // Print the array
    printCouples((const couple**)arr, arrSize);

    // Deallocate the array
    deleteCouples(arr, arrSize);

    return 0;
}


// Global functions' signatures
void terminate(const char * msg) {
    if (errno != 0) perror(msg);
    else fprintf(stderr, "%s\n", msg);
    
    exit(1);
}


// Couple functions' signatures

// Compares 2 couples. Return = 1 if first > second, 0 if equal, -1 if first < second
int compareCouples(const void * a, const void * b) { // Void* is used to get rid of qsort() warnings
    const couple * c1 = *(const couple**) a;
    const couple * c2 = *(const couple**) b;

    int c1Diff = c1 -> first - c1 -> second; 
    int c2Diff = c2 -> first - c2 -> second; 

    return c1Diff - c2Diff;
}

// Prints the couples array
void printCouples(const couple ** arr, int size) {
    printf("Array:\n\t");

    for (int i = 0; i < size; i++)
        printf("(%d %d) ", arr[i] -> first, arr[i] -> second);
    
    printf("\n");
}

// Deallocates the array
void deleteCouples(couple ** arr, int size) {
    for (int i = 0; i < size; i++) free(arr[i]);

    free(arr);
    arr = NULL;
}