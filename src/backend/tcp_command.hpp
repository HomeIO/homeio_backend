#ifndef TCP_COMMAND
#define	TCP_COMMAND

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

#include "meas_type_array.hpp"
#include "action_type_array.hpp"
#include "overseer_array.hpp"
#include "meas_fetcher.hpp"

using namespace std;

class TcpCommand {
public:
  TcpCommand();
  string processCommand(string command);
  
  // name standarization done
  string processMeasIndexCommand(string command);
  string processMeasNameListCommand(string command);
  string processMeasShowCommand(string command);
  string processMeasRawForTimeCommand(string command);
  string processMeasRawForIndexCommand(string command);
  string processMeasRawHistoryForTimeCommand(string command);
  string processMeasStorageCommand(string command);
  string processMeasStatsCommand(string command);
  
  string processActionIndexCommand(string command);
  string processActionShowCommand(string command);
  string processActionExecuteCommand(string command);
  string processActionHistoryCommand(string command);
  
  string processOverseerIndexCommand(string command);
  string processOverseerShowCommand(string command);
  
  string processSettingsCommand(string command);
  string processStatsCommand(string command);
 
  long long int bootTime;
  
  void logInfo(string log); // overwritten color
  
  MeasTypeArray *measTypeArray;
  MeasFetcher *measFetcher;
  ActionTypeArray *actionTypeArray;
  OverseerArray *overseerArray;
  
};

#include "tcp_command.cpp"
#endif
