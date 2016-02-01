#include "ncurses_action.hpp"

#define NC_ACTION_NAME 0

NcursesAction::NcursesAction() {
  page = 0;
  perPage = 1;
  maxPage = 1;
}

unsigned int NcursesAction::setPage(unsigned int p) {
  perPage = (unsigned int) (LINES - 8);
  maxPage = (unsigned int) ( ceil( (float) (actionTypeArray->actionTypes.size()) / (float) (perPage) ) );

  // disable clipping
  if (p >= maxPage) {
    page = maxPage - 1;
  } else {
    page = p;
  }

  return page;
}

void NcursesAction::renderPage(WINDOW *w) {
  mvwprintw(w, LINES - 3, 1, ("Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage) ).c_str());
}

void NcursesAction::render(WINDOW *w) {
  renderPage(w);

  mvwprintw(w, 1, 1 + NC_ACTION_NAME, "Name" );

  unsigned int i;
  long unsigned int j;
  std::shared_ptr<ActionType> element;

  for (i = 0; i < perPage; i++) {
    j = (perPage * page) + i;
    if (j < actionTypeArray->actionTypes.size()) {
      element = actionTypeArray->actionTypes.at(j) ;
      renderAction(w, element, (int) (i + 3));
    }
  }
}

void NcursesAction::renderAction(WINDOW *w, std::shared_ptr<ActionType> a, int i) {
  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, i, 1 + NC_ACTION_NAME, a->name.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);
}
