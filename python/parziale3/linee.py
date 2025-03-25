#!/usr/bin/env python3

import sys, os, os.path


# Miofile class
class Miofile:
    def __init__(self, path, lines): # Absolute path
        self.path = path
        self.lines = lines
        self.patternIndex = 0
    
    def __lt__(self, other):
        if self.lines[self.patternIndex] == other.lines[other.patternIndex]:
            return self.path < other.path
        
        return self.lines[self.patternIndex] < other.lines[other.patternIndex]

    def __str__(self):
        resultString = f"### File: {self.path}\n"

        for line in self.lines:
            resultString += f"{line}"
        
        return resultString



def listFromDir(dirName, explored, num):
    """
    Returns a list of Miofile objects found inside dirName
    (Only called on not yet explored directories)
    """

    # Initialize result list
    files = []

    # Check all files inside dirName
    for path in os.listdir(dirName):
        # Create absolute path
        absPath = os.path.abspath(os.path.join(dirName, path))

        # File case
        if os.path.isfile(absPath):
            # Check if accessible
            if not os.access(absPath, os.F_OK):
                print(f"Error: Unable to access file {absPath}")
                continue
            
            lines = [] # Miofile.lines attribute
            i = 0 # Initialize counter

            with open(absPath, "r") as f:
                for line in f:
                    # If exceeded quit                 
                    if i >= num:
                        break
                    
                    i += 1 # Increment counter
                    lines.append(line) # Save line

            files.append(Miofile(absPath, lines)) # Append to list

        # Directory case
        elif os.path.isdir(absPath):
            # Check if accessible
            if not os.access(absPath, os.F_OK):
                print(f"Error: Unable to access the directory {absPath}")
                continue

            # Check if already explored, if not explore
            if absPath in explored:
                continue
            else:
                explored.append(absPath) # Update explored list
                files += listFromDir(absPath, explored, num) # Append subdirectory files
    
    return files



def filterFiles(list, pattern):
    """
    Returns the filtered list with only files that contain the pattern in a line
    """

    result = [] # Initialize result array

    for file in list:
        # For each line saved check if it contains the pattern
        for i in range(len(file.lines)):
            if pattern in file.lines[i]:
                file.patternIndex = i # Save line index
                result.append(file) # Append the file
                break
        
    return result



def main(dir, num, pattern):
    # Check if the directory passed is correct
    if not os.path.exists(dir):
        print(f"Error: {dir} doesn't exist.")
        sys.exit(1)
    elif not os.path.isdir(dir):
        print(f"Error: {dir} isn't a directory.")
        sys.exit(1)

    # Cast the passed number
    num = int(num)

    # Get file list
    tutti = listFromDir(dir, [], num)

    # Filter the list and sort it
    pochi = filterFiles(tutti, pattern)
    pochi.sort()

    # Print the files
    for file in pochi:
        print(file)



# Start the program
if len(sys.argv) != 4:
    print(f"Error: Incorrect Usage.\nUsage -> {sys.argv[0]} <dir> <num> <pattern>")
else:
    main(sys.argv[1], sys.argv[2], sys.argv[3])