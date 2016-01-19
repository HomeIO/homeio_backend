#ifndef IO_SERVER_HPP
#define	IO_SERVER_HPP

#include <mutex>

#include "io_config.hpp"
#include "io_tcp_server.hpp"
#include "io_rs.hpp"
#include "../log/log_array.hpp"

class IoServer {
 public:
  IoServer();
  void start();
  void stop();

  bool isRunning;
  std::mutex shutdownMutex;

  // temporary char used for sending command (loop)
  unsigned char tmp_char, i;
  // count of command bytes
  unsigned char count_command;
  // count of command response
  unsigned char count_response;
  // RS device
  std::string port;
  // is ready
  bool ready;

  IoRS *rs;
  IoTcpServer *tcp;
  LogArray *logArray;

};

//#include "io_server.cpp"
#endif
