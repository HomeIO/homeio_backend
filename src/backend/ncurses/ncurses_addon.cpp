#include "ncurses_addon.hpp"

NcursesAddon::NcursesAddon() {
  page = 0;
  perPage = 1;
  maxPage = 0;
}

unsigned int NcursesAddon::setPage(unsigned int p) {
  maxPage = (unsigned int) addonsArray->addons.size();

  // disable clipping
  if (p >= maxPage) {
    page = maxPage - 1;
  } else {
    page = p;
  }

  return page;
}

void NcursesAddon::renderPage(WINDOW *w) {
  // if there is no addon we can't render anything
  if (maxPage > 0) {
    mvwprintw(w, LINES - 3, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());
  }
}

void NcursesAddon::render(WINDOW *w) {
  renderPage(w);

  mvwprintw(w, 1, 1, "Name" );
  auto *element = &addonsArray->addons.at(page) ;

  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, 3, 1, (*element)->name.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);
}
