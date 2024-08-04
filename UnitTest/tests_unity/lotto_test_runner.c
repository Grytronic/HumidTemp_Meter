#include "../Frameworks/unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(lotto)
{
   /* Test cases to run */
   RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab5from50initialisedWithZero);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithInRangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithUpperLimitValue);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithPositiveOverrangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOutWithNegativeOverrangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate5from50NumbersBufforsAreFilledOut2timesWithTheSameValues);

   RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab2from12initialisedWithZero);
   RUN_TEST_CASE(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithInRangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithUpperLimitValue);
   RUN_TEST_CASE(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithPositiveOverrangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate2from12NumbersBufforsAreFilledOutWithNegativeOverrangeValues);
   RUN_TEST_CASE(lotto, WhenGenerate2from12NumbersBufforsAreFilledOut2timesWithTheSameValues);

   RUN_TEST_CASE(lotto, GenerateLOTTOnumbersToBeMilionaire);
}
