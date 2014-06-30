#ifndef RS_HPP
#define	RS_HPP

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


#include <termios.h>
#include "config.hpp"

class RS {
public:
  RS();
  
  // Open RS port and set all parameters
  // Parameters are stored in config.h
  int openRS();
  
  // Close RS port
  void closeRS();
  
  struct termios tio;
  int ttyFileDescriptor;
  char* portDevice;
};

#include "rs.cpp"
#endif
