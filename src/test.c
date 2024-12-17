#include "test.h"

typedef state_t (*state_function)(void);
/*
int test_state_function(state_function func, state_t expected_state)
{
	TEST_ASSERT_STATE_EQ(func(), expected_state);
	return 0;
}
//*/

int test_state_functions()
{
	printf("unit testing state functions: \n\n");
	
	TEST_ASSERT_STATE_EQ(red_state(0), red);
	TEST_ASSERT_STATE_EQ(green_state(0), green);
	TEST_ASSERT_STATE_EQ(yellow_state(0), yellow);
	TEST_ASSERT_STATE_EQ(quit_state(0), quit);
	TEST_ASSERT_STATE_EQ(invalid_state(0), invalid);
	
	TEST_ASSERT_STATE_NOT(red_state(0), invalid);
	TEST_ASSERT_STATE_NOT(green_state(0), invalid);
	TEST_ASSERT_STATE_NOT(yellow_state(0), invalid);
	TEST_ASSERT_STATE_NOT(quit_state(0), invalid);
	TEST_ASSERT_STATE_NOT(invalid_state(0), quit);

	printf("\nAll test successful! \n");
	return 0;
}
