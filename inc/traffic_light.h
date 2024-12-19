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
#include <signal.h>
#include <errno.h>

#define CLIENT_CONNECTION_ATTEMPTS  20
#define SOCKET_NAME "./traffic_light_socket"
//#define SOCKET_NAME "/tmp/traffic_light_socket"

#define ENTRY_STATE red
#define EXIT_STATE quit

#define BUFFER_SIZE 100

typedef struct {
	// time in seconds
	uint32_t red_light_duration_in_seconds;
	uint32_t green_light_duration_in_seconds;
	uint32_t yellow_light_duration_in_seconds;
	uint32_t idle_duration_in_seconds;
} state_timeing_t;

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

/*
 * state_t <name>_state(int)
 *
 *
 *
 */
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

/*
 *
 */
void set_mealy_input(input_t);

/*
 *
 */
input_t get_mealy_input(void);

/*
 *
 */
void wait(const char *, int);

/*
 *
 */
int get_signal_time(state_t);

/*
 *
 */
state_t lookup_transitions(state_t, input_t);

/*
 *
 */
int traffic_light_fsm(void);

/*
 *
 */
int start_traffic_light(void);

/*
 *
 */
int stop_traffic_light(void);

/*
 *
 */
int start_socket_server(void);

/*
 *
 */
void *run(void *);

/*
 *
 */
int start_thread(pthread_t, void *);

/*
 *
 */
int stop_thread(pthread_t);
	
/*
 *
 */
uint32_t get_red_duration_in_seconds(void);

/*
 *
 */
uint32_t get_green_duration_in_seconds(void);
	
/*
 *
 */
uint32_t get_yellow_duration_in_seconds(void);
	
/*
 *
 */
uint32_t get_idle_duration_in_seconds(void);

/*
 *
 */
void set_red_duration_in_seconds(int);

/*
 *
 */
void set_green_duration_in_seconds(int);
	
/*
 *
 */
void set_yellow_duration_in_seconds(int);
	
/*
 *
 */
void set_idle_duration_in_seconds(int);



#endif
