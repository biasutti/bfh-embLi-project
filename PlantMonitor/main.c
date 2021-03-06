#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "GPIOControl.h"

#define PIN_RED 17
#define PIN_YELLOW 27
#define PIN_GREEN 22
#define POUT_CONTROL 4

#define PIN_SENSOR 23
#define PIN_BUTTON 24

// TODO: Implement later
int readConfig() {
	return 0;
}

//TODO: Implement read temp sensor values
// Currently digital dignal, improve for analog signal
int readSensor() {
	return GPIORead(PIN_SENSOR);
	//return rand() % 3 + 1;
}

void INThandler(int sig) {
	char  c;
	
	signal(sig, SIG_IGN);
	printf("\nDid you hit Ctrl-C?\nDo you really want to quit? [y/n] ");
	c = getchar();
	if (c == 'y' || c == 'Y') {

		GPIOUnexport(POUT_CONTROL);
		GPIOUnexport(PIN_RED);
		GPIOUnexport(PIN_YELLOW);
		GPIOUnexport(PIN_GREEN);

		GPIOUnexport(PIN_SENSOR);
		GPIOUnexport(PIN_BUTTON);

		exit(0);
	} else {
		signal(SIGINT, INThandler);
	}
     	getchar();
}

void printLog(int severityVal, char message[]) {
	
	FILE *f;
	f = fopen("plantmonitor.log", "a+");
	if(f == NULL) {
		printf("ERROR: Cannot write logfile");
	}
	
	char date[100];
	time_t now = time(NULL);
	struct tm * p = localtime(&now);
	strftime(date, 100, "%c", p);

	char* severity;

	switch (severityVal)
        {
	        case 0:
			severity = "INFO";
        		break;
                case 1:
			severity = "WARNING";
                        break;
                case 2:
			severity = "ERROR";
                        break;
       	}

	fprintf(f, "%s - %s - %s\n", date, severity, message);
}

void printSensorValue(int sensorValue) {
	printf("%d\n", sensorValue);
}

// TODO: Sendmail when sensor get 1
void sendMail() {

}

int initializeGPIO() {
	if(-1 == GPIOExport(POUT_CONTROL) ||
                -1 == GPIOExport(PIN_RED) ||
                -1 == GPIOExport(PIN_YELLOW) ||
                -1 == GPIOExport(PIN_GREEN) ||
                -1 == GPIOExport(PIN_SENSOR) ||
		-1 == GPIOExport(PIN_BUTTON)) {
                return -1;
        }

	if(-1 == GPIODirection(POUT_CONTROL, OUT) ||
                -1 == GPIODirection(PIN_RED, OUT) ||
                -1 == GPIODirection(PIN_YELLOW, OUT) ||
                -1 == GPIODirection(PIN_GREEN, OUT) ||
                -1 == GPIODirection(PIN_SENSOR, IN) ||
		-1 == GPIODirection(PIN_BUTTON, IN)) {
                return -1;
        }

	return 0;
}

int main() {

	signal(SIGINT, INThandler);

        if (-1 == initializeGPIO()) {
		return 1;
	}

        GPIOWrite(POUT_CONTROL, HIGH);

	int sensorValue = -1;

	int cyclesPerSecond = 10;
	int secondsPerInterval = 60;
	int interval = secondsPerInterval * cyclesPerSecond;

	int loopCounter = interval;

	bool lockButton = false;

        while(true) {
		if(interval == loopCounter  || (GPIORead(PIN_BUTTON) == 0 && !lockButton)) {

			lockButton = true;

			sensorValue = readSensor();
			GPIOWrite(PIN_RED, LOW);
                	GPIOWrite(PIN_YELLOW, LOW);
        	        GPIOWrite(PIN_GREEN, LOW);
	
			char buffer[15];
			snprintf(buffer, 15, "Sensorvalue: %d", sensorValue);

			printSensorValue(sensorValue);
			printLog(0, buffer);
		
			switch (sensorValue)
			{
     				case 0:
	     				GPIOWrite(PIN_RED, HIGH);
					break;
				case 2:
     					GPIOWrite(PIN_YELLOW, HIGH);
     					break;
     				case 1:
     					GPIOWrite(PIN_GREEN, HIGH);
					break;
			}

		}

		if(lockButton && GPIORead(PIN_BUTTON) == 1) {
			lockButton = false;
		}

		loopCounter--;

		if(loopCounter == 0) {
			loopCounter = interval;
		}

		usleep((1000 * 1000) / cyclesPerSecond);
        }

        return 0;
}

