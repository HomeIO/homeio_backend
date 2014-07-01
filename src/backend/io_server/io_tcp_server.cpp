// Create TCP listening socket
int IoTcpServer::createTcpServer() {
    if ((list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }

    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);


    /*  Bind our socket addresss to the
        listening socket, and call listen()  */

    if (bind(list_s, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
        fprintf(stderr, "ECHOSERV: Error calling bind()\n");
        exit(EXIT_FAILURE);
    }

    if (listen(list_s, IO_SERVER_LISTENQ) < 0) {
        fprintf(stderr, "ECHOSERV: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }

    return list_s;

}

int IoTcpServer::waitForCommand() {
  if ((conn_s = accept(list_s, NULL, NULL)) < 0) {
    fprintf(stderr, "ECHOSERV: Error calling accept()\n");
    exit(EXIT_FAILURE);
  }
  
  return conn_s;
}

// Read line from socket
ssize_t IoTcpServer::readTcp() {
  ssize_t n;
  n = readLine(conn_s, buffer, MAX_LINE - 1);
  return n;
}

void IoTcpServer::afterReadCommand() {
  time_t t = time(0);
  printf("Time %srcv %s", ctime(&t), buffer);
}

ssize_t IoTcpServer::writeTcp() {
  ssize_t n;
  n = writeLine(conn_s, buffer, count_response);
  return n;
}


// Read line from socket
ssize_t IoTcpServer::readLine(int sockd, char *vptr, size_t maxlen) {
    ssize_t n, rc;
    char c, *buffer;

    buffer = vptr;

    for (n = 1; n < maxlen; n++) {

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
        nleft -= nwritten;
        buffer += nwritten;
    }

    return n;
}

