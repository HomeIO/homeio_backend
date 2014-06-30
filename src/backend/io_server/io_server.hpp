#ifndef IO_SERVER_HPP
#define	IO_SERVER_HPP

using namespace std;

#include "config.hpp"
#include "io_tcp_server.hpp"
#include "rs.hpp"

class IoServer {
public:
  int start();
  
};

#include "io_server.cpp"
#endif
