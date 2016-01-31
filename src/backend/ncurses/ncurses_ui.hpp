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
#include "ncurses_addon.hpp"
#include "ncurses_stats.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

class NcursesUI {
 public:
  NcursesUI();
  void start();
  void stop();
  void clearContent();
  void redrawWindow();

  unsigned int page;
  int interval; // int because of ncurses

  bool isRunning;
  bool ready;

  NcursesHome *home;
  NcursesMeas *meas;
  NcursesLog *log;
  NcursesOverseer *overseer;
  NcursesAction *action;
  NcursesAddon *addon;
  NcursesStats *stats;

  WINDOW *local_win;

  ITEM **my_items;
  int c;
  MENU *menu;
  int n_choices;
  ITEM *cur_item;
};

#endif
