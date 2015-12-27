#ifndef ACTION_TYPE_ARRAY
#define	ACTION_TYPE_ARRAY

#include <iostream>
#include <vector>
#include "action_type.hpp"
#include "io_proxy.hpp"

using namespace std;

class ActionTypeArray {
public:
  ActionTypeArray();
  unsigned int add(ActionType *m);
  ActionType *byName(string s);

  std::vector <ActionType> actionTypes;
  IoProxy *ioProxy;
};

//#include "action_type_array.cpp"
#endif
