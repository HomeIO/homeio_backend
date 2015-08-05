char* currentDateSafe() {
  time (&currentTimeObject);
  currentTimeInfo = localtime (&currentTimeObject);
  
  strftime (currentTimeBuffer, 80, "%Y_%m_%d", currentTimeInfo);

  return currentTimeBuffer;
}

char* currentTime() {
  time (&currentTimeObject);
  currentTimeInfo = localtime (&currentTimeObject);
  
  strftime (currentTimeBuffer, 80, "%Y-%m-%d %H:%M:%S", currentTimeInfo);

  return currentTimeBuffer;
}

string detailCurrentTime() {
  string t = (string) currentTime();
  t += ".";
  
  ostringstream os;
  os << setfill('0') << setw(3) << to_string(onlyMiliseconds());
  t += os.str();
  
  return t;
}

unsigned long long uTime() {
  unsigned long long ut = std::chrono::system_clock::now().time_since_epoch()  / std::chrono::microseconds(1) ;
  return ut;  
}

unsigned long long mTime() {
  unsigned long long ut = std::chrono::system_clock::now().time_since_epoch()  / std::chrono::milliseconds(1) ;
  return ut;  
}

unsigned int onlyMiliseconds() {
  unsigned long long ut = mTime() % 1000;
  return (unsigned int)ut;  
}


char *strColor(int attr, int fg, int bg) {
  sprintf(colorCommand, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  return colorCommand;
}

void txtColor(int attr, int fg, int bg) {
  strColor(attr, fg, bg);
  printf("%s", colorCommand);
}

void resetColor() {
  txtColor(RESET, WHITE, BLACK);
}

void redColor() {
  txtColor(RESET, RED, BLACK);
}

void logWithColor(string log, unsigned char color) {
  logMutex.lock();
  
  txtColor(RESET, color, BLACK);
  cout << detailCurrentTime() << " " << log << endl;
  resetColor();
  
  logMutex.unlock();
}

void logError(string log) {
  logWithColor(log, RED);
}

void logInfo(string log) {
  logWithColor(log, WHITE);
}


void processMemUsage(double& vm_usage, double& resident_set)
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

void longSleep(unsigned long int interval) {
  unsigned long int counts = interval / numeric_limits<unsigned long int>::max();
  unsigned int rest = (unsigned int) (interval % numeric_limits<unsigned long int>::max() );
  
  for (unsigned long int i = 0; i < counts; i++) {
    usleep( numeric_limits<int>::max() );
  }
  usleep( rest );
}