#include "unity/fixture/unity_fixture.h"

#include "stdio.h"
#include "mock_lotto.h"
#include "../App/Lotto/lotto.h"


Lotto_t testLOTTOresults;



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

void print_generated_numbers_5from50(uint8_t test_number)
{
	printf("\n\nTest_nr: %d \n", test_number);

	for (uint8_t i = 0; i < 5; i++)
	{
		printf("%d \n", LOTTO_results.lottoTab5from50[i]);
	}
}

void print_generated_numbers_2from12(uint8_t test_number)
{
	printf("\n\nTest_nr: %d \n", test_number);

	for (uint8_t i = 0; i < 2; i++)
	{
		printf("%d \n", LOTTO_results.lottoTab2from12[i]);
	}
}






TEST_GROUP(lotto);

TEST_SETUP(lotto)
{
    /* Init before every test */
	PERIPHERALS_reset_start_value_for_generated_random_value();
	PERIPHERALS_use_random_generation_mode(faked_numbers);
}

TEST_TEAR_DOWN(lotto)
{
    /* Cleanup after every test */
}


/* TEST 5 from 50 */
TEST(lotto, WhenNoGenerationNumbersTab5from50initialisedWithZero)
{
	uint8_t test_number = 1;
	uint8_t expectedLottoTab5from50[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(0);
	print_generated_numbers_5from50(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithInRangeValues)
{
	uint8_t test_number = 2;
	uint8_t expectedLottoTab5from50[] = {34, 35, 36, 37, 38};

	set_generated_values_and_generate_LOTTO_numbers(34);
	print_generated_numbers_5from50(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithUpperLimitValue)
{
	uint8_t test_number = 3;
	uint8_t expectedLottoTab5from50[] = {50, DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6};

	set_generated_values_and_generate_LOTTO_numbers(50);
	print_generated_numbers_5from50(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithPositiveOverrangeValues)
{
	uint8_t test_number = 4;
	uint8_t expectedLottoTab5from50[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(51);
	print_generated_numbers_5from50(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithOverrangeBigValues)
{
	uint8_t test_number = 5;
	uint8_t expectedLottoTab5from50[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(127);
	print_generated_numbers_5from50(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithNegativeOverrangeValues)
{
	uint8_t test_number = 6;
	uint8_t expectedLottoTab5from50[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4, 5, 6, 7};

	set_generated_values_and_generate_LOTTO_numbers(-128);
	print_generated_numbers_5from50(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5from50NumbersBufforsAreFilledOut2timesWithTheSameValues)
{
	uint8_t test_number = 7;
	uint8_t expectedLottoTab5from50[] = {5, 6, 7, 8, 9};

	LOTTO_results.lottoTab5from50[0] = 5;

	for (uint8_t i = 1; i < 5; i++)
	{
		LOTTO_results.lottoTab5from50[i] = 4 + i;
	}
	print_generated_numbers_5from50(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

/* TEST 2 from 12 */
TEST(lotto, WhenNoGenerationNumbersTab2from12initialisedWithZero)
{
	uint8_t test_number = 8;
	uint8_t expectedLottoTab2from12[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4};

	set_generated_values_and_generate_LOTTO_numbers(0);
	print_generated_numbers_2from12(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab2from12, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithInRangeValues)
{
	uint8_t test_number = 9;
	uint8_t expectedLottoTab2from12[] = {11, 12};

	set_generated_values_and_generate_LOTTO_numbers(11);
	print_generated_numbers_2from12(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab2from12, LOTTO_results.lottoTab2from12, 5);
}

TEST(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithUpperLimitValue)
{
	uint8_t test_number = 10;
	uint8_t expectedLottoTab2from12[] = {12, DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0};

	set_generated_values_and_generate_LOTTO_numbers(12);
	print_generated_numbers_2from12(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab2from12, LOTTO_results.lottoTab2from12, 5);
}

TEST(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithPositiveOverrangeValues)
{
	uint8_t test_number = 11;
	uint8_t expectedLottoTab2from12[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4};

	set_generated_values_and_generate_LOTTO_numbers(13);
	print_generated_numbers_2from12(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab2from12, LOTTO_results.lottoTab2from12, 5);
}

TEST(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithOverrangeBigValues)
{
	uint8_t test_number = 12;
	uint8_t expectedLottoTab2from12[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4};

	set_generated_values_and_generate_LOTTO_numbers(127);
	print_generated_numbers_2from12(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab2from12, LOTTO_results.lottoTab2from12, 5);
}

TEST(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithNegativeOverrangeValues)
{
	uint8_t test_number = 13;
	uint8_t expectedLottoTab2from12[] = {DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0, 4};

	set_generated_values_and_generate_LOTTO_numbers(-128);
	print_generated_numbers_2from12(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab2from12, LOTTO_results.lottoTab2from12, 5);
}

TEST(lotto, WhenGenerate2from12NumbersBufforsAreFilledOut2timesWithTheSameValues)
{
	uint8_t test_number = 14;
	uint8_t expectedLottoTab2from12[] = {0, 6};

	for (uint8_t i = 0; i < 2; i++)
	{
		LOTTO_results.lottoTab2from12[i] = i;
	}
	print_generated_numbers_2from12(test_number);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedLottoTab2from12, LOTTO_results.lottoTab2from12, 5);
}

TEST(lotto, GenerateLOTTOnumbersToBeMilionaire)
{
	uint8_t cout_of_games = 150;

	PERIPHERALS_use_random_generation_mode(generated_numbers);

	printf("\n\n\n\nGenerated LOTTO numbers for today: \n\n");
	printf("5_from_50 : 2_from_12:\n");
	printf("----------------------\n");
	for (uint8_t i = 0; i < cout_of_games; i++)
	{
		LOTTO_generate_numbers();

		for (uint8_t i = 0; i < 5; i++)
		{
			printf("%d ",LOTTO_results.lottoTab5from50[i]);
		}
		printf("  :  ");
		for (uint8_t i = 0; i < 2; i++)
		{
			printf("%d ", LOTTO_results.lottoTab2from12[i]);
		}
		printf("\n");
	}
	printf("\n\n");

    TEST_FAIL_MESSAGE("Before you will win buy generated ticket!!!")
}






TEST(lotto, empty_test_fail)
{
	TEST_FAIL_MESSAGE("Implement your test!");
}




