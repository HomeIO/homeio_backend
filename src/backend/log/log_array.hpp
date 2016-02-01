#ifndef LOG_ARRAY_HPP
#define	LOG_ARRAY_HPP

#include <string>
#include <vector>
#include <memory>

#include "../utils/helper.hpp"
#include "log_item.hpp"

class LogArray {
 public:
  LogArray();
  void log(std::string c);
  void log(std::string m, std::string c);
  void logError(std::string c);
  void logError(std::string m, std::string c);
  void addLogItem(std::shared_ptr<LogItem>);
  void limit(unsigned int maxLines);
  void consoleOutput();

  unsigned int maxHistory;
  std::vector <std::shared_ptr<LogItem>> logBuffer;
};


#endif
