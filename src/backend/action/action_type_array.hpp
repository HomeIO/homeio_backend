#ifndef ACTION_TYPE_ARRAY
#define	ACTION_TYPE_ARRAY

#include <iostream>
#include <vector>

#include "action_type.hpp"
#include "../io/io_proxy.hpp"
#include "../log/log_array.hpp"

class ActionTypeArray {
 public:
  ActionTypeArray();
  unsigned int add(ActionType *m);
  ActionType *byName(std::string s);

  std::vector <ActionType> actionTypes;
  IoProxy *ioProxy;
  std::shared_ptr<LogArray> logArray;
};

//#include "action_type_array.cpp"
#endif
