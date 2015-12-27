#ifndef IO_SERVER_HPP
#define	IO_SERVER_HPP

using namespace std;

#include <mutex>

#include "config.hpp"
#include "io_tcp_server.hpp"
#include "rs.hpp"

class IoServer {
public:
  IoServer();
  void start();
  void stop();

  bool isRunning;
  mutex shutdownMutex;

  // temporary char used for sending command (loop)
  unsigned char tmp_char, i;
  // count of command bytes
  unsigned char count_command;
  // count of command response
  unsigned char count_response;
  // RS device
  string port;
  // is ready
  bool ready;

  RS *rs;
  IoTcpServer *tcp;

};

//#include "io_server.cpp"
#endif
