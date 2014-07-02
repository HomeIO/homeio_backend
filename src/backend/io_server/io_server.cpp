IoServer::IoServer() {
  rs = new RS;
  tcp = new IoTcpServer;
  
  tmp_char = 0;
  i = 0;
  count_command = 0;
  port = "/dev/ttyUSB0";
  rs->port = port;
  
  tcp->port = IO_TCP_SERVER_PORT;
  rs->buffer = tcp->buffer;
}

int IoServer::start() {
  rs->openRS();
  tcp->createTcpServer();
  
  while(1) {
    // Retrieve command
    tcp->waitForCommand();
    tcp->readTcp();
   
    tcp->count_response = rs->send();
    
    //tcp->buffer[0] = 80;
    //tcp->buffer[1] = 80;
    //tcp->buffer[2] = 0;
    //tcp->count_response = 2;
   
    tcp->writeTcp();
    tcp->closeSocket();
  }
  
  return 0;
}