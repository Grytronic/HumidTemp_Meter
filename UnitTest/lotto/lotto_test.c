#include "unity/fixture/unity_fixture.h"

#include "fake/fake_lotto.h"
#include "../App/Lotto/lotto.h"


Lotto_t testLOTTOresults;


/*
void update_5from50_testLOTTOresults_to_last_generated_value(void)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		testLOTTOresults.lottoTab5from50[i] = PERIPHERALS_get_generated_random_value();
	}
}

void update_2from12_testLOTTOresults_to_last_generated_value(void)
{
	for (uint8_t i = 0; i < 2; i++)
	{
		testLOTTOresults.lottoTab2from12[i] = PERIPHERALS_get_generated_random_value();
	}
}
*/

void update_5from50_testLOTTOresults_with_expected_value(uint8_t val)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		testLOTTOresults.lottoTab5from50[i] = val;
	}
}

void update_2from12_testLOTTOresults_with_expected_value(uint8_t val)
{
	for (uint8_t i = 0; i < 2; i++)
	{
		testLOTTOresults.lottoTab2from12[i] = val;
	}
}

void set_5from50_testLOTTOresults_value(int val)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		testLOTTOresults.lottoTab5from50[i] = val;
	}
}

void set_2from12_testLOTTOresults_value(int val)
{
	for (uint8_t i = 0; i < 2; i++)
	{
		testLOTTOresults.lottoTab2from12[i] = val;
	}
}

void set_generated_values_and_generate_LOTTO_numbers(int val)
{
	PERIPHERALS_set_value_for_generated_random_value(val);
	LOTTO_generate_numbers();
}


TEST_GROUP(lotto);

TEST_SETUP(lotto)
{
    /* Init before every test */
	PERIPHERALS_reset_start_value_for_generated_random_value();
}

TEST_TEAR_DOWN(lotto)
{
    /* Cleanup after every test */
}


TEST(lotto, WhenNoGenerationNumbersTab5from50initialisedWithZeros)
{
	uint8_t expectedLottoTab5from50[] = {FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(0);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithInRangeValues)
{
	uint8_t expectedLottoTab5from50[] = {34, 35, 36, 37, 38};

	set_generated_values_and_generate_LOTTO_numbers(34);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithUpperLimitValue)
{
	uint8_t expectedLottoTab5from50[] = {50, FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6};

	set_generated_values_and_generate_LOTTO_numbers(50);
	//update_5from50_testLOTTOresults_with_expected_value(50);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithPositiveOverrangeValues)
{
	uint8_t expectedLottoTab5from50[] = {FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(51);
	//update_5from50_testLOTTOresults_with_expected_value(FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithOverrangeBigValues)
{
	uint8_t expectedLottoTab5from50[] = {FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(127);
	//update_5from50_testLOTTOresults_with_expected_value(127 % LOTTO_5FROM50_OUT_RANGE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithNegativeOverrangeValues)
{
	uint8_t expectedLottoTab5from50[] = {FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(-128);
	//update_5from50_testLOTTOresults_with_expected_value(FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithTheSameValues)
{
	uint8_t expectedLottoTab5from50[] = {0, 1, 2, 3, 4};

	for (uint8_t i = 0; i < 5; i++)
	{
		LOTTO_results.lottoTab5from50[i] = i;
	}

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}








TEST(lotto, WhenNoGenerationNumbersTab2from12initialisedWithZeros)
{
	set_generated_values_and_generate_LOTTO_numbers(0);
	update_2from12_testLOTTOresults_with_expected_value(FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(testLOTTOresults.lottoTab2from12, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithWithInRangeValues)
{
	set_generated_values_and_generate_LOTTO_numbers(6);
	update_2from12_testLOTTOresults_with_expected_value(6);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(testLOTTOresults.lottoTab2from12, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, empty_test_fail)
{
	TEST_FAIL_MESSAGE("Implement your test!");
}




