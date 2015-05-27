Spy::Spy() {
  cycleInterval = 30*60*1000*1000;
  usDelay = 10*1000*1000; // wait to warm up - get enough measurements
  lastTime = mTime();
  
  hiveHost = "http://localhost:3000";
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

// I know it's security issue 
void Spy::announceAll(){
  string command;
  
  for(vector<MeasType>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    command = "curl -X PUT -d \"meas_payload%5Bname%5D=";
    command += it->name;
    command += "&meas_payload%5Bsite%5D=";
    command += siteName;
    command += "&meas_payload%5Bvalue%5D=";
    command += to_string(it->lastValue());
    command += "\" \"";
    command += hiveHost;
    command += "/meas_payloads/announce.json\" ";
    
    if (quiet) {
      command += ">/dev/null 2>&1";
    }
    
    system(command.c_str());
  }

  logInfo("Spy: announce completed");
  lastTime = mTime();
}