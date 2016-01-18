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
  interval = 400;

  page = 0;

  meas = new NcursesMeas;
  home = new NcursesHome;
  log = new NcursesLog;
}

void NcursesUI::start() {
  WINDOW *w;
  w = newwin(LINES - 1, COLS, 1, 0);

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

  refresh();

  // content window
  WINDOW *local_win;

  local_win = newwin(LINES - 1, COLS, 1, 0);
  box(local_win, 0 , 0);
  wrefresh(local_win);

  // menu
  ITEM **my_items;
	int c;
	MENU *my_menu;
	int n_choices;
	ITEM *cur_item;

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

	my_menu = new_menu((ITEM **)my_items);

  menu_opts_off(my_menu, O_SHOWDESC);
  set_menu_format(my_menu, 1, 5);
  set_menu_mark(my_menu, " * ");

	post_menu(my_menu);
	refresh();

	while((c = getch()) != KEY_F(1))
	{
    switch(c)
	    {
        case KEY_LEFT:
		      menu_driver(my_menu, REQ_LEFT_ITEM);
          page = 0;
          local_win = redrawWindow(local_win, my_menu);
				  break;

			  case KEY_RIGHT:
				  menu_driver(my_menu, REQ_RIGHT_ITEM);
          page = 0;
          local_win = redrawWindow(local_win, my_menu);
				  break;

        case KEY_DOWN:
          if (page < 100) {
            page++;
          }
          local_win = redrawWindow(local_win, my_menu);
          break;

        case KEY_UP:
          if (page >= 1) {
            page--;
          }
          local_win = redrawWindow(local_win, my_menu);
          break;

        default:
          local_win = redrawWindow(local_win, my_menu);
          break;
		}
	}

  for (int i=0; i <= NC_MENU_LAST; i++) {
    free_item(my_items[i]);
  }
	free_menu(my_menu);
  endwin();
}

void NcursesUI::stop() {
  // TODO
  //endwin();
}

WINDOW *NcursesUI::redrawWindow(WINDOW *w, MENU *my_menu) {
  // del
  //wborder(w, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  //wrefresh(w);
  delwin(w);

  // new
  WINDOW *local_win;

  local_win = newwin(LINES - 1, COLS, 1, 0);

  switch(item_index(current_item(my_menu)))
  {
    case NC_MENU_HOME:
      home->render(local_win);
      break;

    case NC_MENU_LOG:
      log->page = page;
      log->render(local_win);
      break;

    case NC_MENU_MEAS:
      meas->render(local_win);
      break;

    default:
      break;
  }

  box(local_win, 0 , 0);

  refresh();
  wrefresh(local_win);

  return local_win;
}
