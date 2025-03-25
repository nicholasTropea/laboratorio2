#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "terne.h"

// Function Prototypes
void process_file(const char *filename, terna **root);
int parse_line(const char *line, int *a, int *b, int *c);
void write_pitagorean_terns(const char *filename, terna *root);

// Main Function
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename> [s1 s2 ... sk]\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    terna *root = NULL;

    // Process the file to build the ABR
    process_file(filename, &root);

    // Print the number of nodes in the ABR
    printf("%d\n", abr_conta(root));

    // Process sum queries
    for (int i = 2; i < argc; i++) {
        int si = atoi(argv[i]);
        printf("%d\n", abr_conta_si(root, si));
    }

    // Write pitagorean terns to a new file
    write_pitagorean_terns(filename, root);

    // Cleanup
    // Assuming there would be a function to deallocate the entire ABR
    // For now, it's not implemented, so just free root manually if necessary.

    return 0;
}

// Function to process the input file and populate the ABR
void process_file(const char *filename, terna **root) {
    FILE *file = fopen(filename, "r");
    if (!file) termina("Error opening file");

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1) {
        int a, b, c;
        if (parse_line(line, &a, &b, &c)) {
            terna *t = terna_crea(a, b, c);
            *root = abr_inserisci(*root, t);
        }
    }

    free(line);
    fclose(file);
}

// Function to parse a line into a tern
int parse_line(const char *line, int *a, int *b, int *c) {
    int counts[3] = {0, 0, 0};
    int index = 0;

    // Check for invalid characters
    for (const char *p = line; *p; p++) {
        if (*p == '-') {
            if (index < 3) counts[index]++;
        } else if (*p == ';' || *p == ':') {
            if (index < 3 && counts[index] > 0) index++;
        } else if (*p != '\n') {
            return 0; // Invalid character found
        }
    }

    // Verify that we have exactly three counts
    if (index != 2 || counts[2] == 0) return 0;

    *a = counts[0];
    *b = counts[1];
    *c = counts[2];
    return 1;
}

// Function to write pitagorean terns to a file
void write_pitagorean_terns(const char *filename, terna *root) {
    char *output_filename;
    if (asprintf(&output_filename, "%s.pit", filename) < 0) termina("Error allocating filename");

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) termina("Error opening output file");

    // Print only the pitagorean terns in in-order traversal
    abr_stampa_pit_inorder(root, output_file);

    fclose(output_file);
    free(output_filename);
}
