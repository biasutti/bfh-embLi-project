CC=armv8-rpi3-linux-gnueabihf-gcc
CFLAGS=-c -Wall

all: plantmonitor

plantmonitor: main.o GPIOControl.o
	$(CC) main.o GPIOControl.o -o ./bin/plantmonitor
	scp ./bin/plantmonitor system-01.embedded.lan:project/

main.o: main.c
	$(CC) $(CFLAGS) main.c

GPIOControl.o: GPIOControl.c
	$(CC) $(CFLAGS) GPIOControl.c

.PHONY: clean
clean:
	@rm *.o
	@rm plantmonitor
