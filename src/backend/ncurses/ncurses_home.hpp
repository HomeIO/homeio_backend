#ifndef NCURSES_HOME_HPP
#define	NCURSES_HOME_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../helper.hpp"
#include "ncurses_colors.hpp"
#include "log_item.hpp"

class NcursesHome {
public:
  NcursesHome();
  void render(WINDOW *w);
  void log(std::string c);
  void logError(std::string c);
  void addLogItem(LogItem li);

  unsigned int maxHistory;
  std::vector <LogItem> logBuffer;
};

#endif
