CC=gcc
override CFLAGS+=-lm -Wall -Wextra -pedantic --std=gnu11 -O3 `sdl2-config --cflags --libs` -lSDL2_ttf

SOURCES=utils.c                      \
		disassembler.c               \
		lookup3.c                    \
		automated_tests.c            \
		wad_output.c                 \
		decoder.c                    \
		graphics.c                   \
		time_keeper.c                \
		microcode.c                  \
		memory.c                     \
		cartridge.c                  \
		display.c                    \
		instructions_branch.c        \
		instructions_data_transfer.c \
		instructions_logical.c       \
		instructions_arithmetic.c    \
		instructions_0xcb.c          \
		halfcarry.c                  \
		overlay.c                    \
		audio.c                      \
		ch1.c                        \
		ch2.c                        \
		ch3.c                        \
		ch4.c                        \
		interrupts.c                 \
		instructions_stack_io_control.c

ROM_PATH=../roms/cpu_instrs/individual/
SOURCE_BIN=main.c
EXECUTABLE=main

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_BIN=$(SOURCE_BIN:.c=.o)

all: $(SOURCES) $(SOURCE_BIN) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(OBJECTS_BIN)
	$(CC) $(CFLAGS) $(OBJECTS) $(OBJECTS_BIN) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS_BIN) $(OBJECTS) $(EXECUTABLE)
