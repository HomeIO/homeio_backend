#include "ncurses_ui.hpp"

// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/awordwindows.html

#define NC_MENU_HOME 0
#define NC_MENU_LOG 1
#define NC_MENU_MEAS 2
#define NC_MENU_ACTION 3
#define NC_MENU_OVERSEER 4
#define NC_MENU_ADDONS 5
#define NC_MENU_STATS 6
#define NC_MENU_LAST 7


NcursesUI::NcursesUI() {
  isRunning = true;
  ready = false;

  interval = 400;

  page = 0;

  meas = new NcursesMeas;
  home = new NcursesHome;
  log = new NcursesLog;
  overseer = new NcursesOverseer;
  action = new NcursesAction;
  addon = new NcursesAddon;
  stats = new NcursesStats;
}

void NcursesUI::start() {
  initscr();
  start_color();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(interval); // milliseconds
  curs_set(0);

  // colors
  init_pair(NC_COLOR_PAIR_VALUE, NC_COLOR_PAIR_VALUE_COLOR, COLOR_BLACK);
  init_pair(NC_COLOR_PAIR_NAME, NC_COLOR_PAIR_NAME_COLOR, COLOR_BLACK);
  init_pair(NC_COLOR_PAIR_ERROR, NC_COLOR_PAIR_ERROR_COLOR, COLOR_BLACK);
  init_pair(NC_COLOR_PAIR_LESSER, NC_COLOR_PAIR_LESSER_COLOR, COLOR_BLACK);
  init_pair(NC_COLOR_PAIR_SYMBOL, NC_COLOR_PAIR_SYMBOL_COLOR, COLOR_BLACK);
  init_pair(NC_COLOR_PAIR_VALUE_LESSER, NC_COLOR_PAIR_VALUE_LESSER_COLOR, COLOR_BLACK);
  init_pair(NC_COLOR_PAIR_NAME_LESSER, NC_COLOR_PAIR_NAME_LESSER_COLOR, COLOR_BLACK);

  refresh();

  local_win = newwin(LINES - 1, COLS, 1, 0);
  box(local_win, 0 , 0);
  wrefresh(local_win);

  // menu

  // menu items - max amount
  my_items = (ITEM **)calloc(NC_MENU_LAST + 1, sizeof(ITEM *));

  // menu items
  my_items[NC_MENU_HOME] = new_item("Home", "Home");
  my_items[NC_MENU_LOG] = new_item("Log", "Log");
  my_items[NC_MENU_MEAS] = new_item("Meas", "Meas");
  my_items[NC_MENU_ACTION] = new_item("Action", "Action");
  my_items[NC_MENU_OVERSEER] = new_item("Overseer", "Overseer");
  my_items[NC_MENU_ADDONS] = new_item("Addons", "Addons");
  my_items[NC_MENU_STATS] = new_item("Stats", "Stats");
  my_items[NC_MENU_OVERSEER] = new_item("Overseer", "Overseer");
  my_items[NC_MENU_LAST] = (ITEM *)NULL;

  menu = new_menu((ITEM **)my_items);

  menu_opts_off(menu, O_SHOWDESC);
  set_menu_format(menu, 1, NC_MENU_LAST);
  set_menu_mark(menu, ">>");

  post_menu(menu);
  refresh();

  ready = true;

  // TODO move loop to other method
  //  != KEY_F(1)
  while((isRunning) && (c = getch())) {
    switch(c) {
    case KEY_LEFT:
      menu_driver(menu, REQ_LEFT_ITEM);
      page = 0;
      redrawWindow();
      break;

    case KEY_RIGHT:
      menu_driver(menu, REQ_RIGHT_ITEM);
      page = 0;
      redrawWindow();
      break;

    case KEY_DOWN:
      if (page < 100) {
        page++;
      }
      redrawWindow();
      break;

    case KEY_UP:
      if (page >= 1) {
        page--;
      }
      redrawWindow();
      break;


    default:
      redrawWindow();
      break;
    }
  }

}

void NcursesUI::stop() {
  // TODO move to stop
  for (int i=0; i <= NC_MENU_LAST; i++) {
    free_item(my_items[i]);
  }
  free_menu(menu);
  endwin();
}

void NcursesUI::redrawWindow() {
  // del
  //wborder(w, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wrefresh(local_win);
  delwin(local_win);
  //delete local_win; // TODO

  // new
  local_win = newwin(LINES - 1, COLS, 1, 0);

  switch(item_index(current_item(menu))) {
  case NC_MENU_HOME:
    home->render(local_win);
    break;

  case NC_MENU_LOG:
    page = log->setPage(page);
    log->render(local_win);
    break;

  case NC_MENU_MEAS:
    page = meas->setPage(page);
    meas->render(local_win);
    break;

  case NC_MENU_OVERSEER:
    page = overseer->setPage(page);
    overseer->render(local_win);
    break;

  case NC_MENU_ACTION:
    page = action->setPage(page);
    action->render(local_win);
    break;

  case NC_MENU_ADDONS:
    page = addon->setPage(page);
    addon->render(local_win);
    break;

  case NC_MENU_STATS:
    page = stats->setPage(page);
    stats->render(local_win);
    break;


  default:
    break;
  }

  box(local_win, 0 , 0);

  refresh();
  wrefresh(local_win);
}
