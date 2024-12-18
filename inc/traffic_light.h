#ifndef _TRAFFIC_LIGHT_H_
#define _TRAFFIC_LIGHT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_NAME "/tmp/traffic_light_socket"

#define ENTRY_STATE red
#define EXIT_STATE quit

// time in seconds
#define RED_LIGHT_TIME 	   5
#define GREEN_LIGHT_TIME   5
#define YELLOW_LIGHT_TIME  2
#define IDLE_TIME 				 1
#define QUIT_TIME 				 0

#define BUFFER_SIZE 100

/*
 * Moore is less and Mealy is more...
 */

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
	halt,
	repeat
} input_t;

state_t red_state(int);
state_t green_state(int);
state_t yellow_state(int);
state_t idle_state(int);
state_t quit_state(int);
state_t invalid_state(int);

typedef struct {
  state_t cur_state;
  input_t input;
  state_t next_state;
} state_transition_t;

void wait(const char *, int);
int get_signal_time(state_t);
state_t lookup_transitions(state_t, input_t);
int traffic_light_fsm(void);
int start_traffic_light(void);
int stop_traffic_light(void);
int start_socket_server(void);

#endif
