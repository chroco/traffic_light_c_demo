#include "traffic_light.h"

/*
 *  Traffic light simulation and socket server main
 */

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  printf("Starting traffic light simulation\n");

  pthread_t traffic_light_thread;
  pthread_t socket_server_thread;

  start_thread(traffic_light_thread, traffic_light_fsm);
  start_thread(socket_server_thread, start_socket_server);

  stop_thread(traffic_light_thread);
  stop_thread(socket_server_thread);

  return 0;
}
