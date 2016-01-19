#ifndef TCP_COMMAND
#define	TCP_COMMAND

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

#include "../meas/meas_type_array.hpp"
#include "../meas/meas_fetcher.hpp"
#include "../meas/meas_group.hpp"

#include "../action/action_type_array.hpp"

#include "../overseer/overseer_array.hpp"

#include "../utils/frontend_settings.hpp"

#include "../log/log_array.hpp"

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

  string processMeasGroupIndexCommand(string command);

  string processActionIndexCommand(string command);
  string processActionShowCommand(string command);
  string processActionExecuteCommand(string command);
  string processActionHistoryCommand(string command);

  string processOverseerIndexCommand(string command);
  string processOverseerShowCommand(string command);

  string processSettingsCommand(string command);
  string processStatsCommand(string command);

  unsigned long long int bootTime;

  MeasTypeArray *measTypeArray;
  MeasFetcher *measFetcher;
  ActionTypeArray *actionTypeArray;
  OverseerArray *overseerArray;
  FrontendSettings *frontendSettings;
  MeasGroup *measGroup;
  LogArray *logArray;

};

//#include "tcp_command.cpp"
#endif
