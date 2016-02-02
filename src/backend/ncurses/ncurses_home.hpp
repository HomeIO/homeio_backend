#ifndef NCURSES_HOME_HPP
#define	NCURSES_HOME_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>


#include "../utils/helper.hpp"
#include "ncurses_colors.hpp"
#include "../log/log_array.hpp"
#include "../utils/boot.hpp"

class NcursesHome {
 public:
  NcursesHome();
  void render(WINDOW *w);
  void renderStatus(WINDOW *w, unsigned char row, bool status, bool changing, bool working, std::string statusText);

  std::shared_ptr<Boot> boot;
  std::shared_ptr<LogArray> logArray;
};

#endif
