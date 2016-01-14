#ifndef IO_PROXY
#define	IO_PROXY

#include <iostream>    //cout
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <string>  //string
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent
#include <unistd.h>
#include <mutex>

#include "../helper.hpp"
#include "../log/log_array.hpp"

class IoProxy {
public:
  IoProxy();
  std::string address;
  uint16_t port;
  bool verbose;
  std::mutex tcpMutex;
  LogArray *logArray;

  unsigned int fetch(unsigned char commandChar, unsigned char responseSize);

  unsigned int prepareSocket();
  int connectSocket();
  int disconnectSocket();

private:
  int sock;
  struct sockaddr_in server;
  unsigned char buffer[16];

};

//#include "io_proxy.cpp"
#endif
