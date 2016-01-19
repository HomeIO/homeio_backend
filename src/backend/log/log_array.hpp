#ifndef LOG_ARRAY_HPP
#define	LOG_ARRAY_HPP

#include <string>
#include <vector>
#include "../utils/helper.hpp"
#include "log_item.hpp"

class LogArray {
 public:
  LogArray();
  void log(std::string c);
  void log(std::string m, std::string c);
  void logError(std::string c);
  void logError(std::string m, std::string c);
  void addLogItem(LogItem li);
  void limit(unsigned int maxLines);

  unsigned int maxHistory;
  std::vector <LogItem> logBuffer;
};


#endif
