#ifndef HELPERS
#define	HELPERS

#include <time.h>
#include <stdio.h>

time_t currentTimeObject;
struct tm * currentTimeInfo;
char currentTimeBuffer[20];

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

#include "helpers.cpp"
#endif
