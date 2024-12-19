
CC=gcc

APP_BIN=traffic_light_demo
TEST_BIN=test
CLIENT_BIN=send_cmd

INC=./inc
SRC=./src
OBJ=./src/obj

FLAGS=-I $(INC) -g -Wall -Wextra -Wno-uninitialized -pthread
#			-Wno-unused-parameter 

APP_SRCS=$(SRC)/main.c \
				 $(SRC)/traffic_light.c 

TEST_SRCS=$(SRC)/test_main.c \
					$(SRC)/test.c \
				 	$(SRC)/traffic_light.c 

CLIENT_SRCS=$(SRC)/client_main.c \

APP_OBJS=$(APP_SRCS:.c=.o)
TEST_OBJS=$(TEST_SRCS:.c=.o)
CLIENT_OBJS=$(CLIENT_SRCS:.c=.o)

default: clean $(APP_BIN) $(TEST_BIN) $(CLIENT_BIN)
	@echo build successful!
 
$(APP_BIN): $(APP_OBJS) $(APP_INC) $(APP_SRCS)
	$(CC) -o $(APP_BIN) $(APP_OBJS)
	@echo $(APP_BIN) compiled!

$(TEST_BIN): $(TEST_OBJS)
	$(CC) -o $(TEST_BIN) $(TEST_OBJS)
	@echo $(TEST_BIN) compiled!

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(CC) -o $(CLIENT_BIN) $(CLIENT_OBJS)
	@echo $(CLIENT_BIN) compiled!

.c.o: 
	$(CC) $(FLAGS) -c $^ -o $@

client:
	./$(CLIENT_BIN)

run:
	./$(APP_BIN)

run_all: run_test
	./$(APP_BIN)

run_test:
	valgrind --leak-check=full \
	./$(TEST_BIN)

clean: 
	rm -f $(SRC)/*.o \
		*.o \
		$(APP_BIN) \
		$(TEST_BIN) \
		$(CLIENT_BIN)

valgrind: 
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		-s \
		./$(APP_BIN) \

