#include <stdio.h>
#include <stdlib.h>

void sizeCheck(int);
void allocCheck(void *);

int main(int argc, char ** argv) {
    int n;

    printf("Input an integer: ");
    int e = scanf("%d", &n);

    sizeCheck(e);

    int aIdx = 0, bIdx = 0;
    int aSum = 0, bSum = 0;

    int aMaxSize = (n / 4 > 0) ? n / 4 : 1;
    int bMaxSize = (n / 4 > 0) ? n / 4 : 1;

    int * a = malloc(sizeof(int) * aMaxSize);
    int * b = malloc(sizeof(int) * bMaxSize);

    allocCheck(a);
    allocCheck(b);

    for (int i = 3; i <= n; i++) {
        if (i % 5 != 0 && i % 3 == 0) {
            if (aIdx == aMaxSize) {
                aMaxSize *= 2;
                a = realloc(a, sizeof(int) * aMaxSize);
                allocCheck(a);
            }
            
            a[aIdx++] = i;
            aSum += i;
        }
        else if (i % 3 != 0 && i % 5 == 0) {
            if (bIdx == bMaxSize) {
                bMaxSize *= 2;
                b = realloc(b, sizeof(int) * bMaxSize);
                allocCheck(b);
            }

            b[bIdx++] = i;
            bSum += i;
        }
    }

    free(a);
    free(b);

    a = NULL;
    b = NULL;

    printf(
        "Length a[] = %d,  sum a[] = %d\n"
        "Length b[] = %d,  sum b[] = %d\n", aIdx, aSum, bIdx, bSum
    );
    
    return 0;
}

void sizeCheck(int e) {
    if (e != 1) {
        puts("Error: Invalid input.");
        exit(1);
    }
}

void allocCheck(void * ptr) {
    if (ptr == NULL) {
        puts("Error: Allocation failed.");
        exit(2);
    }
}