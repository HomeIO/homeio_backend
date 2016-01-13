#ifndef NCURSES_MEAS_HPP
#define	NCURSES_MEAS_HPP

#include <ncurses.h>
#include <iostream>
#include <string>

class NcursesMeas {
public:
  NcursesMeas();
  WINDOW *render(WINDOW *w);
};

#endif
