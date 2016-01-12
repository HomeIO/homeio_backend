#ifndef NCURSES_MENU_HPP
#define	NCURSES_MENU_HPP

#include <ncurses.h>

class NcursesMenu {
public:
  NcursesMenu();
  void start();
  void stop();

  WINDOW *menuWindow;
  unsigned char cursor;
};

#endif
