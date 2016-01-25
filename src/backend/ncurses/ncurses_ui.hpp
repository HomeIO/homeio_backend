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
#include "ncurses_action.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

class NcursesUI {
 public:
  NcursesUI();
  void start();
  void stop();
  void clearContent();
  WINDOW *redrawWindow(WINDOW *w);

  unsigned int page;
  unsigned int interval;

  bool isRunning;
  bool ready;
  bool beginShutdown;

  NcursesHome *home;
  NcursesMeas *meas;
  NcursesLog *log;
  NcursesOverseer *overseer;
  NcursesAction *action;

  ITEM **my_items;
  int c;
  MENU *menu;
  int n_choices;
  ITEM *cur_item;
};

#endif
