#include "test.h"

/*
 *
 */
int unit_test_state_functions()
{
  printf("unit testing state functions: \n\n");

  TEST_ASSERT_STATE_EQ(red_state(0), red);
  TEST_ASSERT_STATE_EQ(green_state(0), green);
  TEST_ASSERT_STATE_EQ(yellow_state(0), yellow);
  TEST_ASSERT_STATE_EQ(quit_state(0), quit);
  TEST_ASSERT_STATE_EQ(invalid_state(0), invalid);

  TEST_ASSERT_STATE_NOT(red_state(0), invalid);
  TEST_ASSERT_STATE_NOT(green_state(0), invalid);
  TEST_ASSERT_STATE_NOT(yellow_state(0), invalid);
  TEST_ASSERT_STATE_NOT(quit_state(0), invalid);
  TEST_ASSERT_STATE_NOT(invalid_state(0), quit);

  printf("\nAll unit tests successful! \n\n");
  return 0;
}

/*
 * integration testing
 *
 * break lots of things
 */
int integration_testing(void)
{
  printf("Starting integration testing\n");

  pthread_t traffic_light_thread;
  pthread_t socket_server_thread;

  start_thread(traffic_light_thread, traffic_light_fsm);
  start_thread(socket_server_thread, start_socket_server);

//*
  {
    // test mealy getters and setters

    printf("starting integration_test_check_mealy_gat_and_set...");

    // *** reads only ***
    // writes will explode the state machine

    input_t input = halt;
    input = get_mealy_input();
    assert(input == ok);
    assert(input == get_mealy_input());

    printf("success!\n");
  }
//*/

//*
  {
    // send ok in quick succession

    char cmd[] = "./send_cmd ok\0";
    const int cycles = 5;

    printf("calling: %s\n", cmd);

    for (int i = 0; i < cycles; ++i)
    {
      int ret = system(cmd);
      assert(ret == 0);

      // not sure why, but calling ok without sleep does not cause
      // seg fault but doing this with halt does

      //usleep(100000);
    }
    printf("\n");
  }
//*/

//*
  {
    // send valid command

    char cmd[] = "./send_cmd red 3\0";
    int ret = system(cmd);
    assert(ret == 0);

    printf("\n");
  }
//*/

//*
  {
    // send valid command
    char cmd[] = "./send_cmd green 3\0";
    int ret = system(cmd);
    assert(ret == 0);

    printf("\n");
  }
//*/

//*
  {
    // send valid command

    char cmd[] = "./send_cmd yellow 1\0";
    int ret = system(cmd);
    assert(ret == 0);

    printf("\n");
  }
//*/

//*
  {
    // send valid command

    char cmd[] = "./send_cmd idle 1\0";
    int ret = system(cmd);
    assert(ret == 0);

    printf("\n");
  }
//*/

//*
  {
    // send obligatory foo bar

    char cmd[] = "./send_cmd foo bar\0";
    int ret = system(cmd);
    assert(ret == 0);

    printf("\n");
  }
//*/

//*
  {
    // send bad delay_time

    char cmd[] = "./send_cmd red bg0\0";
    int ret = system(cmd);
    assert(ret == 0);

    printf("\n");
  }
//*/

//*
  {
    // garbage in, garbage out

    char cmd[] = "./send_cmd dfkjbg0kjladfa;lkj\0";
    int ret = system(cmd);
    assert(ret != 0); // output is expected to be bad

    printf("\n");
  }
//*/

//*
  {
    // no args

    char cmd[] = "./send_cmd\0";
    int ret = system(cmd);
    assert(ret != 0);

    printf("\n");
  }
//*/

//*
  {
    // too many args

    char cmd[] = "./send_cmd df adf lkd\0";
    int ret = system(cmd);
    assert(ret != 0);

    printf("\n");
  }
//*/

//*
  {
    // send halt repeatedly

    char cmd[] = "./send_cmd halt\0";
    const int cycles = 2;

    for (int i = 0; i < cycles; ++i)
    {
      printf("calling: %s\n", cmd);

      int ret = system(cmd);
      assert(ret == 0);

      // sending this command too fast
      // results in segfault so add some
      // delay
      usleep(50000);
    }

    printf("\n");
  }
//*/

  stop_thread(socket_server_thread);
  stop_thread(traffic_light_thread);

  return 0;
}

/*
 * for funsies
 */
int generic_test(void)
{
  printf("generic_test o_0... \n");

  const uint32_t a_lot = 0x0FFFFFFF;

  for (uint32_t i = 0; i < a_lot; ++i) {};

  printf("passed generic_test!\n");

  return 0;
}
