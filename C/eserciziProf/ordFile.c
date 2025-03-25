#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


// Prototypes
void terminate(const char*);
int compareFiles(const void*, const void*);

// Main
int main(int argc, char ** argv) {
    // Input check
    if (argc < 2) {
        fprintf(stderr, "Error: Incorrect usage.\nUsage: %s <file1> ... <fileN>\n", argv[0]);
        exit(1);
    }

    // Array size
    int arrSize = argc - 1;

    // Create file array
    FILE ** arr = malloc(arrSize * sizeof(FILE*));
    if (NULL == arr) terminate("Error: Malloc failed.\n");

    // Add files to the array
    for (int i = 1; i < argc; i++) {
        // Create current file
        FILE * f = fopen(argv[i], "r");
        if (NULL == f) terminate("Error: Malloc failed.\n");

        // Push the file
        arr[i - 1] = f;
    }


    
    return 0;
}


// Functions' signatures
void terminate(const char * msg) {
    if (errno != 0) perror(msg);
    else fprintf(stderr, "%s\n", msg);
    
    exit(1);
}


// Compares 2 files and returns integer sort value
int compareFiles(const void * a, const void * b) {
    // Files to compare
    FILE * f1 = *(const FILE**) a;
    FILE * f2 = *(const FILE**) b;

    // First file size
    int e = fseek(f1, 0L, SEEK_END);
    if (-1 == e) terminate("Error: fseek() failed.\n");
    int s1 = ftell(f1);

    // Second file size
    e = fseek(f2, 0L, SEEK_END);
    if (-1 == e) terminate("Error: fseek() failed.\n");    
    int s2 = ftell(f2);

    if (s1 != s2) return s1 - s2;
    else {
        // Sort by fileName
    }
}