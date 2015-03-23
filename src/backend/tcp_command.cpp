TcpCommand::TcpCommand() {
}

// sample command: "meas;batt_u;0;100;"
string TcpCommand::processCommand(string command) {
  cout << currentTime() << " TCP command: " << command << endl;
  
  string commandName, response;
  response = "{}";
  
  commandName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  
  if (commandName == "meas") {
    response = processMeasCommand(command);
  }

  if (commandName == "measList") {
    response = processMeasListCommand(command);
  }
  
  cout << currentTime() << " TCP response: " << response << endl;
  
  return response;
}

string TcpCommand::processMeasCommand(string command) {
  string measName, fromString, toString, response;
  unsigned long int from, to;
  
  measName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  
  fromString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  
  toString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  
  stringstream(fromString) >> from;
  stringstream(toString) >> to;
  
  MeasType *foundMeasType = measTypeArray->byName(measName);
  
  if (foundMeasType) {
    string bufferString = foundMeasType->buffer->jsonArray(from, to);
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\"";
    response += ",\"last_time\":" + to_string( foundMeasType->buffer->lastTime );
    response += ",\"first_time\":" + to_string( foundMeasType->buffer->firstTime );
    response += ",\"interval\":" + to_string( foundMeasType->buffer->calcInterval() );
    response += ",\"count\":" + to_string( foundMeasType->buffer->count );
    response += ",\"data\": " + bufferString + "}";
  }
  else {
    response = "{\"status\":1,\"meas_type\":\"" + measName + "\",\"reason\":\"meas_not_found\"}";
  }
  
  cout << currentTime() << " TCP response: " << response << endl;
  
  return response;
}

string TcpCommand::processMeasListCommand(string command) {
  string response;
  
  response = "{\"status\":0,\"array\":[";
  
  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    response += "\"" + it->name + "\",";
  }
  
  // remove last coma
  if (response[response.size() - 1] == ',') {
    response.resize(response.size() - 1);
  }
  
  response += "]}";
    
  return response;
}