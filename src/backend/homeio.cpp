HomeIO::HomeIO() {
  measTypeArray = new MeasTypeArray;
  measFetcher = new MeasFetcher;
  ioProxy = new IoProxy;
  tcpServer = new TcpServer;
  tcpCommand = new TcpCommand;
  
  // setup some variables
  measFetcher->measTypeArray = measTypeArray;
  measFetcher->ioProxy = ioProxy;
  
  tcpCommand->measTypeArray = measTypeArray;
  tcpServer->tcpCommand = tcpCommand;
}

unsigned char HomeIO::startFetch() {
  measFetcher->start();
  
  return 0;
}

unsigned char HomeIO::startServer() {
  tcpServer->start();
  
  return 0;
}

unsigned char HomeIO::startIoServer() {
  ioServer->start();
  
  return 0;
}


void *measStartThread(void *argument)
{
  cout << "Meas Start" << endl;

  HomeIO *h = (HomeIO *) argument;
  h->startFetch();
  
  return NULL;
}

void *tcpServerThread(void *argument)
{
  cout << "TCP Start" << endl;

  HomeIO *h = (HomeIO *) argument;
  h->startServer();
}

void *ioServerThread(void *argument)
{
  cout << "IoServer Start" << endl;

  HomeIO *h = (HomeIO *) argument;
  h->startIoServer();
}

unsigned char HomeIO::start() {
  const char NUM_THREADS = 3;
  pthread_t threads[NUM_THREADS];
  int rc, i;
  
  HomeIO *h = this;
 
  rc = pthread_create(&threads[0], NULL, ioServerThread,  (void *) h);
  rc = pthread_create(&threads[1], NULL, measStartThread, (void *) h);
  rc = pthread_create(&threads[2], NULL, tcpServerThread, (void *) h);
 
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
