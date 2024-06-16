#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(lotto)
{
   /* Test cases to run */
   RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab5from50initialisedWithZeros);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithInRangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithUpperLimitValue);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithPositiveOverrangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithNegativeOverrangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithTheSameValues);

   //RUN_TEST_CASE(lotto,);
   //RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab2from12initialisedWithZeros);
   //RUN_TEST_CASE(lotto, WhenGenerate2of12NumbersBufforsAreFilledOutWithInRangeValues);
}
