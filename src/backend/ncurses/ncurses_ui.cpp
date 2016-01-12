#include "ncurses_ui.hpp"

// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/awordwindows.html

NcursesUI::NcursesUI() {
  //menu = new NcursesMenu;
  //contentWindow = newwin(LINES - 1, COLS, 1, 0);
}

void NcursesUI::start() {
  WINDOW *w;
  w = newwin(LINES - 1, COLS, 1, 0);


  initscr();                      /* Start curses mode            */
  noecho();
  cbreak();
  keypad(stdscr, TRUE);           /* I need that nifty F1         */

  printw("Press F1 to exit");
  refresh();

  WINDOW *local_win;

  local_win = newwin(LINES - 1, COLS, 1, 0);
  box(local_win, 0 , 0);
  wrefresh(local_win);


    ITEM **my_items;
  	int c;
  	MENU *my_menu;
  	int n_choices, i;
  	ITEM *cur_item;

  	n_choices = 3;
  	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

    my_items[0] = new_item("Meas", "Meas2");
    my_items[1] = new_item("Overseers", "Overseers2");
    my_items[2] = new_item("Exit", "Exit2");
  	my_items[n_choices] = (ITEM *)NULL;

  	my_menu = new_menu((ITEM **)my_items);

    menu_opts_off(my_menu, O_SHOWDESC);
    //set_menu_win(my_menu, contentWindow);
    //set_menu_sub(my_menu, derwin(contentWindow, 6, 68, 3, 1));
	  set_menu_format(my_menu, 1, 5);
	  set_menu_mark(my_menu, " * ");

  	post_menu(my_menu);
  	refresh();

  	while((c = getch()) != KEY_F(1))
  	{   switch(c)
  	    {
          case KEY_LEFT:
  		      menu_driver(my_menu, REQ_LEFT_ITEM);
            local_win = redrawWindow(local_win);
  				  break;
  			  case KEY_RIGHT:
  				  menu_driver(my_menu, REQ_RIGHT_ITEM);
            local_win = redrawWindow(local_win);
  				  break;
          default:
            /* DO NOTHING */
            break;
  		}
  	}

  	free_item(my_items[0]);
  	free_item(my_items[1]);
  	free_menu(my_menu);




  //menu->start();

  /*
  int ch;
  while((ch = getch()) != KEY_F(1))
  {
    //sleep 1;
  }
  */

  //menu->stop();

  endwin();
}

/*
WINDOW *NcursesUI::getWindow() {
  WINDOW *local_win;

  local_win = newwin(LINES - 1, COLS, 1, 0);
  box(local_win, 0 , 0);
  wrefresh(local_win);

	return local_win;
}
*/

void NcursesUI::clearContent() {
  //wborder(contentWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  //wrefresh(contentWindow);
  //delwin(contentWindow);
}

void NcursesUI::stop() {
  // TODO
  //menu->stop();
  //endwin();
}

WINDOW *NcursesUI::redrawWindow(WINDOW *w) {
  //return w;

  // del
  wborder(w, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wrefresh(w);
  delwin(w);


  // new
  WINDOW *local_win;

  local_win = newwin(LINES - 1, COLS, 1, 0);

  fillWindow(local_win);

  box(local_win, 0 , 0);
  wrefresh(local_win);

  return local_win;
}

void NcursesUI::fillWindow(WINDOW *w) {
  mvwprintw(w, 10, 10, std::to_string(1).c_str() );
  mvwprintw(w, 11, 11, "a" );
}
