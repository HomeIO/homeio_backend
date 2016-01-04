#include "ncurses_ui.hpp"

NcursesUI::NcursesUI() {

}

void NcursesUI::start() {
  initscr();
	clear();
  
	// Print a string on screen
 	printw("Seems that you can use ncurses ...\nPress any key to exit!");

 	// Wait until the user press a key
 	getch();
}

void NcursesUI::stop() {
  endwin();
}
