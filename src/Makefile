CC=gcc
CFLAGS=-lm -Wall -Wextra -pedantic --std=c11 -Og -g
SOURCES=main.c utils.c
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
