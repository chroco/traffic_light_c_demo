#include "traffic_light.h"

state_transition_t table[] = {
  {red,		  ok,    green},
  {red,     off,    idle},
  {green,   ok,   yellow},
  {green,   off,    idle},
  {yellow,  ok,      red},
  {yellow,  off,    idle},
  {idle,    ok,     idle},
  {idle,    off,    quit},
};

state_t (* state[])(void) = {
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

state_t red_state(void)
{
	printf("red (%d seconds)\n",RED_LIGHT_TIME);

	sleep(RED_LIGHT_TIME);

	return red;
}

state_t green_state(void)
{
	printf("green (%d seconds)\n",GREEN_LIGHT_TIME);
	
	sleep(GREEN_LIGHT_TIME);

	return green;
}

state_t yellow_state(void)
{
	printf("yellow (%d seconds)\n",YELLOW_LIGHT_TIME);
	
	sleep(YELLOW_LIGHT_TIME);

	return yellow;
}

state_t idle_state(void)
{
	printf("idle\n");

	return idle;
}

state_t quit_state(void)
{
	printf("quit\n");

	return quit;
}

state_t invalid_state(void)
{
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

uint16_t traffic_light_fsm() 
{
  state_t cur_state = ENTRY_STATE;
  input_t input = ok;
  state_t (* state_func)(void);

  for (;;) 
	{
    if (cur_state == EXIT_STATE)
		{
      break;
		}

    state_func = state[cur_state];
    state_func();
    cur_state = lookup_transitions(cur_state, input);
		assert(cur_state != invalid);
  }

  return EXIT_SUCCESS;
}
