#ifndef HELPERS_HPP
#define	HELPERS_HPP

#define UNUSED(x) (void)(x)

#include <stdio.h>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <limits>

#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


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

class Helper {
 public:
  //static time_t currentTimeObject;
  //static struct tm * currentTimeInfo;
  //static char currentTimeBuffer[20];

  static std::mutex logMutex;
  static std::mutex storageMutex;

  static std::string currentTime();
  static std::string detailCurrentTime();
  static std::string currentDateSafe();
  static unsigned long long uTime();
  static unsigned long long mTime();
  static unsigned int onlyMiliseconds();

  static void txtColor(int attr, int fg, int bg);
  static std::string strColor(int attr, int fg, int bg);
  static void resetColor();
  static void redColor();
  static void logWithColor(std::string log, unsigned char color);
  static void logError(std::string log);
  static void logInfo(std::string log);

  static void processMemUsage(double& vm_usage, double& resident_set);

  static void longSleep(unsigned long int interval);
};

//#include "helpers.cpp"
#endif
