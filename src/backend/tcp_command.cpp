TcpCommand::TcpCommand() {
}

// sample command: "meas;batt_u;0;100;"
string TcpCommand::processCommand(string command) {
  logInfo("TCP command: " + command);
  
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

  if (commandName == "measDetails") {
    response = processMeasDetailsCommand(command);
  }

  if (commandName == "actionDetails") {
    response = processActionDetailsCommand(command);
  }

  if (commandName == "actionExecute") {
    response = processActionExecuteCommand(command);
  }

  logInfo("TCP response: " + response);
  
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

string TcpCommand::processMeasDetailsCommand(string command) {
  string response, detailsResponse;
  
  response = "{\"status\":0,\"array\":[";
  
  for(std::vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    detailsResponse += it->toJson() + ",";
  }
  
  // remove last coma
  if (detailsResponse[detailsResponse.size() - 1] == ',') {
    detailsResponse.resize(detailsResponse.size() - 1);
  }
  
  response += detailsResponse + "]}";
    
  return response;
}

string TcpCommand::processActionDetailsCommand(string command) {
  string response, detailsResponse;
  
  response = "{\"status\":0,\"array\":[";
  
  for(std::vector<ActionType>::iterator it = actionTypeArray->actionTypes.begin(); it != actionTypeArray->actionTypes.end(); ++it) {
    detailsResponse += it->toJson() + ",";
  }  
  
  // remove last coma
  if (detailsResponse[detailsResponse.size() - 1] == ',') {
    detailsResponse.resize(detailsResponse.size() - 1);
  }
  
  response += detailsResponse + "]}";
    
  return response;
}

string TcpCommand::processActionExecuteCommand(string command) {
  string actionName;
  actionName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  
  string response;
  
  ActionType *foundActionType = actionTypeArray->byName(actionName);
  
  if (foundActionType) {
    string bufferString;
    foundActionType->execute();
    response = "{\"status\":0,\"action\":" + foundActionType->toJson() + "}";
  }
  else {
    response = "{\"status\":1,\"action\":\"" + actionName + "\",\"reason\":\"action_not_found\"}";
  }
  
  return response;  
}


