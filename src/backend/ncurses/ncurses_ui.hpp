#ifndef NCURSES_UI_HPP
#define	NCURSES_UI_HPP

#include <ncurses.h>
#include <menu.h>
#include <iostream>
#include <string>

#include "../helper.hpp"
#include "ncurses_menu.hpp"
#include "ncurses_meas.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

class NcursesUI {
public:
  NcursesUI();
  void start();
  void stop();
  void clearContent();
  WINDOW *redrawWindow(WINDOW *w, MENU *my_menu);
  void windowHome(WINDOW *w);

  NcursesMeas *meas;
};

#endif
