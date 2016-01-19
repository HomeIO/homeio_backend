#ifndef IO_TCP_SERVER_HPP
#define	IO_TCP_SERVER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include <sys/socket.h> // socket definitions
#include <sys/types.h> // socket types
#include <arpa/inet.h> // inet (3) funtions
#include <unistd.h> // misc. UNIX functions
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "io_config.hpp"
#include "../utils/helper.hpp"
#include "../log/log_array.hpp"

#define IO_SERVER_LISTENQ (1024) // Backlog for listen()
#define IO_SERVER_MAX_LINE (1000)

class IoTcpServer {
 public:
  // port number
  unsigned short port;
  // listening socket
  int list_s;
  // socket address structure
  struct sockaddr_in servaddr;
  // for strtol()
  char *endptr;
  // connection socket
  int conn_s;
  // TCP buffer
  unsigned char buffer[IO_SERVER_MAX_LINE];
  // count of response bytes
  unsigned char count_response = 0;
  // just for logging
  char verbose_buffer[80];
  time_t rawtime;
  struct tm * timeinfo;
  bool verbose;

  LogArray *logArray;

  IoTcpServer();
  int createTcpServer();
  int waitForCommand();
  ssize_t readTcp();
  ssize_t writeTcp();
  void closeSocket();

  void escapeBuffer();

  // Read line from socket
  ssize_t readLine(int sockd, char *vptr, size_t maxlen);
  // Write line to socket
  ssize_t writeLine(int sockd, const char *vptr, size_t n);

};

//#include "io_tcp_server.cpp"
#endif
