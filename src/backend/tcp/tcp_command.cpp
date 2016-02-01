#include "tcp_command.hpp"

TcpCommand::TcpCommand() {
  bootTime = Helper::mTime();
}

// sample command: "meas;batt_u;0;100;"
std::string TcpCommand::processCommand(std::string command) {
  logArray->log("TcpCommand", command);

  std::string commandName, response;
  response = "{}";

  commandName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  if (commandName == "measIndex") {
    response = processMeasIndexCommand(command);
  }
  if (commandName == "measNameList") {
    response = processMeasNameListCommand(command);
  }
  if (commandName == "measShow") {
    response = processMeasShowCommand(command);
  }
  if (commandName == "measRawForTime") {
    response = processMeasRawForTimeCommand(command);
  }
  if (commandName == "measRawForIndex") {
    response = processMeasRawForIndexCommand(command);
  }
  if (commandName == "measRawHistoryForTime") {
    response = processMeasRawHistoryForTimeCommand(command);
  }
  if (commandName == "measStorage") {
    response = processMeasStorageCommand(command);
  }
  if (commandName == "measStats") {
    response = processMeasStatsCommand(command);
  }

  if (commandName == "measGroups") {
    response = processMeasGroupIndexCommand(command);
  }

  if (commandName == "actionIndex") {
    response = processActionIndexCommand(command);
  }
  if (commandName == "actionShow") {
    response = processActionShowCommand(command);
  }
  if (commandName == "actionExecute") {
    response = processActionExecuteCommand(command);
  }
  if (commandName == "actionHistory") {
    response = processActionHistoryCommand(command);
  }
  if (commandName == "overseerIndex") {
    response = processOverseerIndexCommand(command);
  }
  if (commandName == "overseerShow") {
    response = processOverseerShowCommand(command);
  }
  if (commandName == "settings") {
    response = processSettingsCommand(command);
  }
  if (commandName == "stats") {
    response = processStatsCommand(command);
  }

  // TODO large amount of text
  //logArray->log("TcpCommand", "response: " + response);

  return response;
}

// meas#index
std::string TcpCommand::processMeasIndexCommand(std::string command) {
  UNUSED(command);

  std::string response, detailsResponse;

  response = "{\"status\":0,\"array\":[";

  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::shared_ptr<MeasType> measType = *it;
    detailsResponse += measType->toJson() + ",";
  }

  // remove last coma
  if (detailsResponse[detailsResponse.size() - 1] == ',') {
    detailsResponse.resize(detailsResponse.size() - 1);
  }

  response += detailsResponse + "]}";

  return response;
}

// meas#name_list
std::string TcpCommand::processMeasNameListCommand(std::string command) {
  UNUSED(command);
  std::string response;

  response = "{\"status\":0,\"array\":[";

  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::shared_ptr<MeasType> measType = *it;
    response += "\"" + measType->name + "\",";
  }

  // remove last coma
  if (response[response.size() - 1] == ',') {
    response.resize(response.size() - 1);
  }
  response += "]}";

  return response;
}

// meas#show
std::string TcpCommand::processMeasShowCommand(std::string command) {
  std::string response, measName;
  measName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  std::shared_ptr<MeasType> foundMeasType = measTypeArray->byName(measName);

  if (foundMeasType) {
    response = "{\"status\":0,\"object\":" + foundMeasType->toJson() + "}";
  } else {
    response = "{\"status\":1,\"meas_type\":\"" + measName + "\",\"reason\":\"meas_not_found\"}";
  }

  return response;
}

// meas#raw_for_time
std::string TcpCommand::processMeasRawForTimeCommand(std::string command) {
  std::string measName, fromString, toString, response;
  unsigned long long tFrom, tTo;

  measName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  fromString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  toString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  std::stringstream(fromString) >> tFrom;
  std::stringstream(toString) >> tTo;

  std::shared_ptr<MeasType> foundMeasType = measTypeArray->byName(measName);

  if (foundMeasType) {
    logArray->log("TcpCommand", "processMeasRawForTimeCommand(" + measName + ", " + std::to_string(tFrom) + ", " + std::to_string(tTo) + ")" );

    std::string bufferString = foundMeasType->rawForTimeJson(tFrom, tTo);
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\"";
    response += ",\"lastTime\":" + std::to_string( foundMeasType->buffer->lastTime );
    response += ",\"firstTime\":" + std::to_string( foundMeasType->buffer->firstTime );
    response += ",\"interval\":" + std::to_string( foundMeasType->buffer->calcInterval() );
    response += ",\"count\":" + std::to_string( foundMeasType->buffer->count );
    response += ",\"usingMiliSeconds\":1";
    response += ",\"data\": " + bufferString;
    response += ",\"fromTime\":" + std::to_string( tFrom );
    response += ",\"toTime\":" + std::to_string( tTo );
    response += "}";
  } else {
    response = "{\"status\":1,\"meas_type\":\"" + measName + "\",\"reason\":\"meas_not_found\"}";
  }

  return response;
}

// meas#raw_for_time
std::string TcpCommand::processMeasRawHistoryForTimeCommand(std::string command) {
  std::string measName, fromString, toString, maxSizeString, response;
  unsigned long long tFrom, tTo;
  unsigned long int maxSize = 1;

  measName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  fromString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  toString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  maxSizeString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  std::stringstream(fromString) >> tFrom;
  std::stringstream(toString) >> tTo;
  std::stringstream(maxSizeString) >> maxSize;

  std::shared_ptr<MeasType> foundMeasType = measTypeArray->byName(measName);

  if (foundMeasType) {
    logArray->log("TcpCommand", "processMeasRawHistoryForTimeCommand(" + measName + ", " + std::to_string(tFrom) + ", " + std::to_string(tTo) + ", " + std::to_string(maxSize) + ")" );

    std::string bufferString = foundMeasType->rawHistoryForTimeJson(tFrom, tTo, maxSize);
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\"";
    response += ",\"lastTime\":" + std::to_string( foundMeasType->buffer->lastTime );
    response += ",\"firstTime\":" + std::to_string( foundMeasType->buffer->firstTime );
    response += ",\"originalInterval\":" + std::to_string( foundMeasType->buffer->calcInterval() );
    response += ",\"responseIndexInterval\":" + std::to_string( foundMeasType->buffer->responseIndexInterval );
    response += ",\"interval\":" + std::to_string( foundMeasType->buffer->calcInterval() * foundMeasType->buffer->responseIndexInterval );
    response += ",\"count\":" + std::to_string( foundMeasType->buffer->count );
    response += ",\"usingMiliSeconds\":1";
    response += ",\"data\": " + bufferString;
    response += ",\"fromTime\":" + std::to_string( tFrom );
    response += ",\"toTime\":" + std::to_string( tTo );
    response += "}";
  } else {
    response = "{\"status\":1,\"meas_type\":\"" + measName + "\",\"reason\":\"meas_not_found\"}";
  }

  return response;
}


// meas#raw_for_index
std::string TcpCommand::processMeasRawForIndexCommand(std::string command) {
  std::string measName, fromString, toString, response;
  unsigned long int iFrom, iTo;

  measName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  fromString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  toString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  std::stringstream(fromString) >> iFrom;
  std::stringstream(toString) >> iTo;

  std::shared_ptr<MeasType> foundMeasType = measTypeArray->byName(measName);

  if (foundMeasType) {
    logArray->log("TcpCommand", "processMeasRawForIndexCommand(" + measName + ", " + std::to_string(iFrom) + ", " + std::to_string(iTo) + ")" );

    std::string bufferString = foundMeasType->rawForIndexJson(iFrom, iTo);
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\"";
    response += ",\"lastTime\":" + std::to_string( foundMeasType->buffer->lastTime );
    response += ",\"firstTime\":" + std::to_string( foundMeasType->buffer->firstTime );
    response += ",\"interval\":" + std::to_string( foundMeasType->buffer->calcInterval() );
    response += ",\"count\":" + std::to_string( foundMeasType->buffer->count );
    response += ",\"usingMiliSeconds\":1";
    response += ",\"data\": " + bufferString;
    response += ",\"fromIndex\":" + std::to_string( iFrom );
    response += ",\"toIndex\":" + std::to_string( iTo );
    response += "}";
  } else {
    response = "{\"status\":1,\"meas_type\":\"" + measName + "\",\"reason\":\"meas_not_found\"}";
  }

  return response;
}

// meas#storage
std::string TcpCommand::processMeasStorageCommand(std::string command) {
  std::string measName, fromString, toString, response;
  unsigned long long from, to;

  measName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  fromString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  toString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  std::stringstream(fromString) >> from;
  std::stringstream(toString) >> to;

  std::shared_ptr<MeasType> foundMeasType = measTypeArray->byName(measName);

  if (foundMeasType) {
    std::string bufferString = foundMeasType->storageJson(from, to);
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\",";
    response +=  "\"data\":" + bufferString;
    response += "}";
  } else {
    response = "{\"status\":1,\"meas_type\":\"" + measName + "\",\"reason\":\"meas_not_found\"}";
  }
  return response;
}

// meas#stats
std::string TcpCommand::processMeasStatsCommand(std::string command) {
  std::string measName, fromString, toString, response;
  unsigned long long from, to;

  measName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  fromString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  toString = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  std::stringstream(fromString) >> from;
  std::stringstream(toString) >> to;

  std::shared_ptr<MeasType> foundMeasType = measTypeArray->byName(measName);

  if (foundMeasType) {
    std::string statsString = foundMeasType->statsJson(from, to);
    response = "{\"status\":0,\"meas_type\":\"" + measName + "\",";
    response +=  "\"data\":" + statsString;
    response += "}";
  } else {
    response = "{\"status\":1,\"meas_type\":\"" + measName + "\",\"reason\":\"meas_not_found\"}";
  }
  return response;
}


// meas_groups#index
std::string TcpCommand::processMeasGroupIndexCommand(std::string command) {
  UNUSED(command);

  std::string response, detailsResponse;

  response = "{\"status\":0,\"array\":";
  response += measGroup->toJson() + "}";

  return response;
}


// actions#index
std::string TcpCommand::processActionIndexCommand(std::string command) {
  UNUSED(command);
  std::string response, detailsResponse;

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

// action#show
std::string TcpCommand::processActionShowCommand(std::string command) {
  std::string response, actionName;
  actionName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  ActionType *foundActionType = actionTypeArray->byName(actionName);

  if (foundActionType) {
    response = "{\"status\":0,\"object\":" + foundActionType->toJson() + "}";
  } else {
    response = "{\"status\":1,\"action_type\":\"" + actionName + "\",\"reason\":\"action_not_found\"}";
  }

  return response;
}

// action#execute
std::string TcpCommand::processActionExecuteCommand(std::string command) {
  std::string actionName;
  actionName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  std::string response;

  ActionType *foundActionType = actionTypeArray->byName(actionName);

  if (foundActionType) {
    foundActionType->execute();
    response = "{\"status\":0,\"action\":" + foundActionType->toJson() + "}";
  } else {
    response = "{\"status\":1,\"action\":\"" + actionName + "\",\"reason\":\"action_not_found\"}";
  }

  return response;
}

// action#history
std::string TcpCommand::processActionHistoryCommand(std::string command) {
  std::string actionName;
  actionName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);

  std::string response;

  ActionType *foundActionType = actionTypeArray->byName(actionName);

  if (foundActionType) {
    response = "{\"status\":0,\"action\":" + foundActionType->historyToJson() + "}";
  } else {
    response = "{\"status\":1,\"action\":\"" + actionName + "\",\"reason\":\"action_not_found\"}";
  }

  return response;
}


// overseers#index
std::string TcpCommand::processOverseerIndexCommand(std::string command) {
  UNUSED(command);
  std::string response, detailsResponse;

  response = "{\"status\":0,\"array\":[";

  for(std::vector<Overseer>::iterator it = overseerArray->overseers.begin(); it != overseerArray->overseers.end(); ++it) {
    detailsResponse += it->toJson() + ",";
  }

  // remove last coma
  if (detailsResponse[detailsResponse.size() - 1] == ',') {
    detailsResponse.resize(detailsResponse.size() - 1);
  }

  response += detailsResponse + "]}";

  return response;
}

// overseers#show
std::string TcpCommand::processOverseerShowCommand(std::string command) {
  std::string response, overseerName;
  overseerName = command.substr(0, command.find(";"));
  command = command.substr(command.find(";") + 1);
  Overseer *foundOverseer = overseerArray->byName(overseerName);

  if (foundOverseer) {
    response = "{\"status\":0,\"object\":" + foundOverseer->toJson() + "}";
  } else {
    response = "{\"status\":1,\"overseer\":\"" + overseerName + "\",\"reason\":\"overseer_not_found\"}";
  }

  return response;
}

// settings
std::string TcpCommand::processSettingsCommand(std::string command) {
  UNUSED(command);
  std::string response, measResponse, frontendResponse;

  measResponse = "{";
  measResponse += "\"betweenMeasInterval\":" + std::to_string(measFetcher->betweenMeasInterval / 1000) + ",";
  measResponse += "\"cycleInterval\":" + std::to_string(measFetcher->cycleInterval / 1000);
  measResponse += "}";

  frontendResponse = "{";
  frontendResponse += "\"intervalCurrent\":" + std::to_string(frontendSettings->intervalCurrent ) + ",";
  frontendResponse += "\"currentCoeff\":" + std::to_string(frontendSettings->currentCoeff ) + ",";
  frontendResponse += "\"intervalHistory\":" + std::to_string(frontendSettings->intervalHistory);
  frontendResponse += "}";

  response = "{\"status\":0,\"object\":{";
  response += "\"meas\":" + measResponse;
  response += ",";
  response += "\"frontend\":" + frontendResponse;
  response += "}}";

  return response;
}

// stats
std::string TcpCommand::processStatsCommand(std::string command) {
  UNUSED(command);
  std::string response, timeResponse, resourceResponse;

  timeResponse = "{";
  timeResponse += "\"bootTime\":" + std::to_string(bootTime) + ",";
  timeResponse += "\"time\":" + std::to_string(Helper::mTime()) + ",";
  timeResponse += "\"uptime\":" + std::to_string(Helper::mTime() - bootTime);
  timeResponse += "}";

  // http://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-run-time-in-c
  double vmUsage;
  double residentSet;
  Helper::processMemUsage(vmUsage, residentSet);

  resourceResponse = "{";
  resourceResponse += "\"vmUsage\":" + std::to_string(vmUsage) + ",";
  resourceResponse += "\"residentSet\":" + std::to_string(residentSet);
  resourceResponse += "}";

  response = "{\"status\":0,\"object\":{";
  response += "\"time\":" + timeResponse + ",";
  response += "\"resources\":" + resourceResponse;
  response += "}}";

  return response;
}
