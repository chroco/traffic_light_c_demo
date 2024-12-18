#include "traffic_light.h"

int main(int argc, char *argv[])
{
	printf("Starting traffic light simulation\n");

	start_traffic_light();
	
	start_socket_server();

	stop_traffic_light();

	return 0;
}
