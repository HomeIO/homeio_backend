#include "log_item.hpp"

LogItem::LogItem() {
  content = "";
  timeString = Helper::detailCurrentTime();
  timeInt = Helper::mTime();
  error = false;
}


LogItem::LogItem(std::string c) {
  content = c;
  timeString = Helper::detailCurrentTime();
  timeInt = Helper::mTime();
  error = false;
}

std::string LogItem::line() {
  return "[" + timeString + "] " + content;
}
