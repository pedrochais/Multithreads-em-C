OBJECTS = list.c main.c 

CFLAGS  = -lm -pg
CFLAGS += -fdiagnostics-color=always -g
#CFLAGS += -march=native -O3 -fgraphite-identity -floop-nest-optimize -fdevirtualize-at-ltrans -fipa-pta -fno-semantic-interposition -flto=2
BUILD_DIR = ./

all: 
	$(CC) -o "$(BUILD_DIR)/main" $(CFLAGS) $(OBJECTS)