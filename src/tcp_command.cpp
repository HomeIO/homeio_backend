TcpCommand::TcpCommand() {
}

// sample command: "batt_u;0;100;"
// sample response: "{data:[1,2,3....,100],measType:batt_u}

string TcpCommand::processCommand(string command) {
  string measName, response;
  measName = command.substr(0, command.find(";"));
  
  MeasType *foundMeasType = measTypeArray->byName(measName);
  
  if (foundMeasType) {
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\"}";
  }
  else {
    response = "{\"status\":1,\"reason\":\"meas_not_found\"}";
  }
  
  cout << measName;
  
  return response;
}