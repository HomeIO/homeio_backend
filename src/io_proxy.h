#ifndef IO_PROXY
#define	IO_PROXY

#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent

using namespace std;

class IoProxy {
public:
  IoProxy();

  bool conn(string, int);
  bool send_data(string data);
  string receive(int);
  void getRawMeas();
  
private:
  int sock;
  std::string address;
  int port;
  struct sockaddr_in server;
     
};

#endif
