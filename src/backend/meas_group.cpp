MeasGroup::MeasGroup() {
}

void MeasGroup::addGroup(string groupName, string groupMeasTypesString) {
  groupNames.push_back(groupName);
  groupMeasTypes.push_back(groupMeasTypesString);
}

string MeasGroup::toJson() {
  string response = "[";

  for(unsigned int i=0; i < groupNames.size(); i++) {
    response += "{";

    response += "\"name\": \"" + groupNames.at(i) + "\"";
    response += ",";
    response += "\"measTypes\": \"" + groupMeasTypes.at(i) + "\"";

    response += "},";
  }

  // remove last coma
  if (response[response.size() - 1] == ',') {
    response.resize(response.size() - 1);
  }

  response += "]";

  return response;
}
