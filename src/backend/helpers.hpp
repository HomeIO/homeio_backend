#ifndef HELPERS
#define	HELPERS

#include <stdio.h>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <mutex>

#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

time_t currentTimeObject;
struct tm * currentTimeInfo;
char currentTimeBuffer[20];
mutex logMutex;
mutex storageMutex; // must place here, because MeasType is in array or sth

char* currentTime();
string detailCurrentTime();
unsigned long long uTime();
unsigned long long mTime();
unsigned int onlyMiliseconds();

// colors

#define RESET           0
#define BRIGHT          1
#define DIM             2
#define UNDERLINE       3
#define BLINK           4
#define REVERSE         7
#define HIDDEN          8
 
#define BLACK           0
#define RED             1
#define GREEN           2
#define YELLOW          3
#define BLUE            4
#define MAGENTA         5
#define CYAN            6
#define WHITE           7

char colorCommand[13];
void txtColor(int attr, int fg, int bg);
char* strColor(int attr, int fg, int bg);
void resetColor();
void redColor();
void logWithColor(string log, unsigned char color);
void logError(string log);
void logInfo(string log);


void processMemUsage(double& vm_usage, double& resident_set);

#include "helpers.cpp"
#endif
