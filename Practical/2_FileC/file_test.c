#include <string.h>

#include "file.h"
#include "unity.h"
#include "resource_detector.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

#define CORRUPT_FILE_1    "TestFileCorrupt1.dat"
#define CORRUPT_FILE_2    "TestFileCorrupt2.dat"
#define EMPTY_FILE        "TestFileEmpty.dat"
#define VALID_FILE        "TestFileValid.dat"
#define NON_EXISTING_FILE "NonExistingFile.bla"
#define NEW_FILE          "students.dat"

// test numbers for search tests
static int TestNumbers[] = {-10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55};
static char* TestNames[] = {"MinusTen", "MinusFive", "Zero", "Five", "Ten", "Fifteen",
                            "Twenty", "Twentyfive", "Thirty", "Thirtyfive", "Forty",
                            "Fortyfive", "Fifty", "Fiftyfive"};
static const int NrTestNumbers = sizeof(TestNumbers)/sizeof(TestNumbers[0]);

void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
    // This is run after EACH test
}

static void StudentPropertiesValidation(int ExpectedNr, char* ExpectedName, int ExpectedStudyResult, const STUDENT* ResultPtr)
{
    TEST_ASSERT_EQUAL(ExpectedNr, ResultPtr->StudentNumber);
    TEST_ASSERT_EQUAL_STRING(ExpectedName, ResultPtr->Surname);
    TEST_ASSERT_EQUAL(ExpectedStudyResult, ResultPtr->StudyCredits);
}

STUDENT FillStudentProperties(int Nr, char* Name, int StudyResult)
{
    STUDENT Student = {Nr, "", StudyResult};
    strncpy(Student.Surname, Name, sizeof(Student.Surname));
    return Student;
}

static void test_ReadElementParameters(void)
{
    STUDENT Student = {-1987, "ThisIsAReallyWeirdName", 12317462};
    FILE* FilePtr = (FILE*)1; // dirty trick to get a pointer that is not NULL, the (FILE*) is a cast to type FILE*

    TEST_ASSERT_EQUAL(-1, ReadElement(FilePtr, 231, NULL));
    TEST_ASSERT_EQUAL(-1, ReadElement(NULL, 87, NULL));
    TEST_ASSERT_EQUAL(-1, ReadElement(NULL, 1, &Student));

    // and test if the Student parameter is not modified
    StudentPropertiesValidation(-1987, "ThisIsAReallyWeirdName", 12317462, &Student);
}

void test_ReadElementEmptyFile(void)
{
    STUDENT Student = {189362, "AnotherAwfulName", -3245};
    FILE* FilePtr = fopen(EMPTY_FILE, "rb");

    TEST_ASSERT_NOT_NULL(FilePtr);

    for (int i = 0; i < 150; i += 12)
    {
        TEST_ASSERT_EQUAL(0, ReadElement(FilePtr, i, &Student));
    }

    // and test if the Student parameter is not modified
    StudentPropertiesValidation(189362, "AnotherAwfulName", -3245, &Student);

    fclose(FilePtr);
    FilePtr = NULL;
}

void test_ReadElementValidFile(void)
{
    STUDENT Student = {189362, "AnotherAwfulName", -3245};
    FILE* FilePtr = fopen(VALID_FILE, "rb");

    TEST_ASSERT_NOT_NULL(FilePtr);

    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 13, &Student));
    StudentPropertiesValidation(55, "Fiftyfive", 220, &Student);

    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 2, &Student));
    StudentPropertiesValidation(0, "Zero", 0, &Student);

    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 0, &Student));
    StudentPropertiesValidation(-10, "MinusTen", -40, &Student);

    TEST_ASSERT_EQUAL(0, ReadElement(FilePtr, 14, &Student));
    StudentPropertiesValidation(-10, "MinusTen", -40, &Student);

    fclose(FilePtr);
    FilePtr = NULL;
}

void test_ReadElementInvalidFile1(void)
{
    STUDENT Student = {19826, "Bladiebladiebla", 12345};
    FILE* FilePtr = fopen(CORRUPT_FILE_1, "rb");

    TEST_ASSERT_NOT_NULL(FilePtr);

    TEST_ASSERT_EQUAL(0, ReadElement(FilePtr, 0, &Student));
    StudentPropertiesValidation(19826, "Bladiebladiebla", 12345, &Student);

    fclose(FilePtr);
    FilePtr = NULL;
}

void test_ReadElementInvalidFile2(void)
{
    STUDENT Student = {19826, "Bladiebladiebla", 12345};
    FILE* FilePtr = fopen(CORRUPT_FILE_2, "rb");

    TEST_ASSERT_NOT_NULL(FilePtr);

    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 1, &Student));
    StudentPropertiesValidation(20, "Finkers", 20, &Student);

    TEST_ASSERT_EQUAL(0, ReadElement(FilePtr, 2, &Student));
    StudentPropertiesValidation(20, "Finkers", 20, &Student);

    fclose(FilePtr);
    FilePtr = NULL;
}


void test_WriteElementParameters(void)
{
    STUDENT Student = {-1987, "ThisIsAReallyWeirdName", 12317462};
    FILE* FilePtr = (FILE*)1;

    TEST_ASSERT_EQUAL(-1, WriteElement(FilePtr, 231, NULL));
    TEST_ASSERT_EQUAL(-1, WriteElement(NULL, 87, NULL));
    TEST_ASSERT_EQUAL(-1, WriteElement(NULL, 1, &Student));

    // and test if the Student parameter is not modified
    StudentPropertiesValidation(-1987, "ThisIsAReallyWeirdName", 12317462, &Student);
}

void test_WriteElement(void)
{
    STUDENT Student = {189362, "AnotherAwfulName", -3245};
    FILE* FilePtr = fopen(NEW_FILE, "wb");
    int i = 0;

    TEST_ASSERT_NOT_NULL(FilePtr);

    // write 10 equal students to file
    for (i = 0; i < 10; i++)
    {
        TEST_ASSERT_EQUAL(0, WriteElement(FilePtr, i, &Student));
    }
    fclose(FilePtr);
    FilePtr = NULL;

    // and read them back
    FilePtr = fopen(NEW_FILE, "rb");

    TEST_ASSERT_NOT_NULL(FilePtr);
    for (i = 0; i < 10; i++)
    {
        TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, i, &Student));
        StudentPropertiesValidation(189362, "AnotherAwfulName", -3245, &Student);
    }
    TEST_ASSERT_EQUAL(0, ReadElement(FilePtr, i, &Student));

    fclose(FilePtr);
    FilePtr = NULL;

    // finally: delete file
    remove(NEW_FILE);
}


void test_ComputeAverageParameters(void)
{
    double Average = 471.8532357;
    TEST_ASSERT_EQUAL(-1, ComputeAverageStudyResults(VALID_FILE, NULL));
    TEST_ASSERT_EQUAL(-1, ComputeAverageStudyResults(NULL, NULL));
    TEST_ASSERT_EQUAL(-1, ComputeAverageStudyResults(NULL, &Average));
    TEST_ASSERT_EQUAL_FLOAT(471.8532357, Average);
}

void test_ComputeAverageInNonExistingFile(void)
{
    double Average = 9287.1234;
    TEST_ASSERT_EQUAL(-1, ComputeAverageStudyResults(NON_EXISTING_FILE, &Average));
    TEST_ASSERT_EQUAL_FLOAT(9287.1234, Average);
}

void test_ComputeAverageInEmptyFile(void)
{
    double Average = 9574363.1;
    TEST_ASSERT_EQUAL(0, ComputeAverageStudyResults(EMPTY_FILE, &Average));
    TEST_ASSERT_EQUAL_FLOAT(0, Average);
}

void test_ComputeAverageInValidFile(void)
{
    double Average = 1.2345567;
    TEST_ASSERT_EQUAL(0, ComputeAverageStudyResults(VALID_FILE, &Average));
    TEST_ASSERT_EQUAL_FLOAT(90, Average);
}

void test_ComputeAverageInCorruptFile1(void)
{
    double Average = 9340.132425;
    TEST_ASSERT_EQUAL(0, ComputeAverageStudyResults(CORRUPT_FILE_1, &Average));
    TEST_ASSERT_EQUAL_FLOAT(0, Average);
}

void test_ComputeAverageInCorruptFile2(void)
{
    double Average = 9340.132425;
    TEST_ASSERT_EQUAL(0, ComputeAverageStudyResults(CORRUPT_FILE_2, &Average));
    TEST_ASSERT_EQUAL_FLOAT(15, Average);
}

void SearchNonExistingFileTest(int search (char*, int, STUDENT*))
{
    STUDENT Student = {-132, "alkdfja;lkdsjf", 20431};
    TEST_ASSERT_EQUAL(-1, search(NON_EXISTING_FILE, 0, &Student));
    StudentPropertiesValidation(-132, "alkdfja;lkdsjf", 20431, &Student);
}

void SearchParameterTest(int search (char*, int, STUDENT*))
{
    STUDENT Student = {-132, "alkdfja;lkdsjf", 20431};
    TEST_ASSERT_EQUAL(-1, search(NULL, 0, &Student));
    TEST_ASSERT_EQUAL(-1, search(VALID_FILE, 0, NULL));
    TEST_ASSERT_EQUAL(-1, search(NULL, 0, NULL));

    StudentPropertiesValidation(-132, "alkdfja;lkdsjf", 20431, &Student);
}

void SearchMatchesTest(int search (char*, int, STUDENT*))
{
    for(int i = 0; i < NrTestNumbers; i++)
    {
        STUDENT Student = {-4231, "bla", 49211};
        TEST_ASSERT_EQUAL(0, search(VALID_FILE, TestNumbers[i], &Student));
        StudentPropertiesValidation(TestNumbers[i], TestNames[i], 4*TestNumbers[i], &Student);
    }
}

void SearchInBetweenTest(int search (char*, int, STUDENT*))
{
    for(int i = 0; i < (NrTestNumbers-1); i++)
    {
        int start = TestNumbers[i] + 1;
        int end = TestNumbers[i+1] - 1;
        for(int j = start; j < end; j++)
        {
            STUDENT Student = {3892, "StrangeName", -92781};
            TEST_ASSERT_EQUAL(-1, search(VALID_FILE, j, &Student));
            StudentPropertiesValidation(3892, "StrangeName", -92781, &Student);
        }
    }
}

void SearchBeforeFirstTest(int search (char*, int, STUDENT*))
{
    // Make sure the first value is negative
    TEST_ASSERT(TestNumbers[0] < 0);

    int start = TestNumbers[0] * 2;
    int end = TestNumbers[0] - 1;

    for(int j = start; j <= end; j++)
    {
        STUDENT Student = {8, "WeirdName", 90};
        TEST_ASSERT_EQUAL(-1, search(VALID_FILE, j, &Student));
        StudentPropertiesValidation(8, "WeirdName", 90, &Student);
    }
}

void SearchBeyondLastTest(int search (char*, int, STUDENT*))
{
    // Make sure the last value is positive
    TEST_ASSERT(TestNumbers[NrTestNumbers-1] > 0);

    int start = TestNumbers[NrTestNumbers-1] + 1;
    int end = TestNumbers[NrTestNumbers-1] * 2;

    for(int j = start; j <= end; j++)
    {
        STUDENT Student = {-12, "MeNotHere", -9571};
        TEST_ASSERT_EQUAL(-1, search(VALID_FILE, j, &Student));
        StudentPropertiesValidation(-12, "MeNotHere", -9571, &Student);
    }
}

void test_LineairSearchNonExistingFile(void)
{
    SearchNonExistingFileTest(LinearSearchStudentsFile);
}

void test_LineairSearchParameters(void)
{
    SearchParameterTest(LinearSearchStudentsFile);
}

void test_LineairSearchMatches(void)
{
    SearchMatchesTest(LinearSearchStudentsFile);
}

void test_LineairSearchInBetweenValues(void)
{
    SearchInBetweenTest(LinearSearchStudentsFile);
}

void test_LineairSearchValuesBeforeFirst(void)
{
    SearchBeforeFirstTest(LinearSearchStudentsFile);
}

void test_LineairSearchValuesBeyondLast(void)
{
    SearchBeyondLastTest(LinearSearchStudentsFile);
}

void test_BinarySearchNonExistingFile(void)
{
    SearchNonExistingFileTest(BinarySearchStudentsFile);
}

void test_BinarySearchParameters(void)
{
    SearchParameterTest(BinarySearchStudentsFile);
}

void test_BinarySearchMatches(void)
{
    SearchMatchesTest(BinarySearchStudentsFile);
}

void test_BinarySearchInBetweenValues(void)
{
    SearchInBetweenTest(BinarySearchStudentsFile);
}

void test_BinarySearchValuesBeforeFirst(void)
{
    SearchBeforeFirstTest(BinarySearchStudentsFile);
}

void test_BinarySearchValuesBeyondLast(void)
{
    SearchBeyondLastTest(BinarySearchStudentsFile);
}

void test_AddStudentParameters(void)
{
    STUDENT Student = {1979, "WeirdName", 1234567};
    TEST_ASSERT_EQUAL(-1, AddStudentSortedToFile(NEW_FILE, NULL));
    TEST_ASSERT_EQUAL(-1, AddStudentSortedToFile(NULL, &Student));
    TEST_ASSERT_EQUAL(-1, AddStudentSortedToFile(NULL, NULL));

    StudentPropertiesValidation(1979, "WeirdName", 1234567, &Student);
}

void test_RemoveStudentParameters(void)
{
    TEST_ASSERT_EQUAL(-1, RemoveStudentFromFile(NULL, 1));
}

void test_AddAndRemoveStudents(void)
{
    // add new students
    STUDENT Student = {20, "Finkers", 40};
    TEST_ASSERT_EQUAL(1, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(40, "Maassen", 60);
    TEST_ASSERT_EQUAL(1, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(10, "Weijers", 30);
    TEST_ASSERT_EQUAL(1, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(30, "Breij, de", 50);
    TEST_ASSERT_EQUAL(1, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(100, "Goedemondt", 120);
    TEST_ASSERT_EQUAL(1, AddStudentSortedToFile(NEW_FILE, &Student));

    // add them again
    Student = FillStudentProperties(10, "New Weijers", 130);
    TEST_ASSERT_EQUAL(0, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(100, "New Goedemondt", 1120);
    TEST_ASSERT_EQUAL(0, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(40, "New Maassen", 160);
    TEST_ASSERT_EQUAL(0, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(20, "New Finkers", 140);
    TEST_ASSERT_EQUAL(0, AddStudentSortedToFile(NEW_FILE, &Student));
    Student = FillStudentProperties(30, "New Breij, de", 150);
    TEST_ASSERT_EQUAL(0, AddStudentSortedToFile(NEW_FILE, &Student));

    // test if they are on the correct spot in the file
    Student = FillStudentProperties(189362, "AnotherAwfulName", -3245);
    FILE* FilePtr = fopen(NEW_FILE, "rb");

    TEST_ASSERT_NOT_NULL(FilePtr);
    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 0, &Student));
    StudentPropertiesValidation(10, "Weijers", 30, &Student);
    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 1, &Student));
    StudentPropertiesValidation(20, "Finkers", 40, &Student);
    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 2, &Student));
    StudentPropertiesValidation(30, "Breij, de", 50, &Student);
    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 3, &Student));
    StudentPropertiesValidation(40, "Maassen", 60, &Student);
    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 4, &Student));
    StudentPropertiesValidation(100, "Goedemondt", 120, &Student);
    TEST_ASSERT_EQUAL(0, ReadElement(FilePtr, 5, &Student)); // no 6th element available
    StudentPropertiesValidation(100, "Goedemondt", 120, &Student);

    fclose(FilePtr);
    FilePtr = NULL;

    // remove some students
    TEST_ASSERT_EQUAL(0, RemoveStudentFromFile(NEW_FILE, 20));
    TEST_ASSERT_EQUAL(-1, RemoveStudentFromFile(NEW_FILE, 19));
    TEST_ASSERT_EQUAL(-1, RemoveStudentFromFile(NEW_FILE, 21));
    TEST_ASSERT_EQUAL(0, RemoveStudentFromFile(NEW_FILE, 10));
    TEST_ASSERT_EQUAL(0, RemoveStudentFromFile(NEW_FILE, 100));

    // read file to see if the remaining students are on the correct spot
    Student = FillStudentProperties(189362, "AnotherAwfulName", -3245);
    FilePtr = fopen(NEW_FILE, "rb");

    TEST_ASSERT_NOT_NULL(FilePtr);
    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 0, &Student));
    StudentPropertiesValidation(30, "Breij, de", 50, &Student);
    TEST_ASSERT_EQUAL(1, ReadElement(FilePtr, 1, &Student));
    StudentPropertiesValidation(40, "Maassen", 60, &Student);
    TEST_ASSERT_EQUAL(0, ReadElement(FilePtr, 2, &Student)); // no 3rd element available
    StudentPropertiesValidation(40, "Maassen", 60, &Student);

    fclose(FilePtr);
    FilePtr = NULL;

    // and remove testfile
    TEST_ASSERT_EQUAL(0, remove(NEW_FILE));
}

/*
 * You might have noticed that most unit test functions in this file are non-static (thus public).
 * This is done for a good reason: it allows for selective switching on tests. If these
 * functions were static, compiling with the -Wall and -Werror compiler flags would give
 * 'defined but not used' errors, which wouldn't allow switching tests one one by one.
 *
 * Switch the tests below on one by one, implement only functionality to satisfy the test
 * (while making sure previous tests keep working)
 */
int main (int argc, char * argv[])
{
    UnityBegin();

    MY_RUN_TEST(test_ReadElementParameters);
    //MY_RUN_TEST(test_ReadElementEmptyFile);
    //MY_RUN_TEST(test_ReadElementValidFile);
    //MY_RUN_TEST(test_ReadElementInvalidFile1);
    //MY_RUN_TEST(test_ReadElementInvalidFile2);

    //MY_RUN_TEST(test_WriteElementParameters);
    //MY_RUN_TEST(test_WriteElement);

    //MY_RUN_TEST(test_ComputeAverageParameters);
    //MY_RUN_TEST(test_ComputeAverageInNonExistingFile);
    //MY_RUN_TEST(test_ComputeAverageInEmptyFile);
    //MY_RUN_TEST(test_ComputeAverageInValidFile);
    //MY_RUN_TEST(test_ComputeAverageInCorruptFile1);
    //MY_RUN_TEST(test_ComputeAverageInCorruptFile2);

    //MY_RUN_TEST(test_LineairSearchNonExistingFile);
    //MY_RUN_TEST(test_LineairSearchParameters);
    //MY_RUN_TEST(test_LineairSearchMatches);
    //MY_RUN_TEST(test_LineairSearchInBetweenValues);
    //MY_RUN_TEST(test_LineairSearchValuesBeforeFirst);
    //MY_RUN_TEST(test_LineairSearchValuesBeyondLast);

    //MY_RUN_TEST(test_BinarySearchNonExistingFile);
    //MY_RUN_TEST(test_BinarySearchParameters);
    //MY_RUN_TEST(test_BinarySearchMatches);
    //MY_RUN_TEST(test_BinarySearchInBetweenValues);
    //MY_RUN_TEST(test_BinarySearchValuesBeforeFirst);
    //MY_RUN_TEST(test_BinarySearchValuesBeyondLast);

    //MY_RUN_TEST(test_AddStudentParameters);
    //MY_RUN_TEST(test_RemoveStudentParameters);
    //MY_RUN_TEST(test_AddAndRemoveStudents);

    return UnityEnd();
}
