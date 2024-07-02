CC = gcc
CFLAGS = -Wall -Wextra -std=c99

OBJECTS = main.o init_attacks.o move_generation.o vars.o

all: chess

chess: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

main.o: main.c vars.h
	$(CC) $(CFLAGS) -c main.c -o main.o

init_attacks.o: init_attacks.c vars.h
	$(CC) $(CFLAGS) -c init_attacks.c -o init_attacks.o

move_generation.o: move_generation.c vars.h
	$(CC) $(CFLAGS) -c move_generation.c -o move_generation.o

vars.o: vars.c vars.h
	$(CC) $(CFLAGS) -c vars.c -o vars.o

clean:
	rm -f chess $(OBJECTS)
