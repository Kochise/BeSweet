/**
/*   Copyright (c) 2003by  Marco Welti
/*
/*   This document is  bound by the  QT Public License
/*   (http://www.trolltech.com/licenses/qpl.html).
/*   See License.txt for more information.
/*
/*
/*
/*   ALL RIGHTS RESERVED.  
/* 
/*   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
/*   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/*   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/*   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/*   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/*   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
/*   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
/*   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/*   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/* 
/***********************************************************************/

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#ifndef __GNUC__

#if !defined(AFX_STDAFX_H__B7D8D92B_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_)
#define AFX_STDAFX_H__B7D8D92B_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_

#define WM_SYSTRAY_NOTIFY   WM_APP+100
#define WM_SHOW_FILEOUTLINE WM_APP+101
#define WM_REPARSE_FILE     WM_APP+102
#define WM_OPEN_WORKSPACE   WM_APP+103
#define WM_CLOSE_WORKSPACE  WM_APP+104
#define WM_SHOW_BROWSER     WM_APP+105

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
void message_box(const std::string&);

#pragma warning( disable: 4786 )
#pragma warning( disable: 4284 )

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>


#define _ATL_APARTMENT_THREADED
#include <atlbase.h>

#include <map>
#include <string>
#include <sstream>
#include <typeinfo>

inline std::ostream& operator<<(std::ostream& os, const std::exception &e)
{
  return os << typeid(e).name() << std::endl << e.what();
}
//labels
extern const std::string GENERAL;

//areas
extern const std::string MODEL;
extern const std::string CTAGS;
extern const std::string CONTROLLER;
extern const std::string RENDERER;
extern const std::string REMOTE_SRV;
extern const std::string REMOTE_MGR;
extern const std::string REMOTE_EDITOR;
extern const std::string NO_LABEL;

//enum LogLevel { eError = 0, eWarning = 10, eInfo = 20, eDebug = 30, eTrace = 40 };
//
//struct LoggerConsumer
//{
//  virtual ~LoggerConsumer() {}
//  virtual void logEntry(long time, int threadId, const std::string& label, const std::string &area, LogLevel level, const std::string message) = 0;
//};
//
//extern LoggerConsumer* _logger;
//extern std::map<std::string, int> area_levels;
//extern const std::string levels[];




#include <Utilities/smart_ptr.h>


//////////////////////////////////////////////////////////////////////////////////
namespace Logging {
  
  class LoggingException : std::runtime_error
  {
  public :
    LoggingException(const std::string &what) 
      : runtime_error(what) { };  
  };
  
  class Rejected : public LoggingException
  {
  public:
    Rejected(const std::string &what) : LoggingException(what) {};
  };
  
  enum LogLevel { eError = 0, eWarning = 10, eInfo = 20, eDebug = 30, eTrace = 40 };
  
  class LoggerConsumer
  {
  public:
    virtual ~LoggerConsumer() {};
    virtual	void logEntry(unsigned long timeStamp, long tid, short level, const std::string &label, const std::string &area, const std::string &text) throw(LoggingException) = 0;	
    
    //virtual void  id(short) throw(Exception) = 0;
    //virtual short id() throw(Exception) = 0;  
  };
  
  class Logger
  {
  public:
    virtual ~Logger() {}
    
    virtual std::string name() const  = 0;
    
    virtual short logLevel() const throw (Logging::LoggingException) = 0;
    virtual void logLevel(short level) throw (Logging::Rejected) = 0;
    
    virtual void areaLevel(const std::string &area, short level) throw(Logging::Rejected) = 0;
    virtual short areaLevel(const std::string &area) const throw (Logging::LoggingException)  = 0;
    
    virtual void registerConsumer(const smart_ptr<LoggerConsumer> &) throw (Logging::Rejected) = 0;	
    virtual void deregisterConsumer(const smart_ptr<LoggerConsumer> &) throw (Logging::Rejected) = 0;   
  };
  
} //namespace Logging



///////////////////////////////////// logger.h /////////////////////////////
#include <list>
#include <map>

namespace Logging {


class LoggerImpl : public Logger
{
private:
  typedef std::list<smart_ptr<LoggerConsumer> > ConsumerList;
  typedef std::map<std::string, LogLevel> LevelDictionary;

  std::string mName;
  ConsumerList mConsumers;
  LogLevel mDefaultLevel;
  LevelDictionary mAreaLevels;

public:
  LoggerImpl(const std::string &name) 
    : mName(name){ }

  virtual std::string name() const
  { return mName; }

  virtual short logLevel() const
  { return mDefaultLevel; }

  virtual void logLevel(short level)
  { mDefaultLevel = (LogLevel)level; }
  
  virtual void areaLevel(const std::string &area, short level)
  { mAreaLevels[area] = (LogLevel)level; }
  
  virtual short areaLevel(const std::string &area) const
  { 
    LevelDictionary::const_iterator level = mAreaLevels.find(area);
    return level != mAreaLevels.end() ? level->second : mDefaultLevel;
  }
 
  virtual void registerConsumer(const smart_ptr<LoggerConsumer> &consumer)
  { mConsumers.push_back(consumer); }
  
  virtual void deregisterConsumer(const smart_ptr<LoggerConsumer> &consumer)
  { mConsumers.remove(consumer); }

  virtual	void logEntry(LogLevel level, const std::string &label, const std::string &area, const std::string &message)
  {
    for(ConsumerList::iterator consumer = mConsumers.begin(); consumer != mConsumers.end(); ++consumer) {
      (*consumer)->logEntry(time(NULL), GetCurrentThreadId(), level, label, area, message);
    }
  }

};

}

///////////////////////////////////// loggermanager.h /////////////////////////////
#include <fstream>
namespace Logging {
  
  class LoggerManager
  {
  private:
    typedef std::map<std::string, smart_ptr<LoggerImpl> > LoggerDictionary;
    
    short mDefaultLogLevel;
    LoggerDictionary  mLoggers;
    smart_ptr<LoggerImpl> mInternalLogger;
    std::ofstream mFile;

  public:		
    void map(const std::string &newLabel , const std::string &existinglabel);
    void map(const std::string &label, const smart_ptr<LoggerImpl> &logger);
    
    void unmap(const std::string &label);
    void unmap(const smart_ptr<LoggerImpl>& logger);
    
    const smart_ptr<LoggerImpl>& logger(const std::string &label) const
    { 
      LoggerDictionary::const_iterator logger = mLoggers.find(label);
      return  logger != mLoggers.end() ? logger->second : mInternalLogger; 
    };
    
    short defaultLogLevel() const
    { return mDefaultLogLevel; };
    
    void defaultLogLevel(const short level)
    { mDefaultLogLevel = level; };
    
    static LoggerManager& instance();
    
  protected:
    LoggerManager();
  };
  
  
} //namespace Logging


#define log(_label_, _area_, _level_) \
{ \
  Logging::LoggerImpl *_logger_ = Logging::LoggerManager::instance().logger(_label_).get(); \
  bool createLogEntry = _level_ <= _logger_->areaLevel(_area_); \
  if(createLogEntry) {\
  std::string _log_label = _label_; \
  std::string _log_area = _area_; \
  Logging::LogLevel _logLevel = _level_; \
  std::stringstream _log_stream; \
_log_stream



#define endlog \
  std::endl; \
  if(createLogEntry) { \
  _logger_->logEntry(_logLevel, _log_label, _log_area, _log_stream.str()); \
  } }\
}


class Tracer
{
private:
  std::string mLabel;
  std::string mArea;
  std::string mWhat;
  bool mDoLog;
  
public:
  Tracer(const std::string &label, const std::string &area, const std::string &what) 
    : mDoLog(false)
  { 
    Logging::LoggerImpl *logger = Logging::LoggerManager::instance().logger(label).get(); \
    if(logger->areaLevel(area) >= Logging::eTrace) {
      mDoLog = true;
      mLabel = label;
      mArea = area;
      mWhat = what;
      logger->logEntry(Logging::eTrace, mLabel, mArea, "-->> " + mWhat); 
    }
  }
  
  ~Tracer()
  { if(mDoLog) log(mLabel, mArea, Logging::eTrace) << "<<-- " << mWhat << endlog; }
  
};

///////////////////////////////////// loggermanager.h /////////////////////////////
namespace Logging
{
  class StreamConsumer : public LoggerConsumer
  {
  private:
    std::ostream &mStream;

  public:
    StreamConsumer(std::ostream &s) : mStream(s) {}
    void logEntry(unsigned long timeStamp, long tid, short level, const std::string &label, const std::string &area, const std::string &message)
    {
      mStream << "[" << timeStamp << "]\t[" << tid << "]\t["<< label << "]\t[" << area << "]\t[" << level << "]" << message << std::endl;
    }
  };
}

using namespace Logging;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B7D8D92B_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_)

#endif //__GNUC__
