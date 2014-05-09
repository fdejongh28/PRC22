#include <string.h>

#ifdef __x86_64__
  #define UNITY_SUPPORT_64
  #define CUSTOM_PTR_TEST TEST_ASSERT_EQUAL_HEX64_MESSAGE
#else
  #define CUSTOM_PTR_TEST TEST_ASSERT_EQUAL_HEX32_MESSAGE
#endif

#include "parameter.h"
#include "unity.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

int TestArray[] = { 7, 4, 1, 8, 5, 2, 9, 6, 3, 0, 10 };
static const char* ReferenceEnglish[] = { "seven", "four", "one", "eight", "five", "two", "nine", "six", "three", "zero", "ten" };
static const char* ReferenceFrench[] = { "sept", "quatre", "un", "huit", "cinq", "deux", "neuf", "six", "trois", "zero", "dix" };
const char* ReferenceDutch[] = { "zeven", "vier", "een", "acht", "vijf", "twee", "negen", "zes", "drie", "nul", "tien" };

char*
French (int i)
{
    static char * Translation [] =
    { "zero", "un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf", "dix" };

    return Translation [i];
}

char*
English (int i)
{
    switch (i)
    {
        case 10:
            return ("ten");
        case 9:
            return ("nine");
        case 8:
            return ("eight");
        case 7:
            return ("seven");
        case 6:
            return ("six");
        case 5:
            return ("five");
        case 4:
            return ("four");
        case 3:
            return ("three");
        case 2:
            return ("two");
        case 1:
            return ("one");
        case 0:
            return ("zero");
        default:
            return ("I don't know");
    }
}


void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
    // This is run after EACH test
}


void SwapNormalTester(int OrgA, int OrgB)
{
    int a = OrgA;
    int b = OrgB;
    SwapValues(&a, &b);
    TEST_ASSERT_EQUAL(OrgA, b);
    TEST_ASSERT_EQUAL(OrgB, a);
}
void test_SwapNormalValues(void)
{
    SwapNormalTester(1234, 5678);
}

void test_SwapValuesOneZero(void)
{
    SwapNormalTester(0, 7890);
}

void test_SwapValuesOneZeroOneBig(void)
{
    SwapNormalTester(555555555, 0);
}

void test_SwapValuesOneBigOneNegative(void)
{
    SwapNormalTester(2123456789, -2098765432);
}

void SwapPointerTester(int OrgA, int OrgB)
{
    int a = OrgA;
    int b = OrgB;
    int* pa = &a;
    int* pb = &b;
    SwapAddresses(&pa, &pb);
    TEST_ASSERT_EQUAL(OrgA, a);
    TEST_ASSERT_EQUAL(OrgB, b);
    CUSTOM_PTR_TEST(&a, pb, "Pointer B check");
    CUSTOM_PTR_TEST(&b, pa, "Pointer A check");
}

void test_SwapPointerNormalValues(void)
{
    SwapPointerTester(1234, 5678);
}

void test_SwapPointerValuesOneZero(void)
{
    SwapPointerTester(0, 7890);
}

void test_SwapPointerValuesOneZeroOneBig(void)
{
    SwapPointerTester(555555555, 0);
}

void test_SwapPointerValuesOneBigOneNegative(void)
{
    SwapPointerTester(2123456789, -2098765432);
}


int SumReturnTester(int n)
{
    int Sum = 0;
    Sum = GetSumReturn(TestArray, n);
    return Sum;
}

void test_SumReturn(void)
{
    TEST_ASSERT_EQUAL(25, SumReturnTester(5));
    TEST_ASSERT_EQUAL(7, SumReturnTester(1));
    TEST_ASSERT_EQUAL(45, SumReturnTester(10));
    TEST_ASSERT_EQUAL(55, SumReturnTester(11));
    TEST_ASSERT_EQUAL(0, SumReturnTester(0));
    TEST_ASSERT_EQUAL(0, SumReturnTester(-1));
}

int SumParameterTester(int n)
{
    int sum = 0;
    GetSumParameter(TestArray, n, &sum);
    return sum;
}

void test_SumParameter(void)
{
    TEST_ASSERT_EQUAL(25, SumParameterTester(5));
    TEST_ASSERT_EQUAL(7, SumParameterTester(1));
    TEST_ASSERT_EQUAL(45, SumParameterTester(10));
    TEST_ASSERT_EQUAL(55, SumParameterTester(11));
    TEST_ASSERT_EQUAL(0, SumParameterTester(0));
    TEST_ASSERT_EQUAL(0, SumParameterTester(-1));
}


void TranslationTest(char* Language (int Index), int NrTranslations, const char* Reference[])
{
    char* Result[sizeof(TestArray)/sizeof(TestArray[0])];
    int i = 0;

    // clear complete array
    for (i = 0; i < 11; i++)
    {
        Result[i] = "";
    }

    Translate(Language, TestArray, NrTranslations, Result);

    for (i = 0; i < 11; i++)
    {
        if (i < NrTranslations)
        {
            TEST_ASSERT_EQUAL_STRING(Reference[i], Result[i]);
        }
        else
        {
            TEST_ASSERT_EQUAL_STRING("", Result[i]);
        }
    }
}

void test_TranslateEnglishFullArray(void)
{
    TranslationTest(English, 11, ReferenceEnglish);
}

void test_TranslateFrenchFullArray(void)
{
    TranslationTest(French, 11, ReferenceFrench);
}

void test_TranslateDutchFullArray(void)
{
    TranslationTest(Dutch, 11, ReferenceDutch);
}

void test_TranslateEnglishThreeValues(void)
{
    TranslationTest(English, 3, ReferenceEnglish);
}

void test_TranslateFrenchThreeValues(void)
{
    TranslationTest(French, 3, ReferenceFrench);
}

void test_TranslateDutchThreeValues(void)
{
    TranslationTest(Dutch, 3, ReferenceDutch);
}

void test_TranslateEnglishZeroValues(void)
{
    TranslationTest(English, 0, ReferenceEnglish);
}

void test_TranslateFrenchZeroValues(void)
{
    TranslationTest(French, 0, ReferenceFrench);
}

void test_TranslateDutchZeroValues(void)
{
    TranslationTest(Dutch, 0, ReferenceDutch);
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
int main(void)
{
    UnityBegin();

    MY_RUN_TEST(test_SwapNormalValues);
    MY_RUN_TEST(test_SwapValuesOneZero);
    MY_RUN_TEST(test_SwapValuesOneZeroOneBig);
    MY_RUN_TEST(test_SwapValuesOneBigOneNegative);

    MY_RUN_TEST(test_SwapPointerNormalValues);
    MY_RUN_TEST(test_SwapPointerValuesOneZero);
    MY_RUN_TEST(test_SwapPointerValuesOneZeroOneBig);
    MY_RUN_TEST(test_SwapPointerValuesOneBigOneNegative);

    MY_RUN_TEST(test_SumReturn);
    MY_RUN_TEST(test_SumParameter);

    MY_RUN_TEST(test_TranslateEnglishFullArray);
    MY_RUN_TEST(test_TranslateFrenchFullArray);
    MY_RUN_TEST(test_TranslateDutchFullArray);
    MY_RUN_TEST(test_TranslateEnglishThreeValues);
    MY_RUN_TEST(test_TranslateFrenchThreeValues);
    MY_RUN_TEST(test_TranslateDutchThreeValues);
    MY_RUN_TEST(test_TranslateEnglishZeroValues);
    MY_RUN_TEST(test_TranslateFrenchZeroValues);
    MY_RUN_TEST(test_TranslateDutchZeroValues);

    return UnityEnd();
}
