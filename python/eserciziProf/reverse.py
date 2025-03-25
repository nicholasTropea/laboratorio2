#! /usr/bin/env python3

import sys

def main(fileName):
    # Open the original file
    with open(fileName, "r") as original:
        newFileName = f"{fileName}.rev"

        # Create the result file
        with open(newFileName, "w") as result:
            # Create an array with all the file lines
            lines = original.readlines()

            # If the last line didn't end with a \n add it
            if lines[-1][-1] != "\n":
                lines[-1] = lines[-1] + "\n"

            # Iterate through the lines backwards
            for line in reversed(lines):
                # Print the current line to the file
                print(line, file=result, end="") # readlines() keeps the \n, so we don't need it here


# Start the file or print an error
if len(sys.argv) == 2:
    main(sys.argv[1])
else:
    print(f"Errore nell'uso del programma.\nUso corretto: {sys.argv[0]} fileTesto")