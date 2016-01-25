#ifndef HOMEIO_MAIN
#define	HOMEIO_MAIN

#include "homeio.hpp"

HomeIO *h;
void handleSignal(int);

void handleSignal(int s) {
  UNUSED(s);
  h->stop();
  exit(1);
}

#endif
