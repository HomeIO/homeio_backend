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
