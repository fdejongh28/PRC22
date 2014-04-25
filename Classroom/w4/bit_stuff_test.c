#include "bit_stuff.h"
#include "unity.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)


void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
    // This is run after EACH test
}


void test_BitMaskNrBits(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x1, MakeBitmask(1, 0));
    TEST_ASSERT_EQUAL_HEX32(0x3, MakeBitmask(2, 0));
    TEST_ASSERT_EQUAL_HEX32(0x1f, MakeBitmask(5, 0));
    TEST_ASSERT_EQUAL_HEX32(0xffffffff, MakeBitmask(32, 0));
}

void test_BitMaskShift(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x2, MakeBitmask(1, 1));
    TEST_ASSERT_EQUAL_HEX32(0x80, MakeBitmask(1, 7));
    TEST_ASSERT_EQUAL_HEX32(0x100, MakeBitmask(1, 8));
    TEST_ASSERT_EQUAL_HEX32(0x38000, MakeBitmask(3, 15));
    TEST_ASSERT_EQUAL_HEX32(0xC0000000, MakeBitmask(8, 30));
}

void test_CountOnes(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, CountOnes(0x0));
    TEST_ASSERT_EQUAL_UINT32(32, CountOnes(0xffffffff));
    TEST_ASSERT_EQUAL_UINT32(16, CountOnes(0x5a5a5a5a));
}

void test_CountOnesInBitMask(void)
{
    unsigned int i = 0;
    for (i = 0; i < 32; i++)
    {
        TEST_ASSERT_EQUAL_UINT32(i, CountOnes(MakeBitmask(i, 0)));
    }
}

int main(void)
{
    UnityBegin();
    
    MY_RUN_TEST(test_BitMaskNrBits);
    MY_RUN_TEST(test_BitMaskShift);
    MY_RUN_TEST(test_CountOnes);
    MY_RUN_TEST(test_CountOnesInBitMask);

    return UnityEnd();
}
