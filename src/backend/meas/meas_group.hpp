#ifndef MEAS_GROUP_H
#define	MEAS_GROUP_H

#include <string>
#include <vector>
#include <sstream>

#include "meas_type_array.hpp"

class MeasGroup {
 public:
  MeasGroup();
  void addGroup(std::string groupName, std::string groupMeasTypesString);
  std::string toJson();

  std::vector <std::string> groupNames;
  std::vector <std::string> groupMeasTypes;

  std::shared_ptr<MeasTypeArray> measTypeArray;
};

//#include "meas_group.cpp"
#endif
