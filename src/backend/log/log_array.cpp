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

void LogArray::addLogItem(std::shared_ptr<LogItem> li) {
  if ( (unsigned int) logBuffer.size() >= (unsigned int) maxHistory ) {
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
