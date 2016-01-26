#include "tcp_server.hpp"

TcpServer::TcpServer() {
  usDelay = 2000000;

  isRunning = true;
  ready = false;
}

void TcpServer::start() {
  Helper::longSleep(usDelay);
  // wait for enough measurements
  measTypeArray->delayTillReady();

  port = 2005;
  commandBuffer = (char*)malloc(COMMAND_BUFFER_SIZE*sizeof(char));

  int conn_s; // connection socket
  int list_s = createTcpServer();

  logArray->log("TCP", "started on " + std::to_string(port));

  ready = true;

  while (isRunning) {
    // Wait for a connection, then accept() it
    if ((conn_s = accept(list_s, NULL, NULL)) < 0) {
      logArray->logError("TCP", "TcpServer::start(): Error calling accept()");
      exit(EXIT_FAILURE);
    }

    shutdownMutex.lock();

    // Retrieve command
    readLine(conn_s, MAX_LINE - 1);

    processCommand();
    writeLine(conn_s);

    shutdownMutex.unlock();

    // Close the connected socket
    if (close(conn_s) < 0) {
      logArray->logError("TCP", "TcpServer::start(): Error calling close()");
      exit(EXIT_FAILURE);
    }
  }

}

void TcpServer::stop() {
  isRunning = false;
  shutdownMutex.lock();
  shutdownMutex.unlock();
  logArray->log("TCP", "stop");
}

int TcpServer::processCommand() {
  std::string command;
  command = (std::string) commandBuffer;
  responseBuffer = tcpCommand->processCommand(command);

  return 0;
}

// Read line from socket
ssize_t TcpServer::readLine(int sockd, size_t maxlen) {
  size_t n;
  ssize_t rc;

  //MAX_LINE
  for (n = 0; n < maxlen; n++) {

    if ((rc = read(sockd, &c, 1)) == 1) {
      commandBuffer[n] = c;
      if (c == '\n')
        break;
    } else if (rc == 0) {
      if (n == 1)
        return 0;
      else
        break;
    } else {
      if (errno == EINTR)
        continue;
      return -1;
    }
  }

  commandBuffer[n] = 0;

  return (ssize_t) n;
}

// Write line to socket
ssize_t TcpServer::writeLine(int sockd) {
  ssize_t nwritten;
  if ((nwritten = write(sockd, responseBuffer.c_str(), responseBuffer.length())) <= 0) {
    if (errno == EINTR)
      return (ssize_t) nwritten;
    else
      return (ssize_t) -1;
  }

  return (ssize_t) 0;
}

// Create TCP listening socket
int TcpServer::createTcpServer() {
  // start TCP server
  int list_s; /*  listening socket          */
  struct sockaddr_in servaddr; /*  socket address structure  */

  //char *endptr;

  /*  Create the listening socket  */
  if ((list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    logArray->logError("TCP", "TcpServer::createTcpServer(): Error creating listening socket.");
    exit(EXIT_FAILURE);
  }

  // http://stackoverflow.com/questions/548879/releasing-bound-ports-on-process-exit/548912#548912
  // allow faster restart
  int iSetOption = 1;
  setsockopt(list_s, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));


  /*  Set all bytes in socket address structure to
      zero, and fill in the relevant data members   */

  memset(&servaddr, 0, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);


  /*  Bind our socket addresss to the
      listening socket, and call listen()  */

  if (bind(list_s, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
    logArray->logError("TCP", "TcpServer::createTcpServer(): Error calling bind()");
    exit(EXIT_FAILURE);
  }

  if (listen(list_s, LISTENQ) < 0) {
    logArray->logError("TCP", "TcpServer::createTcpServer(): Error calling listen()");
    exit(EXIT_FAILURE);
  }

  logArray->log("TCP", "server prepared");

  return list_s;
}
