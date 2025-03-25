// Libraries
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Function Prototypes
int mioa2toi(const char*, int*);


// MAIN
int main(int argc, char ** argv) {


    return 0;
}



/********************************************************************************
 * @brief Emulates the atoi function
 * 
 * This function takes a string and converts it into an integer number,
 * it doesn't consider initial spaces, if a character has been found and it
 * finds a space, it considers the number to be ended.
 * 
 * @param s: String
 * @param n: Pointer to an integer that will store the converted value if successful
 * 
 * @return 0 if the convertion is successful
 *         1 if the string is empty or only contains spaces
 *         2 if the string contains a character apart from +-0123456789
 *         3 if the characters + and - are misused
 *         4 if the string contains just + or -
 ********************************************************************************/

int mioa2toi(const char * s, int * n) {
    // Lenght of s
    int len = strlen(s);

    // Empty string
    if (len == 0) return 1;

    char cur = s[0]; // Current character
    int started = 0; // 0 if only found spaces, 1 otherwise
    char finalString[len]; // String where the valid characters will end up into

    while (cur != '\0' && !(started && cur == ' ')) {
        // Invalid Character
        if ((cur <= 48 || cur >= 57 ) && cur != '-' && cur != '+') return 2;

        // + or - misused
        if (started && (cur == '+' || cur == '-')) return 3;

        // Updated when it finds the first valid character
        if (!started && cur != ' ') started = 1;

        



    }

    return 0;
}