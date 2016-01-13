#ifndef LOG_ITEM_HPP
#define	LOG_ITEM_HPP

#include <string>
#include "../helper.hpp"

class LogItem {
public:
  LogItem();
  LogItem(std::string c);
  std::string line();

  std::string content;
  std::string timeString;
  unsigned long long timeInt;
  bool error;
};


#endif
