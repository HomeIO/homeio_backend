IoServer::IoServer() {
  rs = new RS;
  tcp = new IoTcpServer;
  
  tmp_char = 0;
  i = 0;
  count_command = 0;
  port = "/dev/ttyUSB0";
  rs->port = port;
  
  tcp->port = IO_TCP_SERVER_PORT;
}

int IoServer::start() {
  rs->openRS();
  tcp->createTcpServer();
  
  while(1) {
    // Retrieve command
    tcp->waitForCommand();
    tcp->readTcp();
   
    tcp->buffer[0] = 80;
    tcp->buffer[1] = 80;
    tcp->buffer[2] = 0;
    tcp->count_response = 2;
   
    tcp->writeTcp();
    tcp->closeSocket();
  }
  
  /*
    // infinite server loop
    while (1) {
        // Wait for a connection, then accept() it
        

        

	

        // command and response char count
        count_command = buffer[0];
        count_response = buffer[1];
        
        // send to uC
        for (i=0; i<count_command; i++) {
            tmp_char = buffer[2 + i];
            write(tty_fd, &tmp_char, 1);
        }
        // receive from uC
        unsigned long int tmp = 0;
        for (i=0; i<count_response; i++) {
            // next byte, *256 current value
            tmp *= 256;

            read(tty_fd, &tmp_char, 1);
            buffer[i] = tmp_char;
            // sum for displaying result
            tmp += (unsigned long int) tmp_char;
        }
        buffer[count_response] = 0;
        printf("res raw %d\n", tmp);

          // send uC reply via socket
        // count_response + 1 to add \0
        writeLine(conn_s, buffer, count_response);

        // Close the connected socket
        if (close(conn_s) < 0) {
            fprintf(stderr, "ECHOSERV: Error calling close()\n");
            exit(EXIT_FAILURE);
        }
    }
*/
  
  return 0;
}