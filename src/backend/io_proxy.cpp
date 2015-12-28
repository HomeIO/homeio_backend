#include "io_proxy.hpp"

// http://www.binarytides.com/code-a-simple-socket-client-class-in-c/
IoProxy::IoProxy()
{
  port = 0;
  address = "";
  verbose = false;
}

unsigned int IoProxy::fetch(char commandChar, unsigned char responseSize)
{
  tcpMutex.lock();
  // after mutex

  connectSocket();

  char commandArray[4];
  commandArray[0] = 1; // size of command
  commandArray[1] = responseSize; // size of response
  commandArray[2] = commandChar; // command
  commandArray[3] = '\n'; // end, it's needed

  if( send(sock, commandArray, sizeof(commandArray), 0) < 0)
  {
    Helper::logError("IoProxy: Send failed");
    tcpMutex.unlock(); // end of mutex
    return 1;
  }

  //Receive a reply from the server
  if( recv(sock, buffer, responseSize, 0) < 0)
  {
    Helper::logError("IoProxy: recv failed");
  }

  disconnectSocket();
  tcpMutex.unlock(); // end of mutex

  unsigned int raw = 0;
  unsigned char part_raw = 0;

  for (unsigned char i = 0; i < responseSize; i++)
  {
    part_raw = buffer[i];
    raw *= 256;
    raw += part_raw;
  }

  if (verbose) {
    Helper::logInfo("IoProxy: Data received" + std::to_string(raw));
  }

  return raw;
}


unsigned int IoProxy::prepareSocket()
{
  // crazy way to get rid of warning
  // it checks if address is ip or name based, like localhost
  in_addr *tmp = new in_addr;
  bool isIp = (inet_aton(address.c_str(), tmp) > 0);

  //setup address structure
  if(isIp == false)
  {
    struct hostent *he;
    struct in_addr **addr_list;

    //resolve the hostname, its not an ip address
    if ( (he = gethostbyname( address.c_str() ) ) == NULL)
    {
      Helper::logError("IoProxy: Failed to resolve hostname " + address);

      return false;
    }

    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **) he->h_addr_list;

    for(int i = 0; addr_list[i] != NULL; i++)
    {
      //strcpy(ip , inet_ntoa(*addr_list[i]) );
      server.sin_addr = *addr_list[i];
      break;
    }
  }

  //plain ip address
  else
  {
    server.sin_addr.s_addr = inet_addr( address.c_str() );
  }

  server.sin_family = AF_INET;
  server.sin_port = htons( port );

  return 0;
}

int IoProxy::connectSocket()
{
  sock = socket(AF_INET , SOCK_STREAM , 0);
  if (sock == -1)
  {
    Helper::logError("IoProxy: could not create socket");
  }

  //Connect to remote server
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
  {
    Helper::logError("IoProxy: connect failed. Error");
    return 1;
  }

  return 0;

}

int IoProxy::disconnectSocket()
{
  close(sock);
  return 0;

  if (sock > 0)
  {
    return 0;
  }
  return 1;
}
