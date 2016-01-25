#ifndef NCURSES_ADDON_HPP
#define	NCURSES_ADDON_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "../addons/addons_array.hpp"
#include "../addons/all_addons.hpp"
#include "ncurses_colors.hpp"

class NcursesAddon {
 public:
  NcursesAddon();
  void render(WINDOW *w);
  void renderPage(WINDOW *w);
  unsigned int setPage(unsigned int p);

  unsigned int page;
  unsigned int perPage;
  unsigned int maxPage;

  AddonsArray *addonsArray;
};

#endif
