void *measStartThread(void *argument)
{
  cout << "Meas Start" << endl;

  HomeIO *h = (HomeIO *) argument;
  h->ioProxy->prepareSocket();
  
  // initials
  for(std::vector<MeasType>::iterator m = h->measTypes.begin(); m != h->measTypes.end(); ++m) {
    m->ioProxy = h->ioProxy;
  } 
  
  unsigned int i;
  for (i=0; i<30; i++) {
    for(std::vector<MeasType>::iterator m = h->measTypes.begin(); m != h->measTypes.end(); ++m) {
      m->fetch();
      usleep(50000);
    } 
  }
  
  return NULL;
}

void *tcpServerThread(void *argument)
{
  cout << "TCP Start" << endl;

  HomeIO *h = (HomeIO *) argument;
  h->tcpServer->start();
  
  /*
  unsigned int i;
  for (i=0; i<10; i++) {
    cout << i << endl;
    usleep(100000);
  }
  */
  
}

HomeIO::HomeIO() {
  ioProxy = new IoProxy();
  tcpServer = new TcpServer();
}

int HomeIO::start() {
  const char NUM_THREADS = 2;
  pthread_t threads[NUM_THREADS];
  int rc, i;
  
  HomeIO *h = this;
 
  rc = pthread_create(&threads[0], NULL, measStartThread, (void *) h);
  rc = pthread_create(&threads[1], NULL, tcpServerThread, (void *) h);
 
   // wait for each thread to complete
   for (i=0; i<NUM_THREADS; ++i) {
      // block until thread i completes
      rc = pthread_join(threads[i], NULL);
      printf("In main: thread %d is complete\n", i);
      //assert(0 == rc);
   }
 
   printf("In main: All threads completed successfully\n");
   //exit(EXIT_SUCCESS);
}
