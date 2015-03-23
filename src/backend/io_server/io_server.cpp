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
}

int IoServer::start() {
  rs->port = port;
  
  rs->openRS();
  tcp->createTcpServer();
  
  ready = true;
  
  while(1) {
    // Retrieve command
    tcp->waitForCommand();
    tcp->readTcp();
   
    tcp->count_response = rs->send();
    tcp->writeTcp();
    tcp->closeSocket();
  }
  
  return 0;
}