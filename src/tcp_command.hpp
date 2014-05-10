#ifndef TCP_COMMAND
#define	TCP_COMMAND

#include "homeio.hpp"

using namespace std;

class TcpCommand {
public:
  TcpCommand();
  HomeIO *parent;
};

#include "tcp_command.cpp"
#endif
