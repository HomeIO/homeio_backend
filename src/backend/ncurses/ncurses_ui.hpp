#ifndef NCURSES_UI_HPP
#define	NCURSES_UI_HPP

#include <ncurses.h>
#include <menu.h>
#include <iostream>
#include <string>

#include "ncurses_meas.hpp"
#include "ncurses_home.hpp"
#include "ncurses_log.hpp"
#include "ncurses_overseer.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

class NcursesUI {
public:
  NcursesUI();
  void start();
  void stop();
  void clearContent();
  WINDOW *redrawWindow(WINDOW *w, MENU *my_menu);

  unsigned int page;
  unsigned int interval;

  bool ready;

  NcursesHome *home;
  NcursesMeas *meas;
  NcursesLog *log;
  NcursesOverseer *overseer;
};

#endif
