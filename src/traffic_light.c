#include "traffic_light.h"

state_timeing_t state_timing = {
	.red_light_duration_in_seconds = 5,
	.green_light_duration_in_seconds = 5,
	.yellow_light_duration_in_seconds = 2, 
	.idle_duration_in_seconds = 2
};
 
/*
 * state machine transition table
 */
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

static input_t mealy_input = ok;
pthread_mutex_t lock;	

/*
 *
 */
void set_mealy_input(input_t input)
{
	pthread_mutex_lock(&lock);	
	mealy_input =  input;
	pthread_mutex_unlock(&lock);	
}

/*
 *
 */
input_t get_mealy_input(void)
{
	pthread_mutex_lock(&lock);
	input_t input = mealy_input;
	pthread_mutex_unlock(&lock);

	return input;
}

/*
 *
 */
void wait(const char *state_str, int delay_in_seconds)
{
	assert(state_str != NULL);
	printf("%s (%d seconds)\n", state_str, delay_in_seconds);
	
	for (int i = 0; i < delay_in_seconds; ++i)
	{
		sleep(1);
	}
}

/*
 *
 */
state_t red_state(int delay_in_seconds)
{
	wait("red\0", delay_in_seconds);
		
	return red;
}

/*
 *
 */
state_t green_state(int delay_in_seconds)
{
	wait("green\0", delay_in_seconds);
	
	return green;
}

/*
 *
 */
state_t yellow_state(int delay_in_seconds)
{
	wait("yellow\0", delay_in_seconds);
	
	return yellow;
}

/*
 *
 */
state_t idle_state(int delay_in_seconds)
{ 
	set_mealy_input(repeat);
	wait("idle\0", delay_in_seconds);
	return idle;
}

state_t quit_state(int delay_in_seconds)
{
	wait("quit\0", delay_in_seconds);

	return quit;
}

/*
 *
 */
state_t invalid_state(int delay_in_seconds)
{
	(void)delay_in_seconds;
	
	printf("invalid\n");

	return invalid;
}

/*
 *
 */
state_t lookup_transitions(state_t cur_state, input_t input)
{	
	for (int i = 0; i < table_entries; ++i) 
	{
		if(table[i].cur_state == cur_state && table[i].input == input)
		{
			return table[i].next_state;
		}
	}

	printf("[ERROR]: invalid state transition\n");
	
	return invalid;
}

/*
 *
 */
int get_signal_time(state_t cur_state)
{
	int signal_time = 0;

	switch(cur_state)
	{
		case red:
			signal_time = get_red_duration_in_seconds();
			break;
		case green:
			signal_time = get_green_duration_in_seconds();
			break;
		case yellow:
			signal_time = get_yellow_duration_in_seconds();
			break;
		case idle:
			signal_time = get_idle_duration_in_seconds();
			break;
		case quit:
		case invalid:
		default: 
			signal_time = -1;
			break;
	}

	return signal_time;
}

/*
 *
 */
int traffic_light_fsm() 
{
  state_t cur_state = ENTRY_STATE;
  state_t (* state_func)(int);
	input_t good_input = get_mealy_input();
	input_t unchecked_input = 0;
	
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
			unchecked_input = get_mealy_input();

	    check_state = lookup_transitions(cur_state, unchecked_input);
			
			if(check_state == quit)
			{
				exit(EXIT_SUCCESS);
			}

			if(check_state == invalid)
			{
				set_mealy_input(good_input);
			}
		} while (check_state == invalid);

		good_input = get_mealy_input();
		
		cur_state = check_state;

		assert(cur_state != invalid);
  }

  return EXIT_SUCCESS;
}

/*
 *
 */
void *run(void *arg) 
{
	assert(arg != NULL);	

	void (*func_ptr)(void) = NULL;
	func_ptr = arg;

	func_ptr();
	
	return NULL;
}

/*
 *
 */
int start_thread(pthread_t thread, void *arg)
{
	assert(arg != NULL);	
	
	int ret = pthread_create(&thread, NULL, run, arg);
	if(ret != 0)
	{
		printf("[ERROR]: %d pthread_create\n", ret);
		return ret;
	}

	return 0;
}

/*
 *
 */
int stop_thread(pthread_t thread)
{
	pthread_attr_t attr;
  int chk;
	
	pthread_attr_getdetachstate(&attr, &chk);	

	if(chk == PTHREAD_CREATE_DETACHED) 
	{
		return 0;
	}

	int ret = pthread_join(thread, NULL);
	if(ret != 0)
	{
		printf("[ERROR]: %d pthread_join\n", ret);
		return ret;
	}

	return 0;
}

pthread_mutex_t duration_lock;

/*
 *
 */
uint32_t get_red_duration_in_seconds(void)
{
	pthread_mutex_lock(&duration_lock);
	uint32_t duration = state_timing.red_light_duration_in_seconds;
	pthread_mutex_unlock(&duration_lock);

	return duration;
}

/*
 *
 */
uint32_t get_green_duration_in_seconds(void)
{
	pthread_mutex_lock(&duration_lock);
	uint32_t duration = state_timing.green_light_duration_in_seconds;
	pthread_mutex_unlock(&duration_lock);

	return duration;
}

/*
 *
 */
uint32_t get_yellow_duration_in_seconds(void)
{
	pthread_mutex_lock(&duration_lock);
	uint32_t duration = state_timing.yellow_light_duration_in_seconds;
	pthread_mutex_unlock(&duration_lock);

	return duration;
}

/*
 *
 */
uint32_t get_idle_duration_in_seconds(void)
{
	pthread_mutex_lock(&duration_lock);
	uint32_t duration = state_timing.idle_duration_in_seconds; 
	pthread_mutex_unlock(&duration_lock);

	return duration; 
}

/*
 *
 */
void set_red_duration_in_seconds(int duration)
{
	pthread_mutex_lock(&duration_lock);
	state_timing.red_light_duration_in_seconds = duration; 
	pthread_mutex_unlock(&duration_lock);
}

/*
 *
 */
void set_green_duration_in_seconds(int duration)
{
	pthread_mutex_lock(&duration_lock);
	state_timing.green_light_duration_in_seconds = duration; 
	pthread_mutex_unlock(&duration_lock);
}

/*
 *
 */
void set_yellow_duration_in_seconds(int duration)
{
	pthread_mutex_lock(&duration_lock);
	state_timing.yellow_light_duration_in_seconds = duration; 
	pthread_mutex_unlock(&duration_lock);
}

/*
 *
 */
void set_idle_duration_in_seconds(int duration)
{
	pthread_mutex_lock(&duration_lock);
	state_timing.idle_duration_in_seconds = duration; 
	pthread_mutex_unlock(&duration_lock);
}

/*
 *
 */
int start_socket_server()
{
	int server_socket;
	int client_socket;
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;
  const char delimiter[] = " ";

	server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCKET_NAME);

	int slen = sizeof(server_addr);

	//unlink(SOCKET_NAME); 
	bind(server_socket, (struct sockaddr *) &server_addr, slen);

	listen(server_socket, 5);

	for(;;)
	{
		unsigned int clen = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
	
		input_t temp_input = get_mealy_input();

		const int arg_length = 20;
		char arg[2][arg_length];
		char buffer[BUFFER_SIZE];
		
		read(client_socket, buffer, BUFFER_SIZE);
		printf("recv: %s from client!\n", buffer);
		fflush(stdout);

    char *token = strtok(buffer, delimiter);
		strncpy(arg[0], token, arg_length);
		printf("arg[0]: %s\n", arg[0]);

		token = strtok(NULL, delimiter);
		if(token != NULL)
		{
			strncpy(arg[1], token, arg_length);
			printf("arg[1]: %s\n", arg[1]);
		}

		if (strncmp(arg[0], "ok", BUFFER_SIZE) == 0)
		{
			temp_input = ok;	
		}
		else if (strncmp(arg[0], "halt", BUFFER_SIZE) == 0)
		{
			temp_input = halt;	
		}
		else if	(strncmp(arg[0], "repeat", BUFFER_SIZE) == 0)
		{
			temp_input = repeat;	
		}
		else if	(strncmp(arg[0], "red", BUFFER_SIZE) == 0)
		{
			int duration = atoi(arg[1]);
			if (duration == 0)
			{
				duration = 1;
			}
			set_red_duration_in_seconds(duration);
		}
		else if	(strncmp(arg[0], "green", BUFFER_SIZE) == 0)
		{
			int duration = atoi(arg[1]);
			if (duration == 0)
			{
				duration = 1;
			}
			set_green_duration_in_seconds(duration);
		}
		else if	(strncmp(arg[0], "yellow", BUFFER_SIZE) == 0)
		{
			int duration = atoi(arg[1]);
			if (duration == 0)
			{
				duration = 1;
			}
			set_yellow_duration_in_seconds(duration);
		}
		else if	(strncmp(arg[0], "idle", BUFFER_SIZE) == 0)
		{
			int duration = atoi(arg[1]);
			if (duration == 0)
			{
				duration = 1;
			}
			set_idle_duration_in_seconds(duration);
		}
		else
		{
			printf("[ERROR]: Invalid command received %s, ignoring...\n", buffer);
			strncpy(buffer, "invalid command", BUFFER_SIZE);
		}
	
		set_mealy_input(temp_input);

		strncat(buffer, " received", strlen(" received") + 1);

		write(client_socket, buffer, BUFFER_SIZE);
		printf("send: %s from client!\n", buffer);

		close(client_socket);
	}
	
	return 0;
}

