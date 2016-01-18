#ifndef NCURSES_LOG_HPP
#define	NCURSES_LOG_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "../utils/helper.hpp"
#include "ncurses_colors.hpp"
#include "../log/log_array.hpp"

class NcursesLog {
public:
  NcursesLog();
  void render(WINDOW *w);

  unsigned int page;
  unsigned int perPage;

  LogArray *logArray;
};

#endif
