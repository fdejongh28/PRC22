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
    return Result;
}

int WriteElement(FILE* FilePtr, int ElementNr, const STUDENT* StudentPtr)
{
    int Result = 0;
    
    // TODO: implement

    return Result;
}

int ComputeAverageStudyResults (char* FileName, double* Average)
{
    int Result = 0;
    if (FileName == NULL || Average == NULL)
    {
        return -1;
    }

    FILE *file_ptr = fopen(FileName, "rb");
    if (file_ptr != NULL)
    {
        fseek(file_ptr, 0, SEEK_END);
        size_t total_size = ftell(file_ptr);
        size_t total_students = total_size / sizeof(STUDENT);
        rewind(file_ptr);

        int i = 0;
        int total = 0;
        for(; i < total_students; i++)
        {
            STUDENT student;
            if (fread(&student, sizeof(STUDENT), 1, file_ptr) != -1)
            {
                total += student.StudyCredits;
            }
            else
            {
                fclose(file_ptr);
                return -1;
            }
            
        }
        double avg = total / total_students;
        *Average = avg;
    }
    else
    {
        fclose(file_ptr);
        return -1;
    }
    fclose(file_ptr);
    return Result;
}


int LinearSearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}

int BinarySearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}

int AddStudentSortedToFile (char* FileName, STUDENT* StudentPtr)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}


int RemoveStudentFromFile (char* FileName, int StudentNumber)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}

