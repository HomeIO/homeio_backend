#include "log_array.hpp"

LogArray::LogArray() {
  maxHistory = 500;
}

void LogArray::log(std::string c) {
  LogItem *li = new LogItem(c);
  addLogItem(li);
}

void LogArray::log(std::string m, std::string c) {
  LogItem *li = new LogItem(m, c);
  addLogItem(li);
}

void LogArray::logError(std::string c) {
  LogItem *li = new LogItem(c);
  li->error = true;
  addLogItem(li);
}

void LogArray::logError(std::string m, std::string c) {
  LogItem *li = new LogItem(m, c);
  li->error = true;
  addLogItem(li);
}

void LogArray::addLogItem(LogItem *li) {
  if ( (unsigned int) logBuffer.size() >= (unsigned int) maxHistory ) {
    delete logBuffer.at(0);
    logBuffer.erase( logBuffer.begin() );
  }
  logBuffer.push_back(li);
}

void LogArray::limit(unsigned int maxLines) {
  if (maxHistory > maxLines) {
    maxHistory = maxLines;
  }
}

void LogArray::consoleOutput() {
  for (unsigned int i = 0; i < (unsigned int) logBuffer.size(); i++) {
    std::cout << logBuffer.at(i)->line() << std::endl;
  }
}
