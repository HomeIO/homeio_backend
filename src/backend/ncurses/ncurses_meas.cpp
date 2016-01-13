#include "ncurses_meas.hpp"

#define NC_MEAS_NAME 0
#define NC_MEAS_VALUE NC_MEAS_NAME + 30
#define NC_MEAS_RAW NC_MEAS_VALUE + 28
#define NC_MEAS_OFFSET NC_MEAS_RAW + 20

NcursesMeas::NcursesMeas() {
}

void NcursesMeas::render(WINDOW *w) {
  mvwprintw(w, 1, 1 + NC_MEAS_NAME, "Name" );
  mvwprintw(w, 1, 1 + NC_MEAS_VALUE, "Value" );
  mvwprintw(w, 1, 1 + NC_MEAS_RAW, "Raw" );
  mvwprintw(w, 1, 1 + NC_MEAS_OFFSET, "Offset" );

  unsigned char i=0;

  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    renderMeas(w, &*it);
  }
}

void NcursesMeas::renderMeas(WINDOW *w, MeasType *m) {
  std::string valueString = "";

  std::ostringstream os;
  os << std::setprecision(5) << m->lastValue();
  valueString += os.str();;
  valueString += " ";
  valueString += m->unit;

  std::string rawString = "";
  rawString += to_string(m->lastRaw());

  string offsetString = "";
  offsetString += to_string(m->buffer->offset);

  wattron(w, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(w, 3 + m->index, 1 + NC_MEAS_NAME, m->name.c_str() );
  wattroff(w, NC_COLOR_PAIR_NAME_SET);

  wattron(w, NC_COLOR_PAIR_VALUE_SET);
  mvwprintw(w, 3 + m->index, 1 + NC_MEAS_VALUE, valueString.c_str());
  wattroff(w, NC_COLOR_PAIR_VALUE_SET);

  wattron(w, NC_COLOR_PAIR_LESSER_SET);
  mvwprintw(w, 3 + m->index, 1 + NC_MEAS_RAW, rawString.c_str());
  mvwprintw(w, 3 + m->index, 1 + NC_MEAS_OFFSET, offsetString.c_str());
  wattroff(w, NC_COLOR_PAIR_LESSER_SET);
}
