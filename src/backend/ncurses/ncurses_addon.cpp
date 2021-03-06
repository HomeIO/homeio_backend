#include "ncurses_addon.hpp"

NcursesAddon::NcursesAddon() {
  page = 0;
  perPage = 1;
  maxPage = 0;
}

bool NcursesAddon::ready() {
  return addonsArray->ready;
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
  } else {
    mvwprintw(w, 1, 1, "0 addons" );
  }
}

void NcursesAddon::render(WINDOW *w) {
  if (ready() == false) {
    mvwprintw(w, 1, 1, "Initializing..." );
    return;
  } else {
    renderPage(w);
    auto *element = &addonsArray->addons.at(page) ;
    (*element)->window = w;
    (*element)->render();
  }
}
