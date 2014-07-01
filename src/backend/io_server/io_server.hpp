#ifndef IO_SERVER_HPP
#define	IO_SERVER_HPP

using namespace std;

#include "config.hpp"
#include "io_tcp_server.hpp"
#include "rs.hpp"

class IoServer {
public:
  // temporary char used for sending command (loop)
  unsigned char tmp_char, i;
  // count of command bytes
  unsigned char count_command;
  // RS device
  string port;
  
  
  RS *rs;
  IoTcpServer *tcp;
  
  IoServer();
  int start();
  
};

#include "io_server.cpp"
#endif
