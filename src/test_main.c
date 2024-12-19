#include "test.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf("Starting test harness\n");
	
	unit_test_state_functions();
	integration_test_ping_loop();
	generic_test();

	return 0;
}
