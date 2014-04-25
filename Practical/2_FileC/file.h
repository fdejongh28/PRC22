#ifndef _FILE_H
#define _FILE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    int     StudentNumber;
    char    Surname[25];
    int     StudyCredits;
} STUDENT;

extern int ReadElement(FILE* FilePtr, int ElementNr, STUDENT* StudentPtr);
/* pre    : 
 * post   : If file contains enough elements, element 'ElementNr' is read into StudentPtr
 * returns: On succes: 1 (1 element was read)
 *          If element does not exist: 0
 *          In case of an error (file could not be read, input pointers are NULL): -1
 */

extern int WriteElement(FILE* FilePtr, int ElementNr, const STUDENT* StudentPtr);
/* pre    : 
 * post   : Element 'ElementNr' is written into file with data from StudentPtr
 * returns: On succes: 0
 *          In case of an error (file could not be written, input pointers are NULL): -1
 */

extern int ComputeAverageStudyResults (char* FileName, double* Average);
/* pre    :
 * post   : Calculates the average of all study results in a file
 * returns: -1 if an error occurs, 0 if a valid average is computed
 */

extern int LinearSearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr);
/* pre    : 
 * post   : Returns student information of student with student number Number
 * returns: -1 if Number is not available or in case of an error, 0 if Number is available
 */

extern int BinarySearchStudentsFile (char* FileName, int Number, STUDENT* StudentPtr);
/* pre    : 
 * post   : Returns student information of student with student number Number
 * returns: -1 if Number is not available or in case of an error, 0 if Number is available
 */

extern int AddStudentSortedToFile (char* FileName, STUDENT* StudentPtr);
/* pre    : 
 * post   : Adds information from StudentPtr to FileName in a sorted way,
 *          if FileName does not exist, a new file is created.
 * returns: -1 in case of an error
 *          0  if a student with the given student number already exists
 *          1  if the student is added to the file
 */

extern int RemoveStudentFromFile (char* FileName, int StudentNumber);
/* pre    : 
 * post   : Removes student with StudentNumber from file
 * returns: 0 if StudentNumber was removed from file
 *          -1 if StudentNumber was not found or an error occurs
 */

#endif
