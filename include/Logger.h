#ifndef LOGGER_H__
#define LOGGER_H__
#include <boost/scoped_ptr.hpp>
#include <cstdarg>

namespace nccalog
{
  enum Colours{NORMAL,RED,GREEN ,YELLOW,BLUE,MAGENTA,CYAN,WHITE,RESET};

  class NCCALogger
  {
    public :

      void logMessage(const char* fmt, ...);
      void logError(const char* fmt, ...);
      void logWarning(const char* fmt, ...);
      void enableLogToFile();
      void disableLogToFile();
      void enableLogToConsole();
      void disableLogToConsole();
      void enableLogToFileAndConsole();
      void disableLogToFileAndConsole();
      void setLogFile(const std::string &_fname);
      void setColour(Colours _c);
      void enableLineNumbers();
      void disableLineNumbers();
      void enableTimeStamp();
      void disableTimeStamp();
      void disableColours();
      void enableColours();
      NCCALogger();
      ~NCCALogger();
    private :
      class Impl;
      boost::scoped_ptr<Impl> m_impl;


  };


} // end namespace

#endif
