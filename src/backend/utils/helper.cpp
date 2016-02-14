#ifndef HELPERS_CPP
#define	HELPERS_CPP

#include "helper.hpp"

std::mutex Helper::logMutex;
std::mutex Helper::storageMutex;

std::string Helper::currentDateSafe() {
  time_t currentTimeObject;
  struct tm * currentTimeInfo;
  char currentTimeBuffer[TIME_STRING_MAX_BUFFER];

  time (&currentTimeObject);
  currentTimeInfo = localtime (&currentTimeObject);

  strftime (currentTimeBuffer, TIME_STRING_MAX_BUFFER, "%Y_%m_%d", currentTimeInfo);

  return std::string(currentTimeBuffer);
}

std::string Helper::currentTime() {
  time_t currentTimeObject;
  struct tm * currentTimeInfo;
  char currentTimeBuffer[20];

  time (&currentTimeObject);
  currentTimeInfo = localtime (&currentTimeObject);

  strftime (currentTimeBuffer, 80, "%Y-%m-%d %H:%M:%S", currentTimeInfo);

  return std::string(currentTimeBuffer);
}

time_t Helper::timeToObject(meas_time t) {
  time_t tt = (meas_time) ( ((double) t) / 1000.0);
  return tt;
}

std::string Helper::detailCurrentTime() {
  std::string t = currentTime();
  t += ".";

  std::ostringstream os;
  os << std::setfill('0') << std::setw(3) << std::to_string(onlyMiliseconds());
  t += os.str();

  return t;
}

std::string Helper::intervalToString(meas_time timeInterval) {
  // ms
  if (timeInterval < 1000) {
    return std::to_string(timeInterval) + " ms";
  } else {
    timeInterval = std::round((double)timeInterval / 1000.0);
  }

  // s
  if (timeInterval < 600) {
    return std::to_string(timeInterval) + " s";
  } else {
    timeInterval = std::round((double)timeInterval / 60.0);
  }

  // min
  if (timeInterval < 600) {
    return std::to_string(timeInterval) + " min";
  } else {
    timeInterval = std::round((double)timeInterval / 60.0);
  }

  // hours
  if (timeInterval < (24*7)) {
    return std::to_string(timeInterval) + " hours";
  } else {
    timeInterval = std::round((double)timeInterval / 24.0);
  }

  // days
  //if (timeInterval < 1000) {
  return std::to_string(timeInterval) + " days";
  //} else {
  //  timeInterval = timeInterval / 1000;
  //}
}

std::string Helper::timeToTimeString(meas_time mt) {
  time_t t = timeToObject(mt);

  struct tm * currentTimeInfo;
  char currentTimeBuffer[TIME_STRING_MAX_BUFFER];
  currentTimeInfo = localtime (&t);

  strftime (currentTimeBuffer, TIME_STRING_MAX_BUFFER,  "%H:%M:%S", currentTimeInfo);

  return std::string(currentTimeBuffer);
}

std::string Helper::timeToDateTimeString(meas_time mt) {
  time_t t = timeToObject(mt);

  struct tm * currentTimeInfo;
  char currentTimeBuffer[TIME_STRING_MAX_BUFFER];
  currentTimeInfo = localtime (&t);

  strftime (currentTimeBuffer, TIME_STRING_MAX_BUFFER,  "%Y-%m-%d %H:%M:%S", currentTimeInfo);

  return std::string(currentTimeBuffer);
}

std::string Helper::timeToDateString(meas_time mt) {
  time_t t = timeToObject(mt);

  struct tm * currentTimeInfo;
  char currentTimeBuffer[TIME_STRING_MAX_BUFFER];
  currentTimeInfo = localtime (&t);

  strftime (currentTimeBuffer, TIME_STRING_MAX_BUFFER,  "%Y-%m-%d", currentTimeInfo);

  return std::string(currentTimeBuffer);
}

unsigned long long Helper::uTime() {
  unsigned long long ut = (unsigned long long) (std::chrono::system_clock::now().time_since_epoch()  / std::chrono::microseconds(1)) ;
  return ut;
}

unsigned long long Helper::mTime() {
  unsigned long long ut = (unsigned long long) (std::chrono::system_clock::now().time_since_epoch()  / std::chrono::milliseconds(1)) ;
  return ut;
}

unsigned int Helper::onlyMiliseconds() {
  unsigned long long ut = Helper::mTime() % 1000;
  return (unsigned int)ut;
}


std::string Helper::strColor(int attr, int fg, int bg) {
  char colorCommand[13];
  sprintf(colorCommand, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  return std::string(colorCommand);
}

void Helper::txtColor(int attr, int fg, int bg) {
  printf("%s", strColor(attr, fg, bg).c_str() );
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


void Helper::processMemUsage(double& vm_usage, double& resident_set) {
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

void Helper::createDir(std::string path) {
  // create path at start, no wait
  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
}

#endif
