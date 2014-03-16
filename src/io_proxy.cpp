#include "io_proxy.h"

// http://www.binarytides.com/code-a-simple-socket-client-class-in-c/
IoProxy::IoProxy() 
{
  sock = -1;
  port = 0;
  address = "";
}

unsigned int IoProxy::fetch(char commandChar, char responseSize)
{
  connectSocket();
  
  char commandArray[4];
  commandArray[0] = 1; // size of command
  commandArray[1] = responseSize; // size of response
  commandArray[2] = commandChar; // command
  commandArray[3] = '\n'; // end, it's needed

  if( send(sock, commandArray, sizeof(commandArray), 0) < 0)
  {
    perror("Send failed : ");
    return 1;
  }
  cout << "Data send " << commandArray << " " << sizeof(commandArray) << endl;
     
  //Receive a reply from the server
  if( recv(sock, buffer, responseSize, 0) < 0)
  {
    puts("recv failed");
  }
  
  unsigned int raw = 0;
  int i = 0;
  for (i; i < responseSize; i++)
  {
    raw *= 256;
    raw += buffer[i];
    cout << buffer[i] << endl;
  }
  
  cout << "Data received" << buffer << endl;
  
  return raw;
}


int IoProxy::connectSocket()
{
  //create socket if it is not already created
  if(sock == -1)
  {
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
      perror("Could not create socket");
    }
         
    cout << "Socket created\n";
  }
  else
  {
    /* OK , nothing */
  }
     
  //setup address structure
  if(inet_addr(address.c_str()) == -1)
  {
    struct hostent *he;
    struct in_addr **addr_list;
         
    //resolve the hostname, its not an ip address
    if ( (he = gethostbyname( address.c_str() ) ) == NULL)
    {
      //gethostbyname failed
      herror("gethostbyname");
      cout<<"Failed to resolve hostname\n";
             
      return false;
    }
         
    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **) he->h_addr_list;
 
    for(int i = 0; addr_list[i] != NULL; i++)
    {
      //strcpy(ip , inet_ntoa(*addr_list[i]) );
      server.sin_addr = *addr_list[i];
             
      cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;
             
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
     
  //Connect to remote server
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
  {
    perror("connect failed. Error");
    return 1;
  }
     
  cout<<"Connected\n";
  return 0;

}

  