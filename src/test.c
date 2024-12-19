#include "test.h"

typedef state_t (*state_function)(void);
/*
int test_state_function(state_function func, state_t expected_state)
{
	TEST_ASSERT_STATE_EQ(func(), expected_state);
	return 0;
}
//*/

int unit_test_state_functions()
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

/*
static void *run(void *arg) 
{
	assert(arg == NULL);	
	
	start_socket_server();
	
	return NULL;
}
//*/

int integration_test_ping_loop(void)
{

	printf("Starting integration testing\n");

	pthread_t traffic_light_thread;
	pthread_t socket_server_thread;

	start_thread(traffic_light_thread, traffic_light_fsm);
	start_thread(socket_server_thread, start_socket_server);

	for (int i = 0; i < 5; ++i)
	{
		system("./send_cmd ok");
		usleep(100000);
	}	

	stop_thread(traffic_light_thread);
	stop_thread(socket_server_thread);

	return 0;
}

int generic_test(void)
{
	printf("generic_test o_0... \n");	
	
	for (int i = 0; i < 100; ++i)
	{
		usleep(100000);
	}

	printf("passed generic_test!\n");	
	
	return 0;
}
