TcpCommand::TcpCommand() {
}

// sample command: "batt_u;0;100;"
// sample response: "{data:[1,2,3....,100],measType:batt_u}

string TcpCommand::processCommand(string command) {
  cout << "TCP command: " << command << endl;
  
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
  
  //cout << "from " << from << endl;
  //cout << "to " << to << endl;
  //cout << "from " << command << endl;
  
  MeasType *foundMeasType = measTypeArray->byName(measName);
  
  if (foundMeasType) {
    string bufferString = foundMeasType->buffer->jsonArray(from, to);
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\",\"data\": " + bufferString + "}";
  }
  else {
    response = "{\"status\":1,\"reason\":\"meas_not_found\"}";
  }
  
  cout << "TCP response: " << response << endl;
  
  return response;
}
