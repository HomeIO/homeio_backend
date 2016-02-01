#include "homeio.hpp"

HomeIO::HomeIO() {
  prepareDirectories();

  boot = std::make_shared<Boot>();
  spy = std::make_shared<Spy>();
  measFetcher = std::make_shared<MeasFetcher>();
  tcpServer = std::make_shared<TcpServer>();
  tcpCommand = std::make_shared<TcpCommand>();
  measBufferBackupStorage = std::make_shared<MeasBufferBackupStorage>();
  measTypeArray = std::make_shared<MeasTypeArray>();
  ioProxy = std::make_shared<IoProxy>();
  ioServer = std::make_shared<IoServer>();

  logArray = new LogArray;
  actionTypeArray = new ActionTypeArray;
  overseerArray = new OverseerArray;
  fileStorage = new FileStorage;
  frontendSettings = new FrontendSettings;
  measGroup = new MeasGroup;
  addonsArray = new AddonsArray;
  ncursesUI = new NcursesUI;

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

  ncursesUI->meas->measTypeArray = measTypeArray;
  ncursesUI->action->actionTypeArray = actionTypeArray;
  ncursesUI->overseer->overseerArray = overseerArray;
  ncursesUI->addon->addonsArray = addonsArray;
  ncursesUI->log->logArray = logArray;

  ncursesUI->home->ioServer = ioServer;
  ncursesUI->home->measFetcher = measFetcher;
  ncursesUI->home->overseerArray = overseerArray;
  ncursesUI->home->tcpServer = tcpServer;
  ncursesUI->home->fileStorage = fileStorage;
  ncursesUI->home->measBufferBackupStorage = measBufferBackupStorage;
  ncursesUI->home->addonsArray = addonsArray;
  ncursesUI->home->spy = spy;

  tcpServer->tcpCommand = tcpCommand;
  tcpServer->measTypeArray = measTypeArray;

  fileStorage->measTypeArray = measTypeArray;
  measBufferBackupStorage->measTypeArray = measTypeArray;
  spy->measTypeArray = measTypeArray;
  addonsArray->measTypeArray = measTypeArray;

  tcpServer->logArray = logArray;
  tcpCommand->logArray = logArray;
  spy->logArray = logArray;
  overseerArray->logArray = logArray;
  measTypeArray->logArray = logArray;
  fileStorage->logArray = logArray;
  actionTypeArray->logArray = logArray;
  addonsArray->logArray = logArray;
  measBufferBackupStorage->logArray = logArray;
  measFetcher->logArray = logArray;
  ioProxy->logArray = logArray;
  ioServer->logArray = logArray;
  ioServer->tcp->logArray = logArray;

}

HomeIO::~HomeIO() {
  // delete logArray;
  // delete measTypeArray;
  // delete ioProxy;
  // delete tcpCommand;
  // delete ioServer;
  // delete actionTypeArray;
  // delete overseerArray;
  // delete fileStorage;
  // delete measBufferBackupStorage;
  // delete frontendSettings;
  // delete measGroup;
  // delete addonsArray;
  // delete ncursesUI;
}

void HomeIO::prepareDirectories() {
  Helper::createDir("data");
  Helper::createDir("stats");
}

unsigned char HomeIO::startFetch() {
  // set IoProxy to measurements
  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::shared_ptr<MeasType> m = *it;
    m->ioProxy = ioProxy;
  }

  // resize buffer to custom size
  for(std::vector<std::shared_ptr<MeasType>>::iterator it = measTypeArray->measTypes.begin(); it != measTypeArray->measTypes.end(); ++it) {
    std::shared_ptr<MeasType> m = *it;
    m->resizeBuffer( measFetcher->maxBufferSize );
  }
  logArray->log("MeasFetcher", "resize buffer size to " + std::to_string(measFetcher->maxBufferSize) );

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

unsigned char HomeIO::startNcurses() {
  ncursesUI->start();

  return 0;
}

void *measStartThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: startFetch() - meas fetching");
  h->startFetch();

  return NULL;
}

void *tcpServerThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: startServer() - TCP commands");
  h->startServer();

  return NULL;
}

void *ioServerThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: startIoServer() - IoServer - hardware-TCP bridge");
  h->startIoServer();

  return NULL;
}

void *overseerThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: overseerThread() - low level overseeers");
  h->startOverseer();

  return NULL;
}

void *fileStorageThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: fileStorageThread() - store measurement in files");
  h->startFileStorage();

  return NULL;
}

void *fileBufferBackupThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: fileBufferBackupThread() - store measurement buffer backup");
  h->startBufferBackupStorage();

  return NULL;
}

void *spyThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: spyThread() - announce measurements to central server");
  h->startSpy();

  return NULL;
}

void *addonsThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: addonsThread() - execute addon modules");
  h->startAddons();

  return NULL;
}

void *ncursesThread(void *argument) {
  HomeIO *h = (HomeIO *) argument;
  h->logArray->log("HomeIO", "Thread: ncursesThread() - interface");
  h->startNcurses();

  return NULL;
}

void *shutdownWatchThread(void *argument) {
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

  const char NUM_THREADS = 8;
  pthread_t threads[NUM_THREADS];
  int i;

  HomeIO *h = this;

  // ncurses
  pthread_create(&threads[0], NULL, ncursesThread, (void *) h);
  while (ncursesUI->ready == false) {
    usleep(5000);
  }

  // io server
  pthread_create(&threads[0], NULL, ioServerThread,  (void *) h);
  while (ioServer->ready == false) {
    usleep(5000);
  }

  // meas
  pthread_create(&threads[2], NULL, measStartThread, (void *) h);
  while (measFetcher->ready == false) {
    usleep(5000);
  }

  // overseers
  pthread_create(&threads[3], NULL, overseerThread, (void *) h);
  while (overseerArray->ready == false) {
    usleep(5000);
  }

  // tcp server
  pthread_create(&threads[4], NULL, tcpServerThread, (void *) h);
  while (tcpServer->ready == false) {
    usleep(5000);
  }

  // file storage - meas time_from;time_to;value
  pthread_create(&threads[5], NULL, fileStorageThread, (void *) h);
  while (fileStorage->ready == false) {
    usleep(5000);
  }

  // meas buffer storage - raws
  pthread_create(&threads[6], NULL, fileBufferBackupThread, (void *) h);
  while (measBufferBackupStorage->ready == false) {
    usleep(5000);
  }

  // addons
  pthread_create(&threads[7], NULL, addonsThread, (void *) h);
  while (addonsArray->ready == false) {
    usleep(5000);
  }

  // spy
  pthread_create(&threads[8], NULL, spyThread, (void *) h);
  while (spy->ready == false) {
    usleep(5000);
  }

  while (h->boot->beginShutdown == false) {
    usleep(40000);
  }

  stop();

  return 0;
}

unsigned char HomeIO::stop() {
  // TODO check for doubled execution
  std::cout << std::endl << std::endl;
  logArray->log("HomeIO", "Shutdown initialized");

  spy->stop();
  addonsArray->stop();
  measBufferBackupStorage->stop();
  fileStorage->stop();
  tcpServer->stop();
  overseerArray->stop();
  measFetcher->stop();
  ioServer->stop();
  ncursesUI->stop();

  logArray->log("HomeIO", "Shutdown finished");
  logArray->consoleOutput();

  return 0;
}
