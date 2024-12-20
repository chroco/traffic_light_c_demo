## Embedded SW Engineer Coding Challenge

Please write and check into a public facing repo:

1. gcc-compatible C code and makefile for implementing the state machine of 
a traffic light with appropriately timed outputs on the console

2. A test suite for the state machine

## Explanation and Usage

This is a simple and fun traffic light simulation. I used compiler and make 
versions:
- gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
- GNU Make 4.3

### Download, compilation, and use:

clone the repo:

```
git clone https://github.com/chroco/traffic_light_c_demo.git
```
or, if you have your ssh keys set up:
```
git clone git@github.com:chroco/traffic_light_c_demo.git
```
enter into a terminal:

```
cd traffic_light_c_demo
```
```
make
```
this will build 3 executables:
- send_cmd - unix socket client for sending commands to traffic_light_fsm
- test - the test suite for traffic_light_fsm
- traffic_light_fsm - the traffic light simulation with unix socket server

The test suite runs unit tests then opens two threads, a traffic_light_fsm 
and a socket server, and starts integration testing using the send_cmd client.
Additionally, it executs the test binary in valgrind. I used valgrind and 
gdb for debugging.

If you want to use the test suite, run in terminal:

```
make run_test
```

If you want to run traffic_light_fsm, it requires the use of two terminals.
Traffic_light_fsm will execute in one terminal, and then you can send commands
with send_cmd from the second terminal.

Run in first terminal:

```
make run
```

This will start up the traffic light simulation and open an unix socket at
./tmp/traffic_light_socket. You can also use "make valgrind" instead of
"make run" if you want to run traffic_light_fsm in valgrind.

In the second terminal locate the executable send_cmd and run:

```
./send_cmd ok
```

This won't change state, since the fsm starts with 'ok' as it's mealy_input.
However the socket server will respond.

You can change states by sending the fsm input. The rules for these changes
are defined in a state transition table.

  **State transition table**
 | curr   | input  |   next |
 | :---   | :---   |   ---: | 
 | red    | ok     |  green |
 | red    | halt   |   idle |
 | green  | ok     | yellow |
 | green  | halt   |   idle |
 | yellow | ok     |    red |
 | yellow | halt   |   idle |
 | idle   | ok     |    red |
 | idle   | halt   |   quit |
 | idle   | repeat |   idle |
                 
The fsm always enters on state red then loops in the expected sequense:
red -> green -> yellow -> red -> ... -> ad nauseam  
The default times are short because watching traffic lights is boring...
The idle state means that the traffic light is sleeping and not energizing
any of it's lights. When the idle state is entered it changes the input to 
'repeat' automatically.

To quit, run in your command terminal:

```
./send_cmd halt
```

and then one more time

```
./send_cmd halt
```

The first halt moves states from red -> idle then the second halt moves 
the state machine from idle -> quit, which quits the simulation. The 
send_cmd can send the three commands for changing the Mealy input and 
for changing the delay times of the traffic lights.

Note: spamming input commands to quit or change state doesn't make it
happen faster.

Here is the usage of send_cmd:

```
./send_cmd {input_t: ok | halt | repeat }  
``` 
```
./send_cmd {red | green | yellow | idle } {time_delay_in_seconds (1 - 1000)}
```

To change the delay time of the red traffic lights (in seconds) use:

```
./send_cmd red 10
```

this changes the the delay time of the red state to 10 seconds. 

Unfortunately, the simulation uses the sleep function. This means the fsm 
thread doesn't respond while a state is sleeping. So if a state is in a long 
sleep you won't get a state change until it wakes up. However, the server 
will still respond to the client, even if it is ignoring commands while the
fsm is sleeping. Some good upgrades would be to add a separate watch dog 
thread and command queue for valid commands and use linux timers and 
interrupts instead of sleep. 

Testing is not complete IMHO, but is a good start. I added a few unit tests
and integration tests as examples but this is not exhaustive by any means.

My next testing step would be to add a bash script for automated commands
in live testing.

BUGS!
- threads are not not exiting cleanly. without exit(0) the client hangs.
this shows up really well in test using valgrind. handling this bug is
beyond the scope of this exercise (it might be a pita). the workaround 
in traffic_light_fsm is to use exit(0) since it's on the join and nothing
is harmed but the ego. in test i'll accept the seg fault at the end as a
reminder that it needs to be fixed.



