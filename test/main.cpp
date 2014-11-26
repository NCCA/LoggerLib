#include <iostream>
#include <cstdlib>
#include "Logger.h"
#include <pthread.h>

void doThreadTest();

int main()
{
  nccalog::NCCALogger log;
  nccalog::NCCALogger log2("log2.log");
  log.cout()<<"testing extraction\n"<<"Should work with all types "<<1<<1.5<<'c'<<std::endl;
  //log.enableLogToFileAndConsole();
  //log.disableLineNumbers();
  //log.disableTimeStamp();
  //log.disableColours();
  log.logMessage("Simple String\n");
  log.setColour(nccalog::CYAN);
  log.setTimeFormat(nccalog::TIMEDATEDAY);
  log.logMessage("printf style %d %c %f %f\n", 3, 'a', 1.999, 42.5);
  log.logError(" this is an error\n");
  log.logWarning(" this is a warning\n");

  log2.setColour(nccalog::MAGENTA);
  log2.setLogFile("Log2New.log");
  log2.logMessage("this is log 2 in a new file\n");
  log2.setLineNumberPad(2);
  log2.setTimeFormat(nccalog::TIMEDATE);

  for(int i=0; i<20; ++i)
    log2.logError("loop %d\n",i);

  //doThreadTest();
}
nccalog::NCCALogger glog("thread.log");


struct argStruct
{
	int arg1;
	char arg2;
};


void *threadFunc(void *arg)
{
	struct argStruct *args = (argStruct *)arg;
/*	std::cout<<"thread func "<<std::endl;
	std::cout<<"Arg 1 "<<args->arg1<<std::endl;
	std::cout<<"Arg 2 "<<args->arg2<<std::endl;
	*/
	glog.logMessage("thread function %d %c \n",args->arg1,args->arg2);
	int ret=args->arg1*2;
}


void doThreadTest()
{
	//glog.disableLineNumbers();
	//glog.disableTimeStamp();
	pthread_t threadID[4];
	struct argStruct args[4];

	for(int i=0; i<4; ++i)
	{
		args[i].arg1=i;
		args[i].arg2='a'+i;
		pthread_create(&threadID[i],0,threadFunc,(void *)&args[i]);
	}
	// now join

	int retval;
	for(int i=0; i<4; ++i)
	{
		pthread_join(threadID[i],0);
	}
}
