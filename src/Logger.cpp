#include "Logger.h"
#include <iostream>
#include <ostream>
#include <cstdarg>
#include <vector>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/tee.hpp>

#include <fstream>
#include <iostream>

namespace nccalog
{
  // PIMPL Idiom to make lib cleaner
  class NCCALogger::Impl
  {
    typedef boost::iostreams::tee_device<std::ostream, std::ofstream > Tee;
    typedef boost::iostreams::stream<Tee> TeeStream;

  public:
    bool m_logFileAndConsole;
    bool m_logFile;
    bool m_logConsole;
    bool m_timeStamp;
    bool m_lineNumber;
    bool m_disableColours;
    unsigned int m_lineNumberCount;
    Colours m_colour;
    std::string m_logfileName;
    TeeStream m_output;
    std::ofstream m_file;

    Impl();
    void write(const std::string &_text);
    std::string currentTime();
    void setColour(enum Colours c);


  };





  NCCALogger::Impl::Impl(): m_logFileAndConsole(false),
                            m_logFile(true),
                            m_logConsole(false),
                            m_timeStamp(true),
                            m_lineNumber(true),
                            m_disableColours(false),
                            m_lineNumberCount(0),
                            m_colour(RESET),
                            m_logfileName("output.log")
  {

    m_file.open(m_logfileName.c_str() );
     if(!m_file.is_open())
     {
       std::cerr<<"error opening log file for writing\n";
       exit(EXIT_FAILURE);
     }
       Tee tee( std::cout, m_file );

       m_output.open(tee);
       m_output.set_auto_close(true);

    if( !m_output.is_open())
    {
      std::cerr<<"problem opening log stream tee\n";
      exit(EXIT_FAILURE);
    }


  }
  std::string NCCALogger::Impl::currentTime()
  {
  std::string timeStr;
  time_t rawTime;
  time( & rawTime );
  timeStr = ctime( &rawTime );
  //without the newline character
  return timeStr.substr( 0 , timeStr.size() - 1 );
  }

  void NCCALogger::Impl::write(const std::string &_text)
  {
    setColour(m_colour);
    if(m_lineNumber == true)
    {
      m_output<< ++m_lineNumberCount<<" ";
    }
    if(m_timeStamp==true)
    {
      m_output<<currentTime()<<" ";
    }
    m_output<<_text;

  }

  // from http://stackoverflow.com/questions/3585846/color-text-in-terminal-aplications-in-unix
  void NCCALogger::Impl::setColour(enum Colours c)
  {
    if(m_disableColours) return;

    switch(c)
    {
      case NORMAL : m_output<<"\x1B[0m"; break;
      case RED : m_output<<"\x1B[31m"; break;
      case GREEN : m_output<<"\x1B[32m"; break;
      case YELLOW : m_output<<"\x1B[33m"; break;
      case BLUE : m_output<<"\x1B[34m"; break;
      case MAGENTA : m_output<<"\x1B[35m"; break;
      case CYAN : m_output<<"\x1B[36m"; break;
      case WHITE : m_output<<"\x1B[37m"; break;
      case RESET : m_output<<"\033[0m"; break;
      default : m_output<<"\033[0m"; break;
    }



  }



  NCCALogger::NCCALogger() : m_impl(new NCCALogger::Impl())
  {
    m_impl->setColour(BLUE);
    m_impl->m_output<<"NCCALogger started "<<m_impl->currentTime()<<"\n";
    m_impl->setColour(RESET);
  }


  NCCALogger::~NCCALogger()
  {
    m_impl->setColour(RESET);
    m_impl->m_output<<"\n";
    m_impl->m_output.flush();
    m_impl->m_output.close();
  }




  void NCCALogger::logMessage(const char *fmt,...)
  {
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    std::string text=buffer;
    va_end (args);
    m_impl->write(text);
    fflush(stdout);
  }

  void NCCALogger::logError(const char* fmt,...)
  {
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    m_impl->setColour(RED);
    m_impl->m_output<<"[ERROR] ";
    std::string text=buffer;
    va_end (args);
    m_impl->write(text);
    fflush(stdout);

  }

  void NCCALogger::logWarning(const char* fmt...)
  {
    char buffer[1024];
    va_list args;
    va_start (args, fmt);
    vsprintf (buffer,fmt, args);
    m_impl->setColour(GREEN);
    m_impl->m_output<<"[Warning] ";
    std::string text=buffer;
    va_end (args);
    m_impl->write(text);
    fflush(stdout);

  }


  void NCCALogger::enableLogToFile()
  {
    m_impl->m_logFile=true;

  }
  void NCCALogger::disableLogToFile()
  {
    m_impl->m_logFile=false;

  }
  void NCCALogger::enableLogToConsole()
  {
    m_impl->m_logConsole=true;

  }
  void NCCALogger::disableLogToConsole()
  {
    m_impl->m_logConsole=false;

  }
  void NCCALogger::enableLogToFileAndConsole()
  {
    m_impl->m_logConsole=true;
    m_impl->m_logFile=true;
  }
  void NCCALogger::disableLogToFileAndConsole()
  {
    m_impl->m_logConsole=false;
    m_impl->m_logFile=false;

  }
  void NCCALogger::setLogFile(const std::string &_fname)
  {

  }
  void NCCALogger::setColour(Colours _c)
  {
    m_impl->m_colour=_c;
  }
  void NCCALogger::enableLineNumbers()
  {
    m_impl->m_lineNumber=true;
  }
  void NCCALogger::disableLineNumbers()
  {
    m_impl->m_lineNumber=false;

  }
  void NCCALogger::enableTimeStamp()
  {
    m_impl->m_timeStamp=true;
  }
  void NCCALogger::disableTimeStamp()
  {
    m_impl->m_timeStamp=false;
  }

  void NCCALogger::disableColours()
  {
    m_impl->m_disableColours=true;
  }

  void NCCALogger::enableColours()
  {
    m_impl->m_disableColours=false;
  }

}
