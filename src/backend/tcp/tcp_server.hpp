#ifndef TCP_SERVER
#define	TCP_SERVER

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <mutex>
#include <memory>

#include <sys/socket.h> // socket definitions
#include <sys/types.h> // socket types
#include <arpa/inet.h> // inet (3) funtions
#include <unistd.h> // misc. UNIX functions
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "tcp_command.hpp"
#include "../meas/meas_type_array.hpp"
#include "../log/log_array.hpp"

#define LISTENQ (1024) // Backlog for listen()
#define MAX_LINE (5000)
#define COMMAND_BUFFER_SIZE 2000

class TcpServer {
 public:
  TcpServer();

  ssize_t readLine(int sockd, size_t maxlen);
  ssize_t writeLine(int sockd);
  int createTcpServer();
  void start();
  void stop();
  int processCommand();

  unsigned short int port;

  std::shared_ptr<TcpCommand> tcpCommand;
  std::shared_ptr<MeasTypeArray> measTypeArray;

  std::shared_ptr<LogArray> logArray;

  unsigned long long usDelay;

  bool isRunning;
  bool ready;
  bool changing;
  std::mutex shutdownMutex;

 private:
  char c, *commandBuffer;
  std::string responseBuffer;
};

//#include "tcp_server.cpp"
#endif
