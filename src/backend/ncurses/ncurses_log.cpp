#include "ncurses_log.hpp"

NcursesLog::NcursesLog() {
  page = 0;
  perPage = 1;
  maxPage = 1;
}

unsigned int NcursesLog::setPage(unsigned int p) {
  perPage = (unsigned int) (LINES - 4);
  maxPage = (unsigned int) ( ceil( (float) (logArray->logBuffer.size()) / (float) (perPage) ) );

  // disable clipping
  if (p >= maxPage) {
    page = maxPage - 1;
  } else {
    page = p;
  }

  return page;
}

void NcursesLog::renderPage(WINDOW *w) {
  mvwprintw(w, LINES - 3, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());
}

void NcursesLog::render(WINDOW *w) {
  renderPage(w);

  unsigned int i;
  long unsigned int j;

  for (i = 0; i < perPage; i++) {
    j = logArray->logBuffer.size() - 1 - (perPage * page) - i;
    if (j < logArray->logBuffer.size()) {
      LogItem *li = logArray->logBuffer.at(j) ;

      if (li->error) {
        wattron(w, NC_COLOR_PAIR_ERROR_SET);
      }
      mvwprintw(w, (int) i, 1, li->line().c_str() );
      if (li->error) {
        wattroff(w, NC_COLOR_PAIR_ERROR_SET);
      }

    }
  }

}
