CC=gcc
override CFLAGS+=-lm -Wall -Wextra -pedantic --std=gnu11 -O3 `sdl2-config --cflags --libs` -lSDL2_ttf

SOURCES=utils.c                      \
		disassembler.c               \
		lookup3.c                    \
		automated_tests.c            \
		wad_output.c                 \
		other_window.c               \
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
		interrupts.c                 \
		instructions_stack_io_control.c

ROM_PATH=../roms/cpu_instrs/individual/
SOURCE_BIN=main.c
EXECUTABLE=main

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_BIN=$(SOURCE_BIN:.c=.o)

all: $(SOURCES) $(SOURCE_BIN) $(EXECUTABLE)

test: $(EXECUTABLE)
	./$(EXECUTABLE) $(ROM_PATH)/"01-special.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"02-interrupts.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"03-op sp,hl.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"04-op r,imm.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"05-op rp.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"06-ld r,r.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"07-jr,jp,call,ret,rst.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"08-misc instrs.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"09-op r,r.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"10-bit ops.gb"
	./$(EXECUTABLE) $(ROM_PATH)/"11-op a,(hl).gb"


$(EXECUTABLE): $(OBJECTS) $(OBJECTS_BIN)
	$(CC) $(CFLAGS) $(OBJECTS) $(OBJECTS_BIN) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS_BIN) $(OBJECTS) $(EXECUTABLE)
