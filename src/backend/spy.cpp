Spy::Spy() {
  cycleInterval = 30*60*1000*1000;
  usDelay = 10*1000*1000; // wait to warm up - get enough measurements
  lastTime = mTime();
  
  hiveHost = "http://localhost:3000";
  urlPath = "/meas_payloads/announce.json";
  siteName = "test";
  
  quiet = true;
  enabled = false; // disabled by default
}


void Spy::start()
{
  usleep(usDelay);
  
  while(true) {
    if (enabled) {
      announceAll();
    }
    
    usleep(cycleInterval);
  };
}

void Spy::announceAll(){
  url = hiveHost + urlPath;
  
  for(vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    annouceMeas(it->name, it->lastValue());
  }

  logInfo("Spy: announce completed");
  lastTime = mTime();
}

unsigned char Spy::annouceMeas(string name, float value) {
  try {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    
    request.setOpt(new curlpp::options::Url(url)); 
    request.setOpt(new curlpp::options::Verbose(quiet == false)); 
    
    std::list<std::string> header; 
    header.push_back("Content-Type: application/json");
    header.push_back("Accept: application/json"); 
    
    request.setOpt(new curlpp::options::HttpHeader(header)); 
    
    string command;
    command = "{\"meas_payload\": {\"site\": \"" + siteName + "\", \"name\": \"" + name + "\", \"value\": \"" + to_string(value) + "\"}}";
    
    request.setOpt(new curlpp::options::PostFields(command));
    request.setOpt(new curlpp::options::PostFieldSize(command.length()));
    
    request.perform();
    
  }
  catch ( curlpp::LogicError & e ) {
    logError(e.what());
  }
  catch ( curlpp::RuntimeError & e ) {
    logError(e.what());
  }

}