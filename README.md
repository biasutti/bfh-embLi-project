# Plantmonitor - README

C-Programm to read a sensor over GPIO (sysfs). Control LEDs of its data and write a log file.

## Installation
The GPIO pins are defined at the top of the main.c file. There are four outputs pins and two input pins.

Default implementation:

````
> define PIN_RED 17
> define PIN_YELLOW 27
> define PIN_GREEN 22
> define POUT_CONTROL 4
> define PIN_SENSOR 23
> define PIN_BUTTON 24
````

## Compile
The make command compiles the programm with the armv8 RPI3b GCC compiler. The executable is copied automaticly to the RPI configured as system-001.embedded.lan.

## Start
The programm must be ran as sudo user.

````
sudo ./plantcontrol
````

The log file is written to the same directory as the programm is started in.
