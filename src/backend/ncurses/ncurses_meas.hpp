#ifndef NCURSES_MEAS_HPP
#define	NCURSES_MEAS_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>

#include "../meas_type_array.hpp"
#include "../meas_type.hpp"

class NcursesMeas {
public:
  NcursesMeas();
  void render(WINDOW *w);
  void renderMeas(WINDOW *w, MeasType *m);

  MeasTypeArray *measTypeArray;
};

#endif
