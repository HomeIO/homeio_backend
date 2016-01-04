#include "io_rs.hpp"

IoRS::IoRS() {
  // initialize RS232
  tio.c_iflag = 0;
  tio.c_oflag = 0;
  // set in config.h
  tio.c_cflag = RS_FLAGS;
  tio.c_lflag = 0;
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 5;
}

// Open RS port and set all parameters. Parameters are stored in config.h
int IoRS::openRS() {
    ttyFileDescriptor = open(port.c_str(), O_RDWR);

    cfsetospeed(&tio, RS_SPEED);
    cfsetispeed(&tio, RS_SPEED);

    tcsetattr(ttyFileDescriptor, TCSANOW, &tio);

    return ttyFileDescriptor;
}

unsigned char IoRS::send() {
  count_command = buffer[0];
  count_response = buffer[1];

  openRS();

  for (i=0; i<count_command; i++) {
    tmp_char = buffer[2 + i];
    write(ttyFileDescriptor, &tmp_char, 1);
  }

  // receive from uC
  unsigned long int tmp = 0;
  for (i=0; i<count_response; i++) {
    // next byte, *256 current value
    tmp *= 256;

    read(ttyFileDescriptor, &tmp_char, 1);
    buffer[i] = tmp_char;
    // sum for displaying result
    tmp += (unsigned long int) tmp_char;
  }
  buffer[count_response] = 0;

  closeRS();

  return count_response;
}

// Close RS port
void IoRS::closeRS() {
    close(ttyFileDescriptor);
}
