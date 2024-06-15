#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(lotto)
{
   /* Test cases to run */
   RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab2from12EqualZeros);
   RUN_TEST_CASE(lotto, WhenNoGenerationNumbersTab5from50EqualZeros);
   RUN_TEST_CASE(lotto, WhenGenerateNumbersBufforsAreFilledOutWithRandomValues);
   RUN_TEST_CASE(lotto, NextTest);
}
