#ifndef TCP_COMMAND
#define	TCP_COMMAND

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

#include "meas_type_array.hpp"
#include "action_type_array.hpp"
#include "overseer_array.hpp"

using namespace std;

class TcpCommand {
public:
  TcpCommand();
  string processCommand(string command);
  string processMeasCommand(string command);
  string processMeasListCommand(string command);
  string processMeasDetailsCommand(string command);
  string processMeasStorageCommand(string command);
  
  string processActionDetailsCommand(string command);
  string processActionExecuteCommand(string command);
  string processActionHistoryCommand(string command);
  
  string processOverseerDetailsCommand(string command);
  
  MeasTypeArray *measTypeArray;
  ActionTypeArray *actionTypeArray;
  OverseerArray *overseerArray;
};

#include "tcp_command.cpp"
#endif
