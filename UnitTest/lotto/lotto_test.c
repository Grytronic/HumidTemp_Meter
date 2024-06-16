#include "unity/fixture/unity_fixture.h"

#include "fake/fake_lotto.h"
#include "../App/Lotto/lotto.h"

#define LOTTO_5FROM50_OUT_RANGE 51U
#define LOTTO_2FROM12_OUT_RANGE 13U

Lotto_t test_LOTTO_results;



void update_test_LOTTO_values_to_last_generated(void)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		test_LOTTO_results.lottoTab5from50[i] = PERIPHERALS_get_generated_random_value();
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		test_LOTTO_results.lottoTab2from12[i] = PERIPHERALS_get_generated_random_value();
	}
}

void set_generated_values_and_update_test_LOTTO_results(int val)
{
	PERIPHERALS_set_value_for_generated_random_value(val);
	LOTTO_generate_numbers();
	update_test_LOTTO_values_to_last_generated();
}


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
	set_generated_values_and_update_test_LOTTO_results(0);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab2from12, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, WhenNoGenerationNumbersTab5from50initialisedWithZeros)
{
	set_generated_values_and_update_test_LOTTO_results(0);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate5of50NumbersBufforsAreFilledOutWithRandomValues)
{
	set_generated_values_and_update_test_LOTTO_results(34);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab5from50, LOTTO_results.lottoTab5from50, 5);
}

TEST(lotto, WhenGenerate2of12NumbersBufforsAreFilledOutWithRandomValues)
{
	set_generated_values_and_update_test_LOTTO_results(6);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_LOTTO_results.lottoTab2from12, LOTTO_results.lottoTab2from12, 2);
}

TEST(lotto, NextTest)
{
	TEST_FAIL_MESSAGE("Implement your test!");
}

// generater over range
// if generated values < 0, result should be not valid
// check what happens on the age of range - gen 0, -40... generate one more time, gen 51



