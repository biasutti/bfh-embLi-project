CC=armv8-rpi3-linux-gnueabihf-gcc
CFLAGS=-c -Wall

all: watercontrol

watercontrol: main.o
	$(CC) main.o -o ./bin/watercontrol

main.o: main.c
	$(CC) $(CFLAGS) main.c

.PHONY: clean
clean:
	@rm *.o
	@rm watercontrol
