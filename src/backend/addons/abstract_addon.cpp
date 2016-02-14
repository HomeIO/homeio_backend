#include "abstract_addon.hpp"

void AbstractAddon::perform() {
}

void AbstractAddon::setup() {
}

void AbstractAddon::render() {
  mvwprintw(window, 1, 1, "Name" );
  wattron(window, NC_COLOR_PAIR_NAME_SET);
  mvwprintw(window, 3, 1, name.c_str() );
  wattroff(window, NC_COLOR_PAIR_NAME_SET);
}

void AbstractAddon::stop() {
}


std::string AbstractAddon::toJson() {
  return "null";
}

std::string AbstractAddon::toJsonSummary() {
  std::string s = "{\"name\": \"" + name + "\"}";
  return s;
}
