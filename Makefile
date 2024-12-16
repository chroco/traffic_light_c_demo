
CC=gcc

APP_BIN=traffic_light_demo
TEST_BIN=test

INC=./inc
SRC=./src
OBJ=./src/obj

APP_CFLAGS=  
TEST_CFLAGS=
FLAGS=-I $(INC) -g -Wall -Wextra \
			-Wno-unused-parameter 

APP_SRCS=$(SRC)/main.c \
				 $(SRC)/traffic_light.c 

TEST_SRCS=$(SRC)/test_main.c \
					$(SRC)/test.c

APP_INC=$(INC)/traffic_light.h \

TEST_INC=$(INC)/test.h \
				 $(INC)/traffic_light.h

APP_OBJS=$(APP_SRCS:.c=.o)
TEST_OBJS=$(TEST_SRCS:.c=.o)

default: $(APP_BIN) $(TEST_BIN)
	@echo build successful!
 
$(APP_BIN): $(APP_OBJS) $(APP_INC) $(APP_SRCS)
	$(CC) $(APP_CFLAGS) -o $(APP_BIN) $(APP_OBJS)
	@echo $(APP_BIN) compiled!

$(TEST_BIN): $(TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN) $(TEST_OBJS)
	@echo $(TEST_BIN) compiled!

.c.o: 
	$(CC) $(FLAGS) -c $^ -o $@

run_app: run_test
	./$(APP_BIN)

run_test:
	valgrind --leak-check=full \
	./$(TEST_BIN)

clean: 
	rm -f $(SRC)/*.o \
		*.o \
		$(APP_BIN) \
		$(TEST_BIN)

valgrind: $(BIN)
	valgrind --leak-check=full \
		./$(APP_BIN)
		./$(TEST_BIN)

