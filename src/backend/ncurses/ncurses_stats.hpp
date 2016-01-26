#ifndef NCURSES_STATS_HPP
#define	NCURSES_STATS_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "../utils/helper.hpp"
#include "ncurses_colors.hpp"

class NcursesStats {
 public:
  NcursesStats();
  void render(WINDOW *w);
  void renderPage(WINDOW *w);
  unsigned int setPage(unsigned int p);

  unsigned int page;
  unsigned int perPage;
  unsigned int maxPage;
};

#endif
