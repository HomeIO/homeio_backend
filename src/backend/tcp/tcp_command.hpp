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

class TcpCommand {
 public:
  TcpCommand();
  std::string processCommand(std::string command);

  // name standarization done
  std::string processMeasIndexCommand(std::string command);
  std::string processMeasNameListCommand(std::string command);
  std::string processMeasShowCommand(std::string command);
  std::string processMeasRawForTimeCommand(std::string command);
  std::string processMeasRawForIndexCommand(std::string command);
  std::string processMeasRawHistoryForTimeCommand(std::string command);
  std::string processMeasStorageCommand(std::string command);
  std::string processMeasStatsCommand(std::string command);

  std::string processMeasGroupIndexCommand(std::string command);

  std::string processActionIndexCommand(std::string command);
  std::string processActionShowCommand(std::string command);
  std::string processActionExecuteCommand(std::string command);
  std::string processActionHistoryCommand(std::string command);

  std::string processOverseerIndexCommand(std::string command);
  std::string processOverseerShowCommand(std::string command);

  std::string processSettingsCommand(std::string command);
  std::string processStatsCommand(std::string command);

  unsigned long long int bootTime;

  std::shared_ptr<MeasFetcher> measFetcher;
  std::shared_ptr<MeasTypeArray> measTypeArray;
  std::shared_ptr<ActionTypeArray> actionTypeArray;
  std::shared_ptr<OverseerArray> overseerArray;
  std::shared_ptr<FrontendSettings> frontendSettings;
  std::shared_ptr<MeasGroup> measGroup;
  
  LogArray *logArray;

};

//#include "tcp_command.cpp"
#endif
