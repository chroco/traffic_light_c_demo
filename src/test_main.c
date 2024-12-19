#include "test.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf("Starting test harness\n");
	
	unlink(SOCKET_NAME); 
	char buffer[50] = "rm \0";
	
	strncat(buffer, SOCKET_NAME, strlen(SOCKET_NAME) + 1);
	printf("%s \n", buffer);
	
	//int ret = system(buffer);
	//assert(ret == 0);

	unit_test_state_functions();
	integration_test_ping_loop();
	generic_test();

	return 0;
}
