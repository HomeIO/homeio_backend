#ifndef NCURSES_OVERSEER_HPP
#define	NCURSES_OVERSEER_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <memory>

#include "../overseer/overseer_array.hpp"
#include "../overseer/overseer.hpp"
#include "ncurses_colors.hpp"

class NcursesOverseer {
 public:
  NcursesOverseer();
  void render(WINDOW *w);
  void renderPage(WINDOW *w);
  void renderOverseer(WINDOW *w, std::shared_ptr<Overseer> o, int i);
  unsigned int setPage(unsigned int p);

  bool ready();

  unsigned int page;
  unsigned int perPage;
  unsigned int maxPage;

  std::shared_ptr<OverseerArray> overseerArray;
};

#endif
