#!/usr/bin/env python
# Whitespace checks

import os
import glob
import sys

if sys.version_info < (2,7):
    print >> sys.stderr, "must be run with at least Python 2.7"
    sys.exit(1)

# Absolute path to directory of Python script
scriptDirectory = os.path.dirname(os.path.realpath(__file__))

# Root directory of the Git repository
repositoryDirectory = os.path.realpath(os.path.join(scriptDirectory, ".."))

sourceHeader = ""
with open(os.path.join(repositoryDirectory, "share", "source-header.txt")) as sourceHeaderFile:
    sourceHeader = sourceHeaderFile.read()

# File names and extensions to ignore
ignoredFiles = [".gitmodules", "CMakeCache.txt", "COPYING"]
ignoredExtensions = ["nib", "plist", "strings"]
licenseIgnoredFiles = ["ExpatXMLParser.cpp", "ExpatXMLParser.h", "cairosdl.c", "cairosdl.h"]

# http://stackoverflow.com/a/3002505/343845
def is_binary(filename):
    """Return true if the given filename is binary.
    @raise EnvironmentError: if the file does not exist or cannot be accessed.
    @attention: found @ http://bytes.com/topic/python/answers/21222-determine-file-type-binary-text on 6/08/2010
    @author: Trent Mick <TrentM@ActiveState.com>
    @author: Jorge Orpinel <jorge@orpinel.com>"""
    fin = open(filename, 'rb')
    try:
        CHUNKSIZE = 1024
        while 1:
            chunk = fin.read(CHUNKSIZE)
            if '\0' in chunk: # found null byte
                return True
            if len(chunk) < CHUNKSIZE:
                break # done
    finally:
        fin.close()

    return False

# Converts an absolute path to a path relative to
# the root directory of the project's Git repository
def toRepositoryRelativePath(path, gitDirectory):
    if path.startswith(gitDirectory):
        return "." + path[len(gitDirectory):]
    else:
        raise Exception("Path given to toRepositoryRelativePath did not refer to a path within the repository")

# Finds problematic character sequences in the given
# string and returns an array of string messages noting
# the problems that were found
def findProblemSequences(wholeFile, line, file, lineNumber, totalLines):
    problems = []

    if lineNumber == 1 and line.startswith("\xEF\xBB\xBF"):
        problems.append("File contains UTF-8 BOM; skipping additional checks for this file")
        return problems

    if lineNumber == 1 and (line.startswith(" ") or line.startswith("\t") or line.startswith("\n")):
        problems.append("Leading whitespace in file")

    if lineNumber == totalLines and not line.endswith("\n"):
        problems.append("File did not end with a newline")

    # We check for lineNumber == 1 here just so this message appears
    # only once per file... kind of a hack, but at least it's a safe one
    if lineNumber == 1 and wholeFile.endswith("\n\n"):
        problems.append("File contained multiple ending newlines")

    if "\r" in line:
        problems.append("Windows carriage return")

    if "\t" in line:
        problems.append("Tab characters")

    if line.endswith(" ") or line.endswith(" \n"):
        problems.append("Trailing whitespace")

    if lineNumber == 1 and \
        (file.endswith(".h") or \
        file.endswith(".c") or \
        file.endswith(".cpp") or \
        file.endswith(".m") or \
        file.endswith(".mm")) and not os.path.basename(file) in licenseIgnoredFiles and not wholeFile.startswith(sourceHeader):
        problems.append("C-language source code file does not contain standard header")

    return problems

# Build a list of all files to check
def buildInspectionFileList(directory):
    fileList = []
    for root, subFolders, files in os.walk(directory):
        if ".git" in subFolders:
            subFolders.remove(".git")

        for file in files:
            absFile = os.path.realpath(os.path.join(root, file))

            # Only add the file if it's not in our ignore lists
            _, ext = os.path.splitext(file)
            if (not ext[1:] in ignoredExtensions) and (not file in ignoredFiles) and (not is_binary(absFile)):
                fileList.append(absFile)

    return fileList

def runComplianceCheck(directory):
    hasProblems = False
    fileList = buildInspectionFileList(directory)

    # Go through each file for potential issues
    for file in fileList:
        with open(file) as f:
            lines = f.readlines()

            # Check each line in the file for gremlins
            for counter, line in enumerate(lines):
                lineNumber = counter + 1

                # If the line contained gremlins, show details
                problems = findProblemSequences(''.join(lines), line, file, lineNumber, len(lines))
                if problems:
                    hasProblems = True
                    print >> sys.stderr, toRepositoryRelativePath(file, directory) + ":" + str(lineNumber)
                    for problem in problems:
                        print >> sys.stderr, "\t" + problem

            f.close()

    return hasProblems

if not runComplianceCheck(repositoryDirectory):
    print "No problems found!"
else:
    sys.exit(1)
