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

int CountStudents(char *file_name)
{
    int result = 0;
    FILE *file_ptr = NULL;
    long total_file_size = 0;
    long total_students = 0;

    // Controleren of de pointers niet NULL zijn. Anders return -1
    if (file_name != NULL)
    {
        // Bestand openen in een FILE pointer met "read binary"
        file_ptr = fopen(file_name, "rb");

        // Controleren of het bestand ook echt geopend is
        if (file_ptr != NULL)
        {
            // Bestandsgrote bepalen
            //  - Eerst naar het einde van het bestand spoelen
            //  - Vervolgens de huidige cursor locatie ophalen
            //  - En weer terugspoelen
            fseek(file_ptr, 0, SEEK_END);
            total_file_size = ftell(file_ptr);
            rewind(file_ptr);

            // Totaal aantal studenten is
            // totale bestandsgrote delen door de grote van een STUDENT struct
            total_students = total_file_size / sizeof(STUDENT);

            result = total_students;

            // Bestand sluiten
            fclose(file_ptr);
        }
        else
        {
            // Bestand is NULL
            result = -1;
        }
    }
    else
    {
        // Parameter fout
        result = -1;
    }
    
    return result;
}

int CreateStudentsArray(char *file_name, int total_students, STUDENT *student)
{
    int result = 0;

    if (file_name != NULL && total_students >= 0 && student != NULL)
    {
        FILE *file_ptr = fopen(file_name, "rb");

        if (file_ptr != NULL)
        {
            int i = 0;
            for (; i < total_students; i++)
            {
                STUDENT stdnt = {0, "", 0};
                if (-1 != fread(&stdnt, sizeof(STUDENT), 1, file_ptr))
                {
                    student[i] = stdnt;
                }
                else
                {
                    // Lees fout
                    result = -1;
                }
            }

            fclose(file_ptr);
        }
        else
        {
            // Bestands fout
            result = -1;
        }
    }
    else
    {
        // Parameter fout
        result = -1;
    }
    
    return result;
}

int CompareStudents(const void *a, const void *b)
{
    const STUDENT *student_a = (const STUDENT *) a;
    const STUDENT *student_b = (const STUDENT *) b;
    int number_a = student_a->StudentNumber;
    int number_b = student_b->StudentNumber;

    if (number_a == number_b)
    {
        return 0;
    }
    else if (number_a < number_b)
    {
        return -1;
    }
    else
    { 
        return 1;
    }
}

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
    int result = 0;
    

    if (FileName != NULL && Average != NULL)
    {
        int total_students = CountStudents(FileName);

        if (total_students > 0)
        {

            STUDENT students[total_students];
            int create_result = CreateStudentsArray(FileName, total_students, students);

            if (create_result >= 0)
            {
                int total_credits = 0;
                int i = 0;
                for (; i < total_students; i++)
                {
                    total_credits += students[i].StudyCredits;
                }
                total_credits = total_credits / total_students;
                *Average = total_credits;
            }
            else
            {
                result = -1;
            }
            
        }
        else if (total_students == 0)
        {
            *Average = 0;
            result = 0;
        }
        else
        {
            //*Average = 0;
            result = -1;
        }
    }
    else
    {
        // Parameter fout
        result = -1;
    }

    return result;
}


int LinearSearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr)
{
    int Result = -1;
    FILE *file_ptr = NULL;
    long total_file_size = 0;
    long total_students = 0;

    // Controleren of de pointers niet NULL zijn. Anders return -1
    if (FileName != NULL && StudentPtr != NULL)
    {
        // Bestand openen in een FILE pointer met "read binary"
        file_ptr = fopen(FileName, "rb");

        // Controleren of het bestand ook echt geopend is
        if (file_ptr != NULL)
        {
            // Bestandsgrote bepalen
            //  - Eerst naar het einde van het bestand spoelen
            //  - Vervolgens de huidige cursor locatie ophalen
            //  - En weer terugspoelen
            fseek(file_ptr, 0, SEEK_END);
            total_file_size = ftell(file_ptr);
            rewind(file_ptr);

            // Totaal aantal studenten is
            // totale bestandsgrote delen door de grote van een STUDENT struct
            total_students = total_file_size / sizeof(STUDENT);

            // Controleren of er studenten in het bestand zijn
            if (total_students > 0)
            {

                // Iedere student in het bestand uitlezen
                // en vergelijken of het studenten nummer
                // overeen komt met de parameter Number
                int i = 0;
                for (; i < total_students; i++)
                {
                    STUDENT stdnt = {0, "", 0};
                    if (-1 != fread(&stdnt, sizeof(STUDENT), 1, file_ptr))
                    {
                        if (stdnt.StudentNumber == Number)
                        {
                            *StudentPtr = stdnt;
                            Result = 0;
                            break;
                        }
                    }
                    else
                    {
                        // Lees fout
                        Result = -1;
                    }
                }

            }
            else
            {
                // Geen studenten in bestand
                Result = -1;
            }

            // Bestand sluiten
            fclose(file_ptr);
        }
        else
        {
            // Bestand is NULL
            Result = -1;
        }
    }
    else
    {
        // Parameter fout
        Result = -1;
    }
    
    return Result;
}

int BinarySearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr)
{
    int result = 0;
    

    if (FileName != NULL && StudentPtr != NULL)
    {
        int total_students = CountStudents(FileName);

        if (total_students > 0)
        {

            STUDENT students[total_students];
            int create_result = CreateStudentsArray(FileName, total_students, students);

            if (create_result >= 0)
            {
                qsort(students, total_students, sizeof(STUDENT), *CompareStudents);
                //STUDENT *res = 
            }
            else
            {
                // Lees fout
                result = -1;
            }
            
        }
        else
        {
            // Geen studenten
            result = -1;
        }
    }
    else
    {
        // Parameter fout
        result = -1;
    }

    return result;
}



int AddStudentSortedToFile (char* FileName, STUDENT* StudentPtr)
{
    int result = 0;

    return result;
}


int RemoveStudentFromFile (char* FileName, int StudentNumber)
{
    int Result = 0;

    // TODO: implement
    
    return Result;
}

