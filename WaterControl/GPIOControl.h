#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

int GPIOExport(int);
int GPIOUnexport(int);
int GPIODirection(int, int);
int GPIORead(int);
int GPIOWrite(int, int);
