CC=gcc
override CFLAGS+=-lm -Wall -Wextra -pedantic --std=gnu11 -O3 `sdl-config --libs` -lSDL -fopenmp
SOURCES=utils.c                      \
		disassembler.c               \
		emulator.c                   \
		graphics.c                   \
		time_keeper.c                \
		halfcary.c
		#instructions_data_transfer.c \
		#instructions_arithmetic.c    \
		#instructions_logical.c       \
		#instructions_branch.c        \
		#instructions_stack_io_control.c

SOURCE_BIN=main.c
EXECUTABLE=main

SOURCE_TEST=main_test.c
EXECUTABLE_TEST=main_test


OBJECTS=$(SOURCES:.c=.o)
OBJECTS_BIN=$(SOURCE_BIN:.c=.o)
OBJECTS_TEST=$(SOURCE_TEST:.c=.o)

all: $(SOURCES) $(SOURCE_BIN) $(EXECUTABLE)
test: $(SOURCES) $(SOURCE_TEST) $(EXECUTABLE_TEST)

$(EXECUTABLE_TEST): $(OBJECTS) $(OBJECTS_TEST)
	$(CC) $(CFLAGS) $(OBJECTS) $(OBJECTS_TEST) -o $@
	./$(EXECUTABLE_TEST)

$(EXECUTABLE): $(OBJECTS) $(OBJECTS_BIN)
	$(CC) $(CFLAGS) $(OBJECTS) $(OBJECTS_BIN) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS_BIN) $(OBJECTS_TEST) $(OBJECTS) $(EXECUTABLE_TEST) $(EXECUTABLE)
