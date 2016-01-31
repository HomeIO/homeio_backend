#include "io_proxy.hpp"

// http://www.binarytides.com/code-a-simple-socket-client-class-in-c/
IoProxy::IoProxy() {
  port = 0;
  address = "";
  verbose = false;

  tmpAddr = new in_addr;
}

IoProxy::~IoProxy() {
  delete tmpAddr;
}

unsigned int IoProxy::fetch(unsigned char commandChar, unsigned char responseSize) {
  tcpMutex.lock();
  // after mutex

  connectSocket();

  char commandArray[4];
  commandArray[0] = 1; // size of command
  commandArray[1] = (char) responseSize; // size of response
  commandArray[2] = (char) commandChar; // command
  commandArray[3] = '\n'; // end, it's needed

  if( send(sock, commandArray, sizeof(commandArray), 0) < 0) {
    logArray->logError("IoProxy", "Send failed: " + std::to_string( (unsigned int) commandChar ));
    tcpMutex.unlock(); // end of mutex
    return 1;
  }

  //Receive a reply from the server
  if( recv(sock, buffer, responseSize, 0) < 0) {
    logArray->logError("IoProxy", "recv failed");
  }

  disconnectSocket();
  tcpMutex.unlock(); // end of mutex

  unsigned int raw = 0;
  unsigned char part_raw = 0;

  for (unsigned char i = 0; i < responseSize; i++) {
    part_raw = buffer[i];
    raw *= 256;
    raw += part_raw;
  }

  return raw;
}


unsigned int IoProxy::prepareSocket() {
  // crazy way to get rid of warning
  // it checks if address is ip or name based, like localhost
  bool isIp = (inet_aton(address.c_str(), tmpAddr) > 0);

  //setup address structure
  if(isIp == false) {
    struct hostent *he;
    struct in_addr **addr_list;

    //resolve the hostname, its not an ip address
    if ( (he = gethostbyname( address.c_str() ) ) == NULL) {
      logArray->logError("IoProxy", "Failed to resolve hostname " + address);

      return false;
    }

    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **) he->h_addr_list;

    for(int i = 0; addr_list[i] != NULL; i++) {
      //strcpy(ip , inet_ntoa(*addr_list[i]) );
      server.sin_addr = *addr_list[i];
      break;
    }
  }

  //plain ip address
  else {
    server.sin_addr.s_addr = inet_addr( address.c_str() );
  }

  server.sin_family = AF_INET;
  server.sin_port = htons( port );

  return 0;
}

int IoProxy::connectSocket() {
  sock = socket(AF_INET , SOCK_STREAM , 0);
  if (sock == -1) {
    logArray->logError("IoProxy", "could not create socket");
  }

  //Connect to remote server
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
    logArray->logError("IoProxy", "connect failed. Error");
    return 1;
  }

  return 0;

}

int IoProxy::disconnectSocket() {
  close(sock);
  return 0;

  if (sock > 0) {
    return 0;
  }
  return 1;
}
