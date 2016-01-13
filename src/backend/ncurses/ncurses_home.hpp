#ifndef NCURSES_HOME_HPP
#define	NCURSES_HOME_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>


#include "../helper.hpp"
#include "ncurses_colors.hpp"
#include "../log/log_array.hpp"

class NcursesHome {
public:
  NcursesHome();
  void render(WINDOW *w);

  LogArray *logArray;
};

#endif
