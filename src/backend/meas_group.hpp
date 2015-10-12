#ifndef MEAS_GROUP_H
#define	MEAS_GROUP_H

#include <string>
#include <vector>
#include <sstream>

#include "meas_type_array.hpp"

using namespace std;

class MeasGroup {
public:
  MeasGroup();
  void addGroup(string groupName, string groupMeasTypesString);
  string toJson();

  vector <string> groupNames;
  vector <string> groupMeasTypes;

  MeasTypeArray *measTypeArray;
};

#include "meas_group.cpp"
#endif
