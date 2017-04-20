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
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__54F62A94_DA20_4119_AAD6_8246D664959B__INCLUDED_)
#define AFX_STDAFX_H__54F62A94_DA20_4119_AAD6_8246D664959B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>


#include <ObjectModelVC6/ObjectModelVC6.h>
#include <ObjectModelVC7/ObjectModelVC7.h>

#include <map>
#include <string>
#include <sstream>
#include <iomanip>

extern const std::string BE_SWEET;
extern const std::string ADDIN;
extern const std::string COMMAND;
extern const std::string EVENT;
enum { eError = 0, eWarning = 10, eInfo = 20, eDebug = 30, eTrace = 40 } LogLevel;

struct LoggerConsumer
{
  virtual ~LoggerConsumer() {}
  virtual void logEntry(const std::string &) = 0;
};

extern LoggerConsumer* _logger;
extern std::map<std::string, int> area_levels;
extern const std::string levels[];

inline logdump(const std::stringstream &ss)
{
  if(_logger) {
    _logger->logEntry(ss.str()); 
  }
}

#define log(label, area, level) \
{ \
  std::stringstream ss; \
  bool createLogEntry = level <= area_levels[area]; \
  if(createLogEntry) \
  ss << std::setw(5) << "[" << GetCurrentThreadId() << "]\t[" << label << "]\t[" << area << "]\t[" << levels[level/10] << "]\t"



#define endlog                          \
  std::ends;                            \
    if(_logger && createLogEntry) {     \
    _logger->logEntry(ss.str());        \
    }                                   \
}


class Tracer
{
private:
  std::string mLabel;
  std::string mArea;
  std::string mWhat;

public:
  Tracer(const std::string &label, const std::string &area, const std::string &what) 
    : mLabel(label), mArea(area), mWhat(what)
  { log(mLabel, mArea, eTrace) << "-->> " << mWhat << endlog; }

  ~Tracer()
  { log(mLabel, mArea, eTrace) << "<<-- " << mWhat << endlog; }

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__54F62A94_DA20_4119_AAD6_8246D664959B__INCLUDED)
