// Libraries
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Function Prototypes
void scanCheck(int);
void allocCheck(void*);

int * somme_distinte(const int*, int, int*);
int inArray(const int*, int, int);
void printArray(const int*, int);
void fillArray(int*, int, int, int);



// MAIN
int main(int argc, char ** argv) {
    // Array Input
    int n;
    printf("Input the array size: ");
    int e = scanf("%d", &n);

    scanCheck(e);

    if (n <= 0) {
        puts("Error: Size must be positive.");
        exit(1);
    }

    int * arr = malloc(n * sizeof(int));
    allocCheck(arr);

    for (int i = 0; i < n; i++) {
        printf("Input the %d element of the array: ", i + 1);
        e = scanf("%d", &arr[i]);
        scanCheck(e);
    }

    // Calculate the result (resSize will hold res[] size)
    int resSize;
    int * res = somme_distinte(arr, n, &resSize);
    
    // Print Solution
    printArray(res, resSize);

    // Calculate the sum of result[] elements and print it
    int sum = 0;

    for (int i = 0; i < resSize; i++) sum += res[i];

    printf("Somma dei valori dell'array risultante: %d\n", sum);

    // Free the memory
    free(arr);
    free(res);

    return 0;
}



/********************************************************************************
 * @brief Calculates the distinct sums of pairs in an array
 * 
 * This function takes an array and computes an array of distinct
 * sums by summing elements pairwise.
 * 
 * @param a: Input array
 * @param n: Input array's size
 * @param size: Pointer to an integer that will store the final result size
 * 
 * @return Pointer to the dynamically allocated result array of distinct sums
 ********************************************************************************/

int * somme_distinte(const int * a, int n, int * size) {
    // Initializes result array with len(a) as initial size
    int curSize = n;
    int * res = malloc(n * sizeof(int));
    allocCheck(res);

    // Keeps track of elements inside res[]
    int ctr = 0;

    for (int i = 0; i < n; i++) {
        // Adds element * 2 to res[]
        if (!inArray(res, ctr, a[i] * 2)) {
            res[ctr++] = a[i] * 2;

            if (ctr == curSize) {
                curSize *= 1.5;
                res = realloc(res, curSize * sizeof(int));
                allocCheck(res);
            }
        }

        // Adds element + element ... to res[]
        for (int j = i + 1; j < n; j++) {
            // If the elements are the same, sum = 2 * element (handled by previous loop)
            if (a[i] == a[j]) continue;

            if (!inArray(res, ctr, a[i] + a[j])) {
                res[ctr++] = a[i] + a[j];

                if (ctr == curSize) {
                    curSize *= 1.5;
                    res = realloc(res, curSize * sizeof(int));
                    allocCheck(res);
                }
            }
        }
    }

    // Cuts allocated memory to final size
    res = realloc(res, ctr * sizeof(int));
    allocCheck(res);

    // Return result array and its size
    *size = ctr;
    return res;
}



/********************************************************************************
 * printArray: Prints an array on stderr
 * 
 * @param arr: Array
 * @param size: Array's size
 ********************************************************************************/

void printArray(const int * arr, int size) {
    for (int i = 0; i < size; i++) fprintf(stderr, "%d\n", arr[i]);
}



/********************************************************************************
 * inArray: Checks if an element is in an array
 * 
 * @param arr: Array
 * @param size: Array's size
 * @param n: Element to search for
 * 
 * @return 1 if n is in the array, 0 if not
 ********************************************************************************/

int inArray(const int * arr, int size, int n) {
    for (int i = 0; i < size; i++) if (arr[i] == n) return 1;
    return 0;
}



/********************************************************************************
 * scanCheck: Checks scanf's success
 * 
 * @param e: scanf return value
 ********************************************************************************/

void scanCheck(int e) {
    if (e != 1) {
        puts("Error: Invalid Input.");
        exit(2);
    }
}



/********************************************************************************
 * allocCheck: Checks memory allocation's success
 * 
 * @param p: Alloc operation's return value
 ********************************************************************************/

void allocCheck(void * p) {
    if (p == NULL) {
        puts("Error: Memory Allocation Failed.");
        exit(3);
    }
}