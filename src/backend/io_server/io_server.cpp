IoServer::IoServer() {
  ready = false;
  
  rs = new RS;
  tcp = new IoTcpServer;
  
  tmp_char = 0;
  i = 0;
  count_command = 0;
  port = "/dev/ttyUSB0";
  
  tcp->port = IO_TCP_SERVER_PORT;
  rs->buffer = tcp->buffer;
  
  isRunning = true;
}

void IoServer::start() {
  rs->port = port;
  
  rs->openRS();
  tcp->createTcpServer();
  
  ready = true;
  
  while(isRunning) {
    // Retrieve command
    tcp->waitForCommand();
    
    shutdownMutex.lock();
    tcp->readTcp();
    tcp->count_response = rs->send();
    tcp->writeTcp();
    tcp->closeSocket();
    shutdownMutex.unlock();
  }
}

void IoServer::stop() {
  shutdownMutex.lock();
  logInfo("IoServer - stop");
}
