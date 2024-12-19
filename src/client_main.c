#include "traffic_light.h"

/*
 * traffic light simulation client
 */

void help(void)
{
	printf("invalid input!\n\n");
	printf("usage:\n");
	printf("./send_cmd {ok | halt | repeat }\n");
	printf("./send_cmd {red | green | yellow | idle } {time_delay_in_seconds}\n");
}

int main(int argc, char *argv[])
{
	int server_socket;
	struct sockaddr_un server_addr;
	int connection = -1;
	char buffer[BUFFER_SIZE];
	
	switch (argc)
	{
		case 2: 
			// ./send_cmd {input_t: ok | halt | repeat }
			assert(argv[1]);
			strncpy(buffer, argv[1], BUFFER_SIZE);
			break;
		
		case 3:
			// ./send_cmd {red | green | yellow | idle } {time_delay_in_seconds}
			strncpy(buffer, argv[1], BUFFER_SIZE);
			strncat(buffer, " \0", sizeof("yellow"));
			strncat(buffer, argv[2], sizeof("yellow "));
			printf("sending: %s \n", buffer);
			break;
		
		default:
			// o_0
			help();
			return 1;
	}

	server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCKET_NAME);
	
	int attempts = 0;
	
	for(; connection == -1; ++attempts )
	{
		if (attempts >= CLIENT_CONNECTION_ATTEMPTS)
		{
			printf("[ERROR]: Can't connect to socket %s!\n", server_addr.sun_path);
			return 1;
		}
		
		connection = connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
		
		usleep(100000);
	}

	printf("Connection attempts: %d\n", attempts);

	printf("sending: %s \n", buffer);
	write(server_socket, buffer, BUFFER_SIZE);

	read(server_socket, buffer, BUFFER_SIZE);
	printf("%s\n", buffer);

	close(server_socket);

	exit(0);
}
