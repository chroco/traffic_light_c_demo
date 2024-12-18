#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "traffic_light.h"

int main(int argc, char *argv[])
{
	printf("usage: ./commander <command>\n");
	printf("accepted commands: go, halt, repeat\n");
	
	if(argc < 2)
	{
		printf("invalid input!\n");

		return 1;
	}

	int server_socket;
	struct sockaddr_un server_addr;
	int connection_result;

	server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCKET_NAME);

	connection_result = connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if (connection_result == -1) 
	{
		printf("[ERROR]: Can't connect to socket %s!\n", server_addr.sun_path);
		return 1;
	}
	
	char buffer[BUFFER_SIZE];

	strncpy(buffer, argv[1], BUFFER_SIZE);
	printf("sending: %s \n", buffer);
	write(server_socket, buffer, BUFFER_SIZE);

	read(server_socket, buffer, BUFFER_SIZE);
	printf("%s\n", buffer);

	close(server_socket);

	return 0;
}
