#ifndef NCURSES_MEAS_HPP
#define	NCURSES_MEAS_HPP

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <string>

#include "../meas/meas_type_array.hpp"
#include "../meas/meas_type.hpp"
#include "ncurses_colors.hpp"

class NcursesMeas {
public:
  NcursesMeas();
  void render(WINDOW *w);
  void renderMeas(WINDOW *w, MeasType *m);

  MeasTypeArray *measTypeArray;
};

#endif
