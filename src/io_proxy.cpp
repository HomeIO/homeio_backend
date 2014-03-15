#include "io_proxy.h"

// http://www.binarytides.com/code-a-simple-socket-client-class-in-c/
IoProxy::IoProxy() 
{
  sock = -1;
  port = 0;
  address = "";
}

void IoProxy::test()
{
  conn("192.168.0.2", 2002);
  
  char command[3];
  command[0] = 1; // size of command
  command[1] = 2; // size of response
  command[2] = '0'; // command
  command[3] = '\n'; // end

  command[0] = 1; // size of command
  command[1] = 1; // size of response
  command[2] = 's'; // command

  
  //send_data(command);
  //receive(2);
  
  //Send some data
    if( send(sock , command , 4 , 0) < 0)
    {
        perror("Send failed : ");
        return;
    }
    cout<<"Data send " << command << " " << sizeof(command) << endl;
     
    char buffer[10];
    string reply;
     
    //Receive a reply from the server
    if( recv(sock , buffer , 10 , 0) < 0)
    {
        puts("recv failed");
    }
     
    reply = buffer;
    cout<<"Data received" << reply << endl;
  
}

/**
    Connect to a host on a certain port number
*/
bool IoProxy::conn(string address , int port)
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
         
        cout<<"Socket created\n";
    }
    else    {   /* OK , nothing */  }
     
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
    return true;
}
 
/**
    Send data to the connected host
*/
bool IoProxy::send_data(string data)
{
    //Send some data
    if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    cout<<"Data send\n";
     
    return true;
}
 
/**
    Receive data from the connected host
*/
string IoProxy::receive(int size=512)
{
    char buffer[size];
    string reply;
     
    //Receive a reply from the server
    if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    {
        puts("recv failed");
    }
     
    reply = buffer;
    return reply;
}

void IoProxy::getRawMeas() {
}

int mainb(int argc , char *argv[])
{
    IoProxy c;
    string host;
     
    cout<<"Enter hostname : ";
    cin>>host;
     
    //connect to host
    c.conn(host , 80);
     
    //send some data
    c.send_data("GET / HTTP/1.1\r\n\r\n");
     
    //receive and echo reply
    cout<<"----------------------------\n\n";
    cout<<c.receive(1024);
    cout<<"\n\n----------------------------\n\n";
     
    //done
    return 0;
}
  