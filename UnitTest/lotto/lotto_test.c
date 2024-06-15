#include "unity/fixture/unity_fixture.h"

#include "fake/fake_lotto.h"
#include "../App/Lotto/lotto.h"

#define LOTTO_5FROM50_OUT_RANGE 51U
#define LOTTO_2FROM12_OUT_RANGE 13U

Lotto_t test_LOTTO_results;

TEST_GROUP(lotto);

TEST_SETUP(lotto)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(lotto)
{
    /* Cleanup after every test */
}

TEST(lotto, WhenNoGenerationNumbersTab2from12initialisedWithZeros)
{
	for (uint8_t i = 0; i < 2; i++)
	{
		test_LOTTO_results.lottoTab2from12[i] = 0U;
	}

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab2from12, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, WhenNoGenerationNumbersTab5from50initialisedWithZeros)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		test_LOTTO_results.lottoTab5from50[i] = 0U;
	}

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5of50NumbersBufforsAreFilledOutWithRandomValues)
{
	LOTTO_generate_numbers();

	for (uint8_t i = 0; i < 5; i++)
	{
		test_LOTTO_results.lottoTab5from50[i] = (PeripheralsFakeDefaultRandomReturnValue % LOTTO_5FROM50_OUT_RANGE);
	}

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate2of12NumbersBufforsAreFilledOutWithRandomValues)
{
	LOTTO_generate_numbers();

	for (uint8_t i = 0; i < 2; i++)
	{
		test_LOTTO_results.lottoTab2from12[i] = (PeripheralsFakeDefaultRandomReturnValue % LOTTO_2FROM12_OUT_RANGE);
	}

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab2from12, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, NextTest)
{
	TEST_FAIL_MESSAGE("Implement your test!");
}
