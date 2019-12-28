#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN_RED 17
#define PIN_YELLOW 27
#define PIN_GREEN 22
#define POUT_CONTROL 4

// Source: https://elinux.org/RPi_GPIO_Code_Samples#sysfs
static int GPIOExport(int pin) {
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	printf("Successfully exported PIN %d\n", pin);
	return(0);		
}

static int GPIOUnexport(int pin) {
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir) {
	static const char s_directions_str[]  = "in\0out";
#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	printf("Successfully set direction to %d on PIN %d\n", dir, pin);
	return(0);
}

static int GPIORead(int pin) {
	#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

static int GPIOWrite(int pin, int value) {
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

//TODO: Implement read temp sensor values
int readSensor() {
	return rand() % 3 + 1;
}


void INThandler(int sig)
{
	char  c;
	
	signal(sig, SIG_IGN);
	printf("OUCH, did you hit Ctrl-C?\nDo you really want to quit? [y/n] ");
	c = getchar();
	if (c == 'y' || c == 'Y') {

		GPIOUnexport(POUT_CONTROL);
		GPIOUnexport(PIN_RED);
		GPIOUnexport(PIN_YELLOW);
		GPIOUnexport(PIN_GREEN);

		exit(0);
	} else {
		signal(SIGINT, INThandler);
	}
     	getchar(); // Get new line character
}

int main() {

	signal(SIGINT, INThandler);

        if(-1 == GPIOExport(POUT_CONTROL) ||
		-1 == GPIOExport(PIN_RED) || 
		-1 == GPIOExport(PIN_YELLOW) ||
		-1 == GPIOExport(PIN_GREEN)) {
                return 1;
        }

        if(-1 == GPIODirection(POUT_CONTROL, OUT) ||
		-1 == GPIODirection(PIN_RED, OUT) ||
		-1 == GPIODirection(PIN_YELLOW, OUT) ||
		-1 == GPIODirection(PIN_GREEN, OUT)) {
                return 1;
        }

        GPIOWrite(POUT_CONTROL, HIGH);

        while(true) {

		GPIOWrite(PIN_RED, LOW);
		GPIOWrite(PIN_YELLOW, LOW);
		GPIOWrite(PIN_GREEN, LOW);

		int sensorValue = readSensor();
		printf("%d\n", sensorValue);
		switch (sensorValue)
		{
     			case 1:
     			GPIOWrite(PIN_RED, HIGH);
     			;
     			case 2:
     			GPIOWrite(PIN_YELLOW, HIGH);
     			;
     			case 3:
     			GPIOWrite(PIN_GREEN, HIGH);
     			;
		}
                sleep(10);
         }

        return 0;
}

