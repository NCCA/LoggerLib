#include <iostream>
#include <cstdlib>
#include "Logger.h"
#include <pthread.h>

void doThreadTest();

int main()
{
  nccalog::NCCALogger log;
  nccalog::NCCALogger log2("log2.log");

  //log.enableLogToFileAndConsole();
  //log.disableLineNumbers();
  //log.disableTimeStamp();
  //log.disableColours();
  log.logMessage("Simple String\n");
  log.setColour(nccalog::YELLOW);
  log.logMessage("printf style %d %c %f %f\n", 3, 'a', 1.999, 42.5);
  log.logError(" this is an error\n");
  log.logWarning(" this is a warning\n");
  log2.setColour(nccalog::MAGENTA);
  log2.logMessage("this is log 2\n");
  for(int i=0; i<20; ++i)
    log2.logError("loop %d\n",i);
  //doThreadTest();
}
nccalog::NCCALogger glog("thread.log");


char *sharedMem;
const static int SIZE=20;

void *starFillerThread(void *arg)
{
	while(1)
	{
	glog.logMessage("Star Filler\n");
	for(int i=0; i<SIZE; ++i)
		sharedMem[i]='*';
	sleep(1);
	}
}

void *hashFillerThread(void *arg)
{
	while(1)
	{
	glog.logMessage("hash filler\n");
	for(int i=0; i<SIZE; ++i)
		sharedMem[i]='#';
	sleep(1);
	}
}


void *consumerThread(void *arg)
{
		while(1)
		{
		glog.logMessage("Consumer\n");
		for(int i=0; i<SIZE; ++i)
			glog.logMessage("%c",sharedMem[i]);
		glog.logMessage("\n");
		sleep(1);
		}
}

void doThreadTest()
{
	glog.disableColours();
	glog.disableLineNumbers();
	glog.disableTimeStamp();
	sharedMem = new char[SIZE];
	pthread_t threadID[3];

	pthread_create(&threadID[0],0,starFillerThread,0);
	pthread_create(&threadID[1],0,hashFillerThread,0);
	pthread_create(&threadID[2],0,consumerThread,0);

	pthread_join(threadID[0],0);
	pthread_join(threadID[1],0);
	pthread_join(threadID[2],0);
	int i=0;

	while(i<200)
	{
		++i;
		glog.logWarning("main thread %d\n",i);
	}


}



