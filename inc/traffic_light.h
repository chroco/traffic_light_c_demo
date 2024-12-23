#ifndef _TRAFFIC_LIGHT_H_
#define _TRAFFIC_LIGHT_H_

/*
 * Moore is less and Mealy is more...
 */

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
#include <stdbool.h>

#define SOCKET_NAME                 "./tmp/traffic_light_socket"
#define CLIENT_CONNECTION_ATTEMPTS  20
#define MAX_LIGHT_DURATION          1000
#define MIN_LIGHT_DURATION          1
#define ENTRY_STATE                 red
#define EXIT_STATE quit
#define BUFFER_SIZE                 100

/*
 * time duration for states
 */
typedef struct {
  // time in seconds
  uint32_t red_light_duration_in_seconds;
  uint32_t green_light_duration_in_seconds;
  uint32_t yellow_light_duration_in_seconds;
  uint32_t idle_duration_in_seconds;
} state_timeing_t;

/*
 * fsm states
 */
typedef enum {
  red,
  green,
  yellow,
  idle,
  quit,
  invalid
} state_t;

/*
 * inputs
 */
typedef enum {
  ok,
  halt,
  repeat
} input_t;

/*
 * state functions
 */
state_t red_state(int);
state_t green_state(int);
state_t yellow_state(int);
state_t idle_state(int);
state_t quit_state(int);
state_t invalid_state(int);

/*
 * state transitions
 */
typedef struct {
  state_t cur_state;
  input_t input;
  state_t next_state;
} state_transition_t;

/*
 * set_thread_running - the thread_running atomic_bool
 * is used for threads knowing when to die
 */
void set_thread_running(bool);

/*
 * get_thread_running - 
 */
bool get_thread_running(void);

/*
 * set_mealy_input - mutex pretected set
 *
 * @input_t: desired input {ok | halt | repeat}
 *
 */
void set_mealy_input(input_t);

/*
 * get_mealy_input - mutex pretected get
 *
 * @return: mealy_input
 */
input_t get_mealy_input(void);

/*
 * wait - waits
 *
 * @state_str: { "red" | "green" | "yellow" | "idle" }
 *
 * @delay_in_seonds:
 *
 */
void wait(const char *, int);

/*
 * get_light_duration - get a lights duration
 *
 * @state_t;
 *
 * @return;
 */
int get_light_duration(state_t);


/*
 * get_lookup_transitions - returns the next state based
 * on current state and input
 *
 * @state_t; current state
 *
 * @input_t: {ok | halt | repeat}
 *
 * return: next state, quit, invalid
 */

state_t lookup_transitions(state_t, input_t);

/*
 * traffic_light_fsm - entry for finite state machine
 */
int traffic_light_fsm(void);

/*
 * start_traffic_light - entry for the traffic light thread
 */
int start_traffic_light(void);

/*
 * check_duration - checks user input for a valid time duration
 */
uint32_t check_duration(char *);

/*
 * start_socket_server -  entry for unix socket server in a thread
 */
int start_socket_server(void);

/*
 * start_thread - starts a thread
 */
int start_thread(pthread_t, void *);

/*
 * stop_thread - cleans up a thread
 */
int stop_thread(pthread_t);

/*
 * get_red_duration_in_seconds -
 */
uint32_t get_red_duration_in_seconds(void);

/*
 * get_green_duration_in_seconds -
 */
uint32_t get_green_duration_in_seconds(void);

/*
 * get_yellow_duration_in_seconds -
 */
uint32_t get_yellow_duration_in_seconds(void);

/*
 * get_idle_duration_in_seconds -
 */
uint32_t get_idle_duration_in_seconds(void);

/*
 * set_red_duration_in_seconds -
 */
void set_red_duration_in_seconds(int);

/*
 * set_green_duration_in_seconds -
 */
void set_green_duration_in_seconds(int);

/*
 * set_yellow_duration_in_seconds -
 */
void set_yellow_duration_in_seconds(int);

/*
 * set_idle_duration_in_seconds -
 */
void set_idle_duration_in_seconds(int);

#endif
