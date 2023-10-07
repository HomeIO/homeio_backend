#include "../backend/main.hpp"

unsigned char buffer[10];
unsigned char count_response = 0;
std::unique_ptr<IoRS> rs;

unsigned int responseSyncZero = 0;
unsigned int responseCurrent = 0;
unsigned int responseSyncOnes = 0;

unsigned int sendViaRs(unsigned char responseSize, unsigned char command) {
  unsigned int response_value = 0;

  rs->buffer[0] = 1; // command size
  rs->buffer[1] = responseSize; // response size
  rs->buffer[2] = command; // command
  rs->buffer[3] = 0;

  rs->send();

  // TODO make more universal
  if (responseSize == 1) {
    response_value = buffer[0];
  }
  else {
    response_value = buffer[0] * 256 + buffer[1];
  }

  return response_value;
}

int main() {
  rs = std::make_unique<IoRS>();
  rs->port = "/dev/ttyACM0";
  rs->buffer = buffer;

  while (true) {
    responseSyncZero = sendViaRs(2, 's');
    // responseCurrent = sendViaRs(2, 'a');
    // responseSyncOnes = sendViaRs(2, 'S');

    printf("0 = %d, 111 = %d, current = %d\n", responseSyncZero, responseSyncOnes, responseCurrent);
    usleep(1000000);
  }
  return 0;
}
