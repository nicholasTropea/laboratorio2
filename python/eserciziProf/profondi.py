#! /usr/bin/env python3

import sys, os

class Miofile:
    def __init__(self, path, depth):
        self.path = path
        self.depth = depth
        self.size = os.path.getsize(path)
    
    def __lt__(self, other):
        if self.depth != other.depth:
            return self.depth > other.depth

        return self.size < other.size
    
    def __str__(self):
        return f"Path: {self.path}\tSize: {self.size}"



def main(dirName):
    """
    Explores dirName and its subdirectories and prints the files found at the lowest depth, ordered
    by ascending file size. 
    """

    # Check for correct input
    if not os.path.isdir(dirName):
        print(f"Error: {dirName} is not a directory.")
        return

    if not os.access(dirName, os.F_OK):
        print(f"Error: Unable to access the directory {dirName}")
        return

    # Create file list
    fileList = listFromDir(dirName, [dirName], 1)

    # Check for empty list
    if len(fileList) == 0:
        print(f"No files were found inside {dirName}")
        return

    # Sort the list and print the correct files
    fileList.sort()

    d = fileList[0].depth # Maximum depth

    print(f"Files found at lowest depth ({d}):\n")

    for file in fileList:
        if file.depth == d:
            print(f"\t{file}\n{120 * "-"}")
        else:
            break
    
    return



def listFromDir(dirName, explored, curDepth):
    """
    Returns a list of Miofile objects found inside dirName
    (Only called on not yet explored directories)
    """

    # Initialize result list
    files = []

    # Check all files inside dirName
    for path in os.listdir(dirName):
        # Create real path
        completeName = os.path.join(dirName, path)

        if os.path.isfile(completeName):
            # Check if accessible
            if not os.access(completeName, os.F_OK):
                print(f"Error: Unable to access file {completeName}")
                continue

            files.append(Miofile(completeName, curDepth)) # File -> append to list
        elif os.path.isdir(completeName):
            # Check if accessible
            if not os.access(completeName, os.F_OK):
                print(f"Error: Unable to access the directory {completeName}")
                continue

            # Check if already explored, if not explore
            realP = os.path.realpath(completeName)

            if realP in explored:
                continue
            else:
                explored.append(realP)
                files += listFromDir(realP, explored, curDepth + 1)
    
    return files
    


# Check command line parameters and invoke main function
if len(sys.argv) != 2:
    print(f"Error: Incorrect usage.\nUsage: {sys.argv[0]} [dirName]")
else:
    main(sys.argv[1])