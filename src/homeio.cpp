void *measStartThread(void *argument)
{
  cout << "Meas Start" << endl;

  HomeIO *h = (HomeIO *) argument;
  h->ioProxy->prepareSocket();
  
  // initials
  for(std::vector<MeasType>::iterator m = h->measTypes.begin(); m != h->measTypes.end(); ++m) {
    m->ioProxy = h->ioProxy;
  } 
  
  for(std::vector<MeasType>::iterator m = h->measTypes.begin(); m != h->measTypes.end(); ++m) {
    m->fetch();
  } 
  
  return NULL;
}

HomeIO::HomeIO() {
  ioProxy = new IoProxy();
}

int HomeIO::start() {
  const char NUM_THREADS = 1;
  pthread_t threads[NUM_THREADS];
  int rc, i;
  
  HomeIO *h = this;
 
   // create all threads one by one
   for (i=0; i<NUM_THREADS; ++i) {
      printf("In main: creating thread %d\n", i);
      rc = pthread_create(&threads[i], NULL, measStartThread, (void *) h);
      //assert(0 == rc);
   }
 
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
