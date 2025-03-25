#!usr/bin/env python3

import os, time, sys


# Miofile class
class Miofile:
    def __init__(self, path): # Absolute path
        self.path = path
        self.linkPath = ''
        self.lastEditTime = time.strftime("%Y-%m-%d", time.localtime(os.path.getmtime(path)))
    
    def __str__(self):
        return f"{self.path} {self.linkPath}"



def listFromDir(dirName):
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

        # Symbolic link case
        if os.path.islink(completeName):
            continue
        
        # File case
        elif os.path.isfile(completeName):
            # Check if accessible
            if not os.access(completeName, os.F_OK):
                print(f"Error: Unable to access file {completeName}")
                continue

            files.append(Miofile(os.path.abspath(completeName))) # File -> append to list

        # Directory case
        elif os.path.isdir(completeName):
            # Check if accessible
            if not os.access(completeName, os.F_OK):
                print(f"Error: Unable to access the directory {completeName}")
                continue

            # Explore the directory
            realP = os.path.realpath(completeName)
            files += listFromDir(realP)
        
    return files



def createSubDirs(main, list, dest):
    """
    Creates dest/* subdirectories and links and adds the links path to the Miofile objects
    """

    # Move to dest directory
    os.chdir(os.path.join(main, dest))

    # Create dictionary to keep track of created files
    created = {}
    
    for file in list:
        name = os.path.basename(file.path).lower() # Get file name
        subDir = file.lastEditTime # Get subdirectory name

        # If the subdirectory doesn't exist create it
        if not os.path.exists(subDir):
            os.mkdir(subDir)

        # Initialize link name
        linkName = name
        
        # Change directory to the subdirectory
        os.chdir(subDir)

        if name not in created:
            created[name] = 1 # Create next index
        else:
            nameWOExt, ext = os.path.splitext(name) # Extract name and extension

            linkName = f"{nameWOExt}.{created[name]}{ext}" # Update link name
            created[name] += 1 # Increment index
        
        # DEBUG ####################################################
        print(f"\nCreando link simbolico:\n\tNome link: {linkName}\n\tPath link: {os.path.abspath(linkName)}\n\tPath file: {file.path}\n")
        ############################################################

        os.symlink(file.path, linkName) # Create the link
        file.linkPath = os.path.abspath(linkName) # Save link path

        # Move back to dest directory
        os.chdir("..")



def main(src, dest):
    # Check for correct input
    if not os.path.isdir(src):
        print(f"Error: {src} is not a directory.")
        return

    if not os.access(src, os.F_OK):
        print(f"Error: Unable to access the directory {src}")
        return

    # Check if dest already exists
    if os.path.exists(dest):
        print(f"Error: Directory {dest} already exists.")
        sys.exit(1)
    
    # Save start time
    totalTime = time.time()

    # Get main directory
    mainDir = os.getcwd()
    
    # Create dest directory
    os.mkdir(dest)

    # Get file list
    fileList = listFromDir(src)
    
    # Create subdirectories and symbolic links
    createSubDirs(mainDir, fileList, dest)
            
    # Print process result
    for file in fileList:
        print(file)
    
    # Calculate and print elapsed time
    totalTime = time.time() - totalTime
    print(f"The total execution time is {totalTime} seconds.")



# Start the program
if len(sys.argv) != 3:
    print(f"Error: Incorrect Usage.\nUsage -> {sys.argv[0]} <src> <dest>")
else:
    main(sys.argv[1], sys.argv[2])