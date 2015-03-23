#ifndef TCP_COMMAND
#define	TCP_COMMAND

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

#include "meas_type_array.hpp"

class TcpCommand {
public:
  TcpCommand();
  string processCommand(string command);
  string processMeasCommand(string command);
  string processMeasListCommand(string command);
  
  MeasTypeArray *measTypeArray;
};

#include "tcp_command.cpp"
#endif
