#include <iostream>
#include <cstdlib>
#include "Logger.h"


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
}
