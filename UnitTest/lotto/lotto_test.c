#include "unity/fixture/unity_fixture.h"

#include "fake/fake_lotto.h"
#include "../App/Lotto/lotto.h"

TEST_GROUP(lotto);

TEST_SETUP(lotto)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(lotto)
{
    /* Cleanup after every test */
}

TEST(lotto, WhenNoGenerationNumbersTab2from12EqualZeros)
{
	uint8_t clearedArrayWith5elements[2] = {0, 0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(clearedArrayWith5elements, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, WhenNoGenerationNumbersTab5from50EqualZeros)
{
	uint8_t clearedArrayWith5elements[5] = {0, 0, 0, 0, 0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(clearedArrayWith5elements, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerateNumbersBufforsAreFilledOutWithRandomValues)
{
	LOTTO_generate_numbers();

	uint8_t clearedArrayWith5elements[5] = {0, 0, 0, 0, 0};
	for (uint8_t i = 0; i < 5; i++)
	{
		clearedArrayWith5elements[i] = PeripheralsFakeDefaultRandomReturnValue;
	}

    TEST_ASSERT_EQUAL_UINT8_ARRAY(clearedArrayWith5elements, LOTTO_results.lottoTab5from50, 5);
	// TEST_ASSERT_EQUAL(5, 5);
}

TEST(lotto, NextTest)
{
	TEST_FAIL_MESSAGE("Implement your test!");
}
