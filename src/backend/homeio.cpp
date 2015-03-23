HomeIO::HomeIO() {
  measTypeArray = new MeasTypeArray;
  measFetcher = new MeasFetcher;
  ioProxy = new IoProxy;
  tcpServer = new TcpServer;
  tcpCommand = new TcpCommand;
  ioServer = new IoServer;
  actionTypeArray = new ActionTypeArray;
  overseerArray = new OverseerArray;
  
  ioServerReady = false;
  
  // setup some variables
  measFetcher->measTypeArray = measTypeArray;
  measFetcher->ioProxy = ioProxy;
  
  tcpCommand->measTypeArray = measTypeArray;
  tcpCommand->actionTypeArray = actionTypeArray;
  tcpCommand->overseerArray = overseerArray;
  tcpServer->tcpCommand = tcpCommand;
}

unsigned char HomeIO::startFetch() {
  // set IoProxy to measurements
  for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
    m->ioProxy = ioProxy;
  }
  
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

unsigned char HomeIO::startOverseer() {
  // set IoProxy to actions
  for(std::vector<ActionType>::iterator a = actionTypeArray->actionTypes.begin(); a != actionTypeArray->actionTypes.end(); ++a) {
    a->ioProxy = ioProxy;
  }

  
  // access is needed to search for proper objects
  overseerArray->measTypeArray = measTypeArray;
  overseerArray->actionTypeArray = actionTypeArray;
  overseerArray->start();
  
  return 0;
}

void *measStartThread(void *argument)
{
  logInfo("Thread: startFetch() - meas fetching");

  HomeIO *h = (HomeIO *) argument;
  h->startFetch();
  
  return NULL;
}

void *tcpServerThread(void *argument)
{
  logInfo("Thread: startServer() - TCP commands");

  HomeIO *h = (HomeIO *) argument;
  h->startServer();
}

void *ioServerThread(void *argument)
{
  logInfo("Thread: startIoServer() - IoServer - hardware-TCP bridge");

  HomeIO *h = (HomeIO *) argument;
  h->startIoServer();
}

void *ioOverseerThread(void *argument)
{
  logInfo("Thread: ioOverseerThread() - low level overseeers");

  HomeIO *h = (HomeIO *) argument;
  h->startOverseer();
}


unsigned char HomeIO::start() {
  const char NUM_THREADS = 3;
  pthread_t threads[NUM_THREADS];
  int rc, i;
  
  HomeIO *h = this;
 
  rc = pthread_create(&threads[0], NULL, ioServerThread,  (void *) h);
  
  // wait for IoServer
  
  while (ioServer->ready == false) {
    usleep(50000);
  }
  
  rc = pthread_create(&threads[1], NULL, measStartThread, (void *) h);
  rc = pthread_create(&threads[2], NULL, tcpServerThread, (void *) h);
  rc = pthread_create(&threads[3], NULL, ioOverseerThread, (void *) h);
 
   // wait for each thread to complete
   for (i=0; i<NUM_THREADS; ++i) {
      // block until thread i completes
      rc = pthread_join(threads[i], NULL);
      logError("In main: thread " + to_string(i) + " is complete");
   }
 
   logError("In main: All threads completed successfully");
   exit(EXIT_SUCCESS);
}
