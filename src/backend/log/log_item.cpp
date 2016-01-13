#include "log_item.hpp"

LogItem::LogItem() {
  content = "";
  module = "";
  timeString = Helper::detailCurrentTime();
  timeInt = Helper::mTime();
  error = false;
}


LogItem::LogItem(std::string c) {
  content = c;
  module = "";
  timeString = Helper::detailCurrentTime();
  timeInt = Helper::mTime();
  error = false;
}

LogItem::LogItem(std::string m, std::string c) {
  content = c;
  module = m;
  timeString = Helper::detailCurrentTime();
  timeInt = Helper::mTime();
  error = false;
}

std::string LogItem::line() {
  std::string s = "[" + timeString + "] ";
  if (module != "") {
    s += module +": ";
  }
  s += content;
  return s;
}
