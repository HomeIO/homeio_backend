int TcpServer::start() {
  port = 2005;
  commandBuffer = (char*)malloc(COMMAND_BUFFER_SIZE*sizeof(char));
  
  int conn_s; // connection socket
  int list_s = createTcpServer();
  
  cout << "TCP Server started on " << port << endl;
  
  while (1) {
    // Wait for a connection, then accept() it
    if ((conn_s = accept(list_s, NULL, NULL)) < 0) {
      fprintf(stderr, "ECHOSERV: Error calling accept()\n");
      exit(EXIT_FAILURE);
    }
    
    // Retrieve command
    readLine(conn_s, MAX_LINE - 1);
    
    // command and response char count
    char command = commandBuffer[0];
    char count_response = 1;
    
    processCommand();
    
    writeLine(conn_s);

    // Close the connected socket
    if (close(conn_s) < 0) {
      fprintf(stderr, "ECHOSERV: Error calling close()\n");
      exit(EXIT_FAILURE);
    }
  }
  
  return 0;
}

int TcpServer::processCommand() {
  // get name
  string measName, command;
  command = (string) commandBuffer;
  measName = command.substr(0, command.find(";"));
  
  responseBuffer = "{done: 1, meas: " + measName + "}";
    
  return 0;
}

// Read line from socket
ssize_t TcpServer::readLine(int sockd, size_t maxlen) {
    ssize_t n, rc;

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

    cout << n << endl;
    cout << commandBuffer << endl;
    cout << sizeof(commandBuffer) << endl;

    return n;
}

// Write line to socket
ssize_t TcpServer::writeLine(int sockd) {
  size_t nwritten;
  if ((nwritten = write(sockd, responseBuffer.c_str(), responseBuffer.length())) <= 0) {
        if (errno == EINTR)
            return nwritten;
        else
            return -1;
    }
}

// Create TCP listening socket
int TcpServer::createTcpServer() {
    // start TCP server
    int list_s; /*  listening socket          */
    struct sockaddr_in servaddr; /*  socket address structure  */
    
    char *endptr; /*  for strtol()              */



    /*  Create the listening socket  */

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

    if (listen(list_s, LISTENQ) < 0) {
        fprintf(stderr, "ECHOSERV: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }

    cout << "list_s " << list_s << endl;
    
    return list_s;

}
