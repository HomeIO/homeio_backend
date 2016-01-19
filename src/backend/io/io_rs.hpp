#ifndef IO_RS_HPP
#define	IO_RS_HPP

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>

#include <termios.h>
#include "io_config.hpp"

class IoRS {
 public:
  IoRS();

  // Open RS port and set all parameters
  // Parameters are stored in config.h
  int openRS();

  unsigned char send();

  void closeRS();

  struct termios tio;
  int ttyFileDescriptor;

  std::string port;
  unsigned char i, tmp_char;

  unsigned char *buffer;
  unsigned char count_command;
  unsigned char count_response;
};

//#include "rs.cpp"
#endif
