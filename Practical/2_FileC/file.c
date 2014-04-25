#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "resource_detector.h"


/*
 * Normally I'd make ReadElement() and WriteElement() static, as they are only needed
 * inside this file and exposing them to the outside world is a risk.
 * For this exercise I've decided to make these functions public, so they can be properly
 * tested using unit tests. So you can make sure these functions are properly implemented
 */
int ReadElement(FILE* FilePtr, int ElementNr, STUDENT* StudentPtr)
{
    int Result = 0;
    
    // TODO: implement
    
    return Result;
}

int WriteElement(FILE* FilePtr, int ElementNr, const STUDENT* StudentPtr)
{
    int Result = 0;
    
    // TODO: implement

    return Result;
}

int
ComputeAverageStudyResults (char* FileName, double* Average)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}


int
LinearSearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}

int
BinarySearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}

int 
AddStudentSortedToFile (char* FileName, STUDENT* StudentPtr)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}


int
RemoveStudentFromFile (char* FileName, int StudentNumber)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}

