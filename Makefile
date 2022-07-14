OBJECTS = list.c main.c 

CFLAGS  = -lm -pg -pthread -w
BUILD_DIR = ./

all: 
	$(CC) -o "$(BUILD_DIR)/main" $(CFLAGS) $(OBJECTS) -lm
