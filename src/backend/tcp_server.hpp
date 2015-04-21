#ifndef TCP_SERVER
#define	TCP_SERVER

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h> // socket definitions
#include <sys/types.h> // socket types
#include <arpa/inet.h> // inet (3) funtions
#include <unistd.h> // misc. UNIX functions
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "tcp_command.hpp"

#define LISTENQ (1024) // Backlog for listen()
#define MAX_LINE (5000)
#define COMMAND_BUFFER_SIZE 2000

using namespace std;

class TcpServer {
public:
  TcpServer();
  
  ssize_t readLine(int sockd, size_t maxlen);
  ssize_t writeLine(int sockd);
  int createTcpServer();
  int start();
  int processCommand();
  
  short int port;
  TcpCommand *tcpCommand;
  
  unsigned long long usDelay;
  
private:
  char c, *commandBuffer;
  string responseBuffer;
};

#include "tcp_server.cpp"
#endif
