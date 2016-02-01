#include "action_type_array.hpp"

ActionTypeArray::ActionTypeArray() {
}

unsigned int ActionTypeArray::add(std::shared_ptr<ActionType> a) {
  a->logArray = logArray;
  actionTypes.push_back(a);
  logArray->log("ActionType", "[" + a->name + "] added (" + std::to_string(actionTypes.size()) + " total action types)");
  return 0;
}

std::shared_ptr<ActionType> ActionTypeArray::byName(std::string s) {
  for(std::vector<std::shared_ptr<ActionType>>::iterator it = actionTypes.begin(); it != actionTypes.end(); ++it) {
    std::shared_ptr<ActionType> actionType = *it;
    if (actionType->name == s) {
      return actionType;
    }
  }
  return NULL;
}
