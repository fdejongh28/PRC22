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
    // int result = 0;

    // if (FilePtr != NULL)
    // {
    //     fseek(FilePtr, 0, SEEK_END);
    //     int total_file_size = ftell(FilePtr);
    //     rewind(FilePtr);

    //     int total_students = total_file_size / sizeof(STUDENT);

    //     if (total_students > 0)
    //     {
    //         if (ElementNr < total_students)
    //         {
    //             fseek(FilePtr, ElementNr * sizeof(STUDENT), SEEK_SET);
    //             STUDENT stdnt = {0, "", 0};
    //             if (-1 != fread(&stdnt, sizeof(STUDENT), 1, FilePtr))
    //             {
    //                 *StudentPtr = stdnt;
    //                 result = 1;
    //             }
    //             else
    //             {
    //                 result = -1;
    //             }
    //         }
    //         else
    //         {
    //             result = 0;
    //         }
    //     }
    //     else
    //     {
    //         result = 0;
    //     }

    // }
    // else
    // {
    //     // Bestands fout
    //     result = -1;
    // }

    // return result;

    int Result = 0;

    if ( FilePtr != NULL && StudentPtr != NULL)
	{
		// student size
		size_t size = sizeof(STUDENT);

		// buffer for student data
		STUDENT temp;

		// go to student
		fseek (FilePtr , ElementNr * size , SEEK_SET );

		//read into student buffer
		size_t res;
    	res = fread(&temp, size, 1, FilePtr);

    	// if elemet read succeeded, store into student pointer
    	if(res == 1) {  		
    		*StudentPtr = temp;
    		Result = 1;
    	}
    }
    else {
    	// error
    	Result = -1;
    }

    return Result;
}

int WriteElement(FILE* FilePtr, int ElementNr, const STUDENT* StudentPtr)
{
    int Result = 0;
    
    if ( FilePtr != NULL && StudentPtr != NULL)
	{
		// student size
		size_t size = sizeof(STUDENT);

		// write into file
		fwrite (StudentPtr, size, 1, FilePtr);
    }
    else {
    	// error
    	Result = -1;
    }

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
                STUDENT key;
                key.StudentNumber = Number;
                STUDENT *res = bsearch(&key, students, total_students, sizeof(STUDENT), CompareStudents);

                if (res != NULL)
                {
                    printf("Nummer %d gevonden op %d. ", Number, res->StudentNumber);
                    result = 0;
                    StudentPtr = res;
                    printf("Ptr nummer: %d\n", StudentPtr->StudentNumber);
                }
                else
                {
                    result = -1;
                }
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
    int Result = 8;

    if (FileName != NULL && StudentPtr != NULL)
    {
    	// open file to create if not exists
    	FILE *FilePtr = fopen(FileName, "ab+");
    	fclose(FilePtr);

    	// student buffer
    	STUDENT* Student = NULL;

    	// get student number
    	STUDENT Stud = *StudentPtr;
    	int StudentNumber = Stud.StudentNumber;

    	// check if student exitst by return val of search
    	int StudentExists = LinearSearchStudentsFile (FileName, StudentNumber, Student);

    	if(Student->StudentNumber != StudentNumber && StudentExists) {
	    	// count students
	    	int StudentCount = CountStudents(FileName);

	    	// init array
	    	STUDENT Students[StudentCount];

	    	// get student array
	    	int StudentArray = CreateStudentsArray(FileName, StudentCount, Students);

	    	if(StudentArray != -1) {

	    		// add student to the end of file
	    		Students[StudentCount+1] = *StudentPtr;

	    		// sort students
	    		qsort(Students, StudentCount, sizeof(STUDENT), CompareStudents);

	    		// open file
	    		FILE *FilePtr = fopen(FileName, "ab+");

		        // check if opened
		        if (FilePtr != NULL)
		        {
		        	// write into file
		        	WriteElement(FilePtr, 1, Students);

		        	// close file
		        	fclose(FilePtr);

		        	Result = 1;
				}
				else
				{
					Result = -1;
				}
			}
			else
			{
				Result = -1;
			}
		}
		else
		{
			Result = 0;
		}
	}
	else {
		Result = -1;
	}

    return Result;
}


int RemoveStudentFromFile (char* FileName, int StudentNumber)
{
    int Result = 0;

    if (FileName != NULL)
    {
        int total_students = CountStudents(FileName);

        STUDENT students[total_students];
        if (-1 != CreateStudentsArray(FileName, total_students, students))
        {
            STUDENT target;
            if (0 == BinarySearchStudentsFile(FileName, StudentNumber, &target))
            {
                STUDENT new_array[total_students - 1];
                int i = 0;
                int min = 0;
                for (; i < total_students - 1; i++)
                {
                    if (students[i].StudentNumber == target.StudentNumber)
                    {
                        min++;
                    }
                    else
                    {
                        new_array[i - min] = students[i];
                    }
                }
                FILE *file_ptr = fopen(FileName, "w");
                if (file_ptr != NULL)
                {
                    if (-1 != fwrite(new_array, sizeof(new_array), 1, file_ptr))
                    {
                        Result = -1;
                    }

                    fclose(file_ptr);
                }

            }
            else
            {
                Result = -1;
            }
        }
        else
        {
            Result = -1;
        }
    }
    else
    {
        Result = -1;
    }
    
    return Result;
}