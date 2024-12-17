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
	printf("testing state functions: \n\n");
	
	TEST_ASSERT_STATE_EQ(red_state(), red);
	TEST_ASSERT_STATE_EQ(green_state(), green);
	TEST_ASSERT_STATE_EQ(yellow_state(), yellow);
	TEST_ASSERT_STATE_EQ(quit_state(), quit);
	TEST_ASSERT_STATE_EQ(invalid_state(), invalid);
	
	TEST_ASSERT_STATE_NOT(red_state(), invalid);
	TEST_ASSERT_STATE_NOT(green_state(), invalid);
	TEST_ASSERT_STATE_NOT(yellow_state(), invalid);
	TEST_ASSERT_STATE_NOT(quit_state(), invalid);
	TEST_ASSERT_STATE_NOT(invalid_state(), quit);

	printf("completed testing with no errors! \n\n");
	return 0;
}
