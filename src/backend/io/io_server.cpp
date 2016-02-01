#include "io_server.hpp"

IoServer::IoServer() {
  ready = false;
  changing = false;

  rs = std::make_unique<IoRS>();
  tcp = std::make_unique<IoTcpServer>();

  tmp_char = 0;
  i = 0;
  count_command = 0;
  port = "/dev/ttyUSB0";

  tcp->port = IO_TCP_SERVER_PORT;
  rs->buffer = tcp->buffer;

  isRunning = true;
}

void IoServer::start() {
  changing = true;

  rs->port = port;
  rs->openRS();
  tcp->createTcpServer();

  ready = true;
  changing = false;

  usleep(10000);
  logArray->log("IoServer", "start");

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
  isRunning = false;
  changing = true;

  shutdownMutex.lock();
  shutdownMutex.unlock();
  logArray->log("IoServer", "stop");

  ready = false;
  changing = false;
}
