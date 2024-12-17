#ifndef _TRAFFIC_LIGHT_H_
#define _TRAFFIC_LIGHT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#define ENTRY_STATE red
#define EXIT_STATE quit

// time in seconds
#define RED_LIGHT_TIME 	   5
#define GREEN_LIGHT_TIME   5
#define YELLOW_LIGHT_TIME  2

typedef enum {
	red, 
	green, 
	yellow, 
	idle,
	quit,
	invalid
} state_t;

typedef enum {
	ok, 
	off
} input_t;

state_t red_state(void);
state_t green_state(void);
state_t yellow_state(void);
state_t idle_state(void);
state_t quit_state(void);
state_t invalid_state(void);

typedef struct {
  state_t cur_state;
  input_t input;
  state_t next_state;
} state_transition_t;

state_t lookup_transitions(state_t, input_t);
uint16_t traffic_light_fsm();

#endif
