#include "traffic_light.h"


// state machine transition table
state_transition_t table[] = {
// curr     input    next
  {red,		  ok,     green},
  {red,     halt,    idle},
  {green,   ok,    yellow},
  {green,   halt,    idle},
  {yellow,  ok,       red},
  {yellow,  halt,    idle},
  {idle,    ok,      idle},
  {idle,    halt,    quit},
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
			signal_time = -1;
			break;
		default:
			break;
	}

	return signal_time;
}

int traffic_light_fsm() 
{
  state_t cur_state = ENTRY_STATE;
  input_t input = ok;
  state_t (* state_func)(int);

  for (;;) 
	{
    if (cur_state == EXIT_STATE)
		{
      break;
		}
    
		state_func = state[cur_state];
		int signal_time = get_signal_time(cur_state);
    state_func(signal_time);
    cur_state = lookup_transitions(cur_state, input);
		assert(cur_state != invalid);
  }

  return EXIT_SUCCESS;
}
