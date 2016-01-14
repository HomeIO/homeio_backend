#ifndef LOG_ITEM_HPP
#define	LOG_ITEM_HPP

#include <string>
#include "../utils/helper.hpp"

class LogItem {
public:
  LogItem();
  LogItem(std::string c);
  LogItem(std::string m, std::string c);
  std::string line();

  std::string content;
  std::string module;
  std::string timeString;
  unsigned long long timeInt;
  bool error;
};


#endif
