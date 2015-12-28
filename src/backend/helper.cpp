#ifndef HELPERS_CPP
#define	HELPERS_CPP

#include "helper.hpp"

char* Helper::currentDateSafe() {
  time_t currentTimeObject;
  struct tm * currentTimeInfo;
  //char currentTimeBuffer[20];

  time (&currentTimeObject);
  currentTimeInfo = localtime (&currentTimeObject);

  strftime (Helper::currentTimeBuffer, 80, "%Y_%m_%d", currentTimeInfo);

  return Helper::currentTimeBuffer;
}

char* Helper::currentTime() {
  time_t currentTimeObject;
  struct tm * currentTimeInfo;
  //char currentTimeBuffer[20];

  time (&currentTimeObject);
  currentTimeInfo = localtime (&currentTimeObject);

  strftime (Helper::currentTimeBuffer, 80, "%Y-%m-%d %H:%M:%S", currentTimeInfo);

  return Helper::currentTimeBuffer;
}

std::string Helper::detailCurrentTime() {
  std::string t = (std::string) currentTime();
  t += ".";

  std::ostringstream os;
  os << std::setfill('0') << std::setw(3) << std::to_string(onlyMiliseconds());
  t += os.str();

  return t;
}

unsigned long long Helper::uTime() {
  unsigned long long ut = std::chrono::system_clock::now().time_since_epoch()  / std::chrono::microseconds(1) ;
  return ut;
}

unsigned long long Helper::mTime() {
  unsigned long long ut = std::chrono::system_clock::now().time_since_epoch()  / std::chrono::milliseconds(1) ;
  return ut;
}

unsigned int Helper::onlyMiliseconds() {
  unsigned long long ut = Helper::mTime() % 1000;
  return (unsigned int)ut;
}


char *Helper::strColor(int attr, int fg, int bg) {
  sprintf(colorCommand, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  return colorCommand;
}

void Helper::txtColor(int attr, int fg, int bg) {
  strColor(attr, fg, bg);
  printf("%s", colorCommand);
}

void Helper::resetColor() {
  txtColor(RESET, WHITE, BLACK);
}

void Helper::redColor() {
  txtColor(RESET, RED, BLACK);
}

void Helper::logWithColor(std::string log, unsigned char color) {
  logMutex.lock();

  txtColor(RESET, color, BLACK);
  std::cout << detailCurrentTime() << " " << log << std::endl;
  resetColor();

  logMutex.unlock();
}

void Helper::logError(std::string log) {
  logWithColor(log, RED);
}

void Helper::logInfo(std::string log) {
  logWithColor(log, WHITE);
}


void Helper::processMemUsage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = (double) vsize / 1024.0;
    resident_set = (double) (rss * page_size_kb);
}

void Helper::longSleep(unsigned long int interval) {
  unsigned long int counts = interval / std::numeric_limits<unsigned long int>::max();
  unsigned int rest = (unsigned int) (interval % std::numeric_limits<unsigned long int>::max() );

  for (unsigned long int i = 0; i < counts; i++) {
    usleep( std::numeric_limits<int>::max() );
  }
  usleep( rest );
}

#endif
