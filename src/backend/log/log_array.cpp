#include "log_array.hpp"

LogArray::LogArray() {
  maxHistory = 500;
}

void LogArray::log(std::string c) {
  std::shared_ptr<LogItem> li = std::make_shared<LogItem>(c);
  addLogItem(li);
}

void LogArray::log(std::string m, std::string c) {
  std::shared_ptr<LogItem> li = std::make_shared<LogItem>(m, c);
  addLogItem(li);
}

void LogArray::logError(std::string c) {
  std::shared_ptr<LogItem> li = std::make_shared<LogItem>(c);
  li->error = true;
  addLogItem(li);
}

void LogArray::logError(std::string m, std::string c) {
  std::shared_ptr<LogItem> li = std::make_shared<LogItem>(m, c);
  li->error = true;
  addLogItem(li);
}

void LogArray::logFatal(std::string c) {
  logError(c);
  consoleOutputLast();
}

void LogArray::logFatal(std::string m, std::string c) {
  logError(m, c);
  consoleOutputLast();
}

void logError(std::string c);
void logError(std::string m, std::string c);

void LogArray::addLogItem(std::shared_ptr<LogItem> li) {
  addMutex.lock();

  if ( (unsigned int) logBuffer.size() >= (unsigned int) maxHistory ) {
    logBuffer.erase( logBuffer.begin() );
  }
  logBuffer.push_back(li);

  addMutex.unlock();
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

void LogArray::consoleOutputLast() {
  const unsigned int lastCount = 10;
  unsigned int i = 0;
  if ( (unsigned int) logBuffer.size() > lastCount ) {
    i = logBuffer.size() - lastCount;
  }
  for (; i < (unsigned int) logBuffer.size(); i++) {
    std::cout << logBuffer.at(i)->line() << std::endl;
  }
}
