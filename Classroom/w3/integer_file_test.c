#include "integer_file.h"
#include "unity.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)


#define TESTFILE "test.txt"

void setUp(void)
{
    // This is run before EACH test
    create_file(TESTFILE, 8320, 7321);
}

void tearDown(void)
{
    // This is run after EACH test
    delete_file(TESTFILE);
}


void test_NullCheck(void)
{
    FILE* fileptr = NULL;

    TEST_ASSERT_EQUAL(-1, read_value(fileptr, 12));
}

void test_ReadingOnCorrectSpotWithCorrectLength(void)
{
    FILE* fileptr = NULL;
    fileptr = fopen(TESTFILE, "rb");

    //All of these should pass
    TEST_ASSERT_NOT_NULL(fileptr);
    TEST_ASSERT_EQUAL(8320, read_value(fileptr, 1));
    TEST_ASSERT_EQUAL(8071, read_value(fileptr, 250));
    TEST_ASSERT_EQUAL(7321, read_value(fileptr, 1000));

    fclose(fileptr);
}

int main(void)
{
    UnityBegin();

    MY_RUN_TEST(test_NullCheck);
    MY_RUN_TEST(test_ReadingOnCorrectSpotWithCorrectLength);

    return UnityEnd();
}
