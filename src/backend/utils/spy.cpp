#include "spy.hpp"

Spy::Spy() {
  cycleInterval = 30*60*1000*1000;
  usDelay = 10*1000*1000; // wait to warm up - get enough measurements
  lastTime = Helper::mTime();

  hiveHost = "http://localhost:3000";
  urlPath = "/meas_payloads/announce.json";
  siteName = "test";

  quiet = true;
  enabled = false; // disabled by default

  isRunning = true;
  ready = false;
  changing = false;
  work = false;
}


void Spy::start() {
  ready = true;

  while(isRunning) {
    if (enabled) {
      announceAll();
    }

    Helper::longSleep(cycleInterval);
  };
}

void Spy::stop() {
  isRunning = false;
  shutdownMutex.lock();
  shutdownMutex.unlock();

  logArray->log("Spy", "stop");
}


void Spy::announceAll() {
  work = true;
  url = hiveHost + urlPath;

  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    shutdownMutex.lock();
    std::shared_ptr<MeasType> measType = *it;
    annouceMeas(measType->name, measType->lastValue());
    shutdownMutex.unlock();
  }

  lastTime = Helper::mTime();
  logArray->log("Spy", "announce completed");
  work = false;
}

unsigned char Spy::annouceMeas(std::string name, double value) {
// TODO there is problem cross compiling it to arm
#ifndef __arm__
  try {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::Verbose(quiet == false));

    // there is problem with curlpp and clang, sorry
#if not defined(__clang_version__)
    std::list<std::string> header;
    header.push_back( std::string("Content-Type: application/json") );
    header.push_back( std::string("Accept: application/json") );
    request.setOpt(new curlpp::options::HttpHeader(header));
#endif

    std::string command;
    command = "{\"meas_payload\": {\"site\": \"" + siteName + "\", \"name\": \"" + name + "\", \"value\": \"" + std::to_string(value) + "\"}}";

    request.setOpt(new curlpp::options::PostFields(command));
    request.setOpt(new curlpp::options::PostFieldSize( (const long) command.length() ));

    request.perform();

    return 0;

  } catch ( curlpp::LogicError & e ) {
    logArray->logError("Spy", e.what());
    return 1;
  } catch ( curlpp::RuntimeError & e ) {
    logArray->logError("Spy", e.what());
    return 1;
  }
#endif
}
