CC=gcc
CFLAGS=-lm -Wall -Wextra -pedantic --std=c11 -Og -g `sdl-config --libs` -lSDL
SOURCES=main.c utils.c disassembler.c emulator.c graphics.c
EXECUTABLE=main

OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
