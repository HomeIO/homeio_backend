#include "meas_group.hpp"

MeasGroup::MeasGroup() {
}

void MeasGroup::addGroup(std::string groupName, std::string groupMeasTypesString) {
  groupNames.push_back(groupName);
  groupMeasTypes.push_back(groupMeasTypesString);
}

std::string MeasGroup::toJson() {
  std::string response = "[";
  std::string tok;
  char delim = ',';

  for(unsigned int i=0; i < groupNames.size(); i++) {
    response += "{";

    response += "\"name\": \"" + groupNames.at(i) + "\"";
    response += ",";
    //response += "\"measTypes\": \"" + groupMeasTypes.at(i) + "\"";

    response += "\"measTypes\": [";

    // split string
    stringstream ss( groupMeasTypes.at(i) ); // Turn the string into a stream.
    while(getline(ss, tok, delim)) {
      response += "\"" + tok + "\",";
    }

    // remove last coma
    if (response[response.size() - 1] == ',') {
      response.resize(response.size() - 1);
    }

    response += "]";

    response += "},";
  }

  // remove last coma
  if (response[response.size() - 1] == ',') {
    response.resize(response.size() - 1);
  }

  response += "]";

  return response;
}
