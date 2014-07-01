RS::RS() {
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
int RS::openRS() {
    ttyFileDescriptor = open(port.c_str(), O_RDWR);

    cfsetospeed(&tio, RS_SPEED);
    cfsetispeed(&tio, RS_SPEED);

    tcsetattr(ttyFileDescriptor, TCSANOW, &tio);

    return ttyFileDescriptor;
}

// Close RS port
void RS::closeRS() {
    close(ttyFileDescriptor);
}

