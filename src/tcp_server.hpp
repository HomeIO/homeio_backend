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

#define LISTENQ (1024) // Backlog for listen()
#define MAX_LINE (5000)

using namespace std;

class TcpServer {
public:
  ssize_t readLine(int sockd, void *vptr, size_t maxlen);
  ssize_t writeLine(int sockd, const void *vptr, size_t n);
  int createTcpServer();
  int start();
  
  short int port;
  
private:
  char c, *buffer;  
};

#include "tcp_server.cpp"
#endif
