#include "action_type_array.hpp"

ActionTypeArray::ActionTypeArray() {
  std::vector <ActionType> actionTypes;
}

unsigned int ActionTypeArray::add(ActionType *a) {
  actionTypes.push_back(*a);
  Helper::logInfo("ActionType added: '" + a->name + "' (" + std::to_string(actionTypes.size()) + " total action types)");
  return 0;
}

ActionType *ActionTypeArray::byName(std::string s) {
  for(std::vector<ActionType>::iterator it = actionTypes.begin(); it != actionTypes.end(); ++it) {
    if (it->name == s) {
      return &*it;
    }
  }
  return NULL;
}
