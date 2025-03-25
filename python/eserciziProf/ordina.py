#! /usr/bin/env python3

import sys

def main(fileName):
    # Open original file
    with open(fileName, "r") as original:
        # Create new file with updated name
        newFileName = fileName + ".dec"

        with open(newFileName, "w") as result:
            # Create array containing original file lines
            lines = []

            for line in original:
                lines.append(line)
            
            # If last line doesn't end with \n add it
            if lines[-1][-1] != '\n':
                lines[-1] = lines[-1] + '\n'

            # Sort the lines array by line length in descending order
            lines.sort(key=lambda x: len(x), reverse=True)

            # Populate new file with the sorted lines
            for line in lines:
                print(line, file=result, end="")
            

# Start the program or print an error
if len(sys.argv) == 2:
    main(sys.argv[1])
else:
    print(f"Error: Incorrect Use\nUsage: {sys.argv[0]} fileName")