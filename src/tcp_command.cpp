TcpCommand::TcpCommand() {
}

// sample command: "batt_u;0;100;"
// sample response: "{data:[1,2,3....,100],measType:batt_u}

string TcpCommand::processCommand(string command) {
  string measName;
  measName = command.substr(0, command.find(";"));
  
  cout << measName;
  
  return "test";
}