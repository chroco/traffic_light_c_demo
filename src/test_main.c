#include "test.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	printf("Starting test harness\n");

	unit_test_state_functions();
	
	generic_test();
	
	integration_testing();

	return 0;
}
