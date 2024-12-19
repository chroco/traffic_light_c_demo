#include "test.h"

/*
 *
 */
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
 *
 */
int integration_test_ping_loop(void)
{
	printf("Starting integration testing\n");

	pthread_t traffic_light_thread;
	pthread_t socket_server_thread;

	start_thread(traffic_light_thread, traffic_light_fsm);
	start_thread(socket_server_thread, start_socket_server);

//*
	{
		char cmd[] = "./send_cmd ok\0";
		const int cycles = 5;
		
		printf("calling: %s\n", cmd);
		
		for (int i = 0; i < cycles; ++i)
		{
			int ret = system(cmd);
			assert(ret == 0);
			
			usleep(100000);
		}
	}
//*/

//*
	{
		printf("\n");
		char cmd[] = "./send_cmd red 3\0";
		int ret = system(cmd);
		assert(ret == 0);
		printf("\n");
	}
//*/

//*
	{
		printf("\n");
		char cmd[] = "./send_cmd green 3\0";
		int ret = system(cmd);
		assert(ret == 0);
		printf("\n");
	}
//*/

//*
	{
		printf("\n");
		char cmd[] = "./send_cmd yellow 1\0";
		int ret = system(cmd);
		assert(ret == 0);
		printf("\n");
	}
//*/

//*
	{
		printf("\n");
		char cmd[] = "./send_cmd idle 1\0";
		int ret = system(cmd);
		assert(ret == 0);
		printf("\n");
	}
//*/

//*
	{
		printf("\n");
		char cmd[] = "./send_cmd foo bar\0";
		int ret = system(cmd);
		assert(ret == 0);
		printf("\n");
	}
//*/

//*
	{
		printf("\n");
		char cmd[] = "./send_cmd red bg0\0";
		int ret = system(cmd);
		assert(ret == 0);
		printf("\n");
	}
//*/

//*
	{
		char cmd[] = "./send_cmd halt\0";
		const int cycles = 5;
		
		for (int i = 0; i < cycles; ++i)
		{
			printf("calling: %s\n", cmd);
			
			int ret = system(cmd);
			assert(ret == 0);
			
			usleep(100000);
		}

	}
//*/

	stop_thread(traffic_light_thread);
	stop_thread(socket_server_thread);

	return 0;
}

/*
 *
 */
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
