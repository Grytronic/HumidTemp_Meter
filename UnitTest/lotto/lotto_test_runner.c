#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(lotto)
{
   /* Test cases to run */
   RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab2from12initialisedWithZeros);
   RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab5from50initialisedWithZeros);
   RUN_TEST_CASE(lotto, WhenGenerate5of50NumbersBufforsAreFilledOutWithRandomValues);
   RUN_TEST_CASE(lotto, WhenGenerate2of12NumbersBufforsAreFilledOutWithRandomValues);
   //RUN_TEST_CASE(lotto, NextTest);
}
