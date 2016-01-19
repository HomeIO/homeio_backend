#include "io_tcp_server.hpp"

IoTcpServer::IoTcpServer() {
  verbose = false;
}

// Create TCP listening socket
int IoTcpServer::createTcpServer() {
  if ((list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    logArray->logError("IoServer", "ECHOSERV: Error creating listening socket.");
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
    logArray->logError("IoServer", "ECHOSERV: Error calling bind()");
    exit(EXIT_FAILURE);
  }

  if (listen(list_s, IO_SERVER_LISTENQ) < 0) {
    logArray->logError("IoServer", "ECHOSERV: Error calling listen()");
    exit(EXIT_FAILURE);
  }

  logArray->log("IoServer", "TCP server started on port " + std::to_string(port));

  return list_s;
}

int IoTcpServer::waitForCommand() {
  if ((conn_s = accept(list_s, NULL, NULL)) < 0) {
    logArray->logError("IoServer", "ECHOSERV: Error calling accept()");
    exit(EXIT_FAILURE);
  }

  return conn_s;
}

// Read line from socket
ssize_t IoTcpServer::readTcp() {
  ssize_t n;
  n = readLine(conn_s, (char *) buffer, IO_SERVER_MAX_LINE - 1);

  escapeBuffer();
  if (verbose) {
    printf("'%s'", verbose_buffer);
  }
  return n;
}

ssize_t IoTcpServer::writeTcp() {
  ssize_t n;

  if (verbose) {
    printf(" - '%s'\n", buffer);
  }

  n = writeLine(conn_s, (char *) buffer, count_response);
  return n;
}

void IoTcpServer::closeSocket() {
  if (close(conn_s) < 0) {
    logArray->logError("IoServer", "ECHOSERV: Error calling close()");
    exit(EXIT_FAILURE);
  }
}

void IoTcpServer::escapeBuffer() {
  std::strcpy(verbose_buffer, (char*) buffer);
  size_t n;
  for (n=0; n<strlen(verbose_buffer); n++) {
    if (verbose_buffer[n] == '\n') {
      verbose_buffer[n] = ' ';
    }
  }
}

// Read line from socket
ssize_t IoTcpServer::readLine(int sockd, char *vptr, size_t maxlen) {
  ssize_t n;
  ssize_t rc;
  char c, *buffer;

  buffer = vptr;

  for (n = 1; n < (ssize_t) maxlen; n++) {

    if ((rc = read(sockd, &c, 1)) == 1) {
      *buffer++ = c;
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

  *buffer = 0;
  return n;
}

// Write line to socket
ssize_t IoTcpServer::writeLine(int sockd, const char *vptr, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *buffer;

  buffer = vptr;
  nleft = n;

  while (nleft > 0) {
    if ((nwritten = write(sockd, buffer, nleft)) <= 0) {
      if (errno == EINTR)
        nwritten = 0;
      else
        return -1;
    }
    nleft = nleft - (size_t) nwritten;
    buffer += nwritten;
  }

  return (ssize_t) n;
}
