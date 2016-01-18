#include "ncurses_overseer.hpp"

#define NC_OVERSEER_NAME 0
#define NC_OVERSEER_LINE_SIZE 4

NcursesOverseer::NcursesOverseer() {
  page = 0;
  perPage = 1;
  maxPage = 1;
}

unsigned int NcursesOverseer::setPage(unsigned int p) {
  perPage = (unsigned int) ( (LINES - 8) / NC_OVERSEER_LINE_SIZE );
  maxPage = (unsigned int) ( ceil( (float) (overseerArray->overseers.size()) / (float) (perPage) ) );

  // disable clipping
  if (p >= maxPage) {
    page = maxPage - 1;
  }
  else {
    page = p;
  }

  return page;
}

void NcursesOverseer::renderPage(WINDOW *w) {
  mvwprintw(w, LINES - 3, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());
}

void NcursesOverseer::render(WINDOW *w) {
  renderPage(w);

  mvwprintw(w, 1, 1 + NC_OVERSEER_NAME, "Name" );

  unsigned int i;
  long unsigned int j;
  Overseer *element;

  for (i = 0; i < perPage; i++) {
    j = (perPage * page) + i;
    if (j < overseerArray->overseers.size()) {
      element = &overseerArray->overseers.at(j) ;
      renderOverseer(w, element, (i * NC_OVERSEER_LINE_SIZE) + 3);
    }
  }
}

void NcursesOverseer::renderOverseer(WINDOW *w, Overseer *o, int i) {
  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, i, 1 + NC_OVERSEER_NAME, o->name.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);
}
