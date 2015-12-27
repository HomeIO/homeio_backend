#include "homeio.hpp"

HomeIO::HomeIO() {
  //HomeIO::running = true;

  measTypeArray = new MeasTypeArray;
  measFetcher = new MeasFetcher;
  ioProxy = new IoProxy;
  tcpServer = new TcpServer;
  tcpCommand = new TcpCommand;
  ioServer = new IoServer;
  actionTypeArray = new ActionTypeArray;
  overseerArray = new OverseerArray;
  fileStorage = new FileStorage;
  measBufferBackupStorage = new MeasBufferBackupStorage;
  frontendSettings = new FrontendSettings;
  spy = new Spy;
  measGroup = new MeasGroup;
  addonsArray = new AddonsArray;

  ioServerReady = false;

  // setup some variables
  measFetcher->measTypeArray = measTypeArray;
  measFetcher->ioProxy = ioProxy;

  measGroup->measTypeArray = measTypeArray;

  tcpCommand->measTypeArray = measTypeArray;
  tcpCommand->measFetcher = measFetcher;
  tcpCommand->actionTypeArray = actionTypeArray;
  tcpCommand->overseerArray = overseerArray;
  tcpCommand->frontendSettings = frontendSettings;
  tcpCommand->measGroup = measGroup;

  tcpServer->tcpCommand = tcpCommand;
  tcpServer->measTypeArray = measTypeArray;

  fileStorage->measTypeArray = measTypeArray;
  measBufferBackupStorage->measTypeArray = measTypeArray;
  spy->measTypeArray = measTypeArray;
  addonsArray->measTypeArray = measTypeArray;
}

unsigned char HomeIO::startFetch() {
  // set IoProxy to measurements
  for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
    m->ioProxy = ioProxy;
  }

  // resize buffer to custom size
  for(std::vector<MeasType>::iterator m = measTypeArray->measTypes.begin(); m != measTypeArray->measTypes.end(); ++m) {
    m->resizeBuffer( measFetcher->maxBufferSize );
  }
  logInfo("MeasFetcher: resize buffer size to " + to_string(measFetcher->maxBufferSize) );

  // restore buffer before restart
  measBufferBackupStorage->performRestore();

  // start fetching measurements
  // to clear buffer or
  // restored buffer
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

unsigned char HomeIO::startFileStorage() {
  fileStorage->start();

  return 0;
}

unsigned char HomeIO::startBufferBackupStorage() {
  measBufferBackupStorage->start();

  return 0;
}

unsigned char HomeIO::startSpy() {
  spy->start();

  return 0;
}

unsigned char HomeIO::startAddons() {
  addonsArray->start();

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

  return NULL;
}

void *ioServerThread(void *argument)
{
  logInfo("Thread: startIoServer() - IoServer - hardware-TCP bridge");

  HomeIO *h = (HomeIO *) argument;
  h->startIoServer();

  return NULL;
}

void *ioOverseerThread(void *argument)
{
  logInfo("Thread: ioOverseerThread() - low level overseeers");

  HomeIO *h = (HomeIO *) argument;
  h->startOverseer();

  return NULL;
}

void *fileStorageThread(void *argument)
{
  logInfo("Thread: fileStorageThread() - store measurement in files");

  HomeIO *h = (HomeIO *) argument;
  h->startFileStorage();

  return NULL;
}

void *fileBufferBackupThread(void *argument)
{
  logInfo("Thread: fileBufferBackupThread() - store measurement buffer backup");

  HomeIO *h = (HomeIO *) argument;
  h->startBufferBackupStorage();

  return NULL;
}

void *spyThread(void *argument)
{
  logInfo("Thread: spyThread() - announce measurements to central server");

  HomeIO *h = (HomeIO *) argument;
  h->startSpy();

  return NULL;
}

void *addonsThread(void *argument)
{
  logInfo("Thread: addonsThread() - execute addon modules");

  HomeIO *h = (HomeIO *) argument;
  h->startAddons();

  return NULL;

}

void HomeIO::copyInternalDelays() {
  fileStorage->usDelay += measFetcher->cycleInterval * 4;
  measBufferBackupStorage->usDelay += measFetcher->cycleInterval * 6;
  overseerArray->usDelay += measFetcher->cycleInterval * 5;
  tcpServer->usDelay += measFetcher->cycleInterval * 2;
  spy->usDelay += measFetcher->cycleInterval * 3;
}

unsigned char HomeIO::start() {
  copyInternalDelays();

  const char NUM_THREADS = 7;
  pthread_t threads[NUM_THREADS];
  int i;

  HomeIO *h = this;

  pthread_create(&threads[0], NULL, ioServerThread,  (void *) h);

  // wait for IoServer

  while (ioServer->ready == false) {
    usleep(50000);
  }

  pthread_create(&threads[1], NULL, measStartThread, (void *) h);
  pthread_create(&threads[2], NULL, tcpServerThread, (void *) h);
  pthread_create(&threads[3], NULL, ioOverseerThread, (void *) h);
  pthread_create(&threads[4], NULL, fileStorageThread, (void *) h);
  pthread_create(&threads[5], NULL, fileBufferBackupThread, (void *) h);
  pthread_create(&threads[6], NULL, spyThread, (void *) h);
  pthread_create(&threads[7], NULL, addonsThread, (void *) h);

   // wait for each thread to complete
   for (i=0; i<NUM_THREADS; ++i) {
      // block until thread i completes
      pthread_join(threads[i], NULL);
      logError("In main: thread " + to_string(i) + " is complete");
   }

   logError("In main: All threads completed successfully");
   exit(EXIT_SUCCESS);
}

unsigned char HomeIO::stop() {
  cout << endl << endl;
  logInfo("Shutdown initialized");

  measBufferBackupStorage->stop();
  fileStorage->stop();
  tcpServer->stop();
  spy->stop();
  overseerArray->stop();
  measFetcher->stop();
  ioServer->stop();
  addonsArray->stop();

  logInfo("Shutdown completed");
  return 0;
}
