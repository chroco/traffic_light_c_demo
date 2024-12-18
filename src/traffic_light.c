#include "traffic_light.h"

// state machine transition table
const state_transition_t table[] = {
/* curr     input      next */
  {red,		  ok,       green},
  {red,     halt,      idle},
  {green,   ok,      yellow},
  {green,   halt,      idle},
  {yellow,  ok,         red},
  {yellow,  halt,      idle},
  {idle,    ok,    			red},
	{idle,    halt,      quit},
  {idle,    repeat,    idle},
};

state_t (* state[])(int) = {
	red_state, 
	green_state, 
	yellow_state,
	idle_state,
	quit_state,
	invalid_state,
};

const size_t state_transision_size = sizeof(state_transition_t);
const size_t table_size = sizeof(table);
const uint16_t table_entries = table_size / state_transision_size;

static input_t input = ok;

void wait(const char *state_str, int delay_in_seconds)
{
	assert(state_str != NULL);
	printf("%s (%d seconds)\n", state_str, delay_in_seconds);
	sleep(delay_in_seconds);
}

state_t red_state(int delay_in_seconds)
{
	wait("red\0", delay_in_seconds);
		
	return red;
}

state_t green_state(int delay_in_seconds)
{
	wait("green\0", delay_in_seconds);
	
	return green;
}

state_t yellow_state(int delay_in_seconds)
{
	wait("yellow\0", delay_in_seconds);
	
	return yellow;
}

state_t idle_state(int delay_in_seconds)
{
	input = repeat; // stay in idle once selected
	wait("idle\0", delay_in_seconds);
	return idle;
}

state_t quit_state(int delay_in_seconds)
{
	wait("quit\0", delay_in_seconds);

	return quit;
}

state_t invalid_state(int delay_in_seconds)
{
	//wait(delay_in_seconds);
	printf("invalid\n");

	return invalid;
}

state_t lookup_transitions(state_t cur_state, input_t input)
{	
	for (int i = 0; i < table_entries; ++i) 
	{
		if(table[i].cur_state == cur_state && table[i].input == input)
		{
//			printf("found table entry, input: %u, %u, ", cur_state, input);
//			printf("next_state: %u\n", table[i].next_state);
			return table[i].next_state;
		}
	}
	printf("[ERROR]: invalid state transition\n");
	return invalid;
}

int get_signal_time(state_t cur_state)
{
	int signal_time = 0;

	switch(cur_state)
	{
		case red:
			signal_time = RED_LIGHT_TIME;
			break;
		case green:
			signal_time = GREEN_LIGHT_TIME;
			break;
		case yellow:
			signal_time = YELLOW_LIGHT_TIME;
			break;
		case idle:
			signal_time = IDLE_TIME;
			break;
		case quit:
			signal_time = QUIT_TIME;
			break;
		case invalid:
		default: 
			signal_time = -1;
			break;
	}

	return signal_time;
}

int traffic_light_fsm() 
{
  state_t cur_state = ENTRY_STATE;
  state_t (* state_func)(int);
	input_t good_input = input;
	
	assert(good_input == ok);

  for (;;) 
	{
    if (cur_state == EXIT_STATE)
		{
      break;
		}
    
		state_func = state[cur_state];
		int signal_time = get_signal_time(cur_state);
    state_func(signal_time);
	
		state_t check_state;

		do 
		{
	    check_state = lookup_transitions(cur_state, input);
			
			if(check_state == quit)
			{
				exit(EXIT_SUCCESS);
			}

			if(check_state == invalid)
			{
				input = good_input;
			}
		} while (check_state == invalid);

		good_input = input;
		cur_state = check_state;

		assert(cur_state != invalid);
  }

  return EXIT_SUCCESS;
}

static void *run(void *arg) 
{
	assert(arg == NULL);	
	
	traffic_light_fsm();
	
	return NULL;
}

pthread_t thread;

int start_traffic_light()
{
	int ret = pthread_create(&thread, NULL, run, NULL);
	if(ret != 0)
	{
		printf("[ERROR]: %d pthread_create\n", ret);
		return ret;
	}

	return 0;
}

int stop_traffic_light()
{
	int ret = pthread_join(thread, NULL);
	if(ret != 0)
	{
		printf("[ERROR]: %d pthread_join\n", ret);
		return ret;
	}

	return 0;
}

int start_socket_server()
{
	int server_socket;
	int client_socket;
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;

	unlink(SOCKET_NAME); 
	server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCKET_NAME);

	int slen = sizeof(server_addr);

	bind(server_socket, (struct sockaddr *) &server_addr, slen);

	listen(server_socket, 5);

	for(;;)
	{
		unsigned int clen = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
		
		char buffer[BUFFER_SIZE];
		read(client_socket, buffer, BUFFER_SIZE);
		printf("recv: %s from client!\n", buffer);

		if (strncmp(buffer, "ok", BUFFER_SIZE) == 0)
		{
			input = ok;	
		}
		else if (strncmp(buffer, "halt", BUFFER_SIZE) == 0)
		{
			input = halt;	
		}
		else if	(strncmp(buffer, "repeat", BUFFER_SIZE) == 0)
		{
			input = repeat;	
		}
		else
		{
			printf("[ERROR]: Invalid command received %s, ignoring...\n", buffer);
			strncpy(buffer, "invalid command", BUFFER_SIZE);
		}
		
		strncat(buffer, " received", strlen(" received") + 1);

		write(client_socket, buffer, BUFFER_SIZE);
		printf("send: %s from client!\n", buffer);

		close(client_socket);
	}
	
	return 0;
}

