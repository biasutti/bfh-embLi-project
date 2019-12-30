#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include "GPIOControl.h"

#define PIN_RED 17
#define PIN_YELLOW 27
#define PIN_GREEN 22
#define POUT_CONTROL 4

//TODO: Implement read temp sensor values
int readSensor() {
	return rand() % 3 + 1;
}


void INThandler(int sig)
{
	char  c;
	
	signal(sig, SIG_IGN);
	printf("\nDid you hit Ctrl-C?\nDo you really want to quit? [y/n] ");
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
				break;
			case 2:
     				GPIOWrite(PIN_YELLOW, HIGH);
     				break;
     			case 3:
     				GPIOWrite(PIN_GREEN, HIGH);
				break;
		}
		sleep(10);
        }

        return 0;
}

