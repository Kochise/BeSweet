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

// stdafx.cpp : source file that includes just the standard includes
//	ESBServer.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void message_box(const std::string &text)
{
  ::MessageBox(NULL, text.c_str(), "BeSweet", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
}

LoggerConsumer* _logger = NULL;
 const std::string GENERAL("General");

//areas
 const std::string MODEL("Model");
 const std::string CONTROLLER("Controller");
 const std::string RENDERER("Renderer");
 const std::string REMOTE_SRV("COM Server");
 const std::string REMOTE_MGR("COM Server Manager");
 const std::string REMOTE_EDITOR("COM Editor");
  const std::string CTAGS("ctags");
  const std::string NO_LABEL("--");
 

std::map<std::string, int> area_levels;

struct LoggerInitializer
{
  LoggerInitializer() {
    area_levels[CTAGS] = eDebug;
    area_levels[MODEL] = eDebug;
    area_levels[CONTROLLER] = eDebug;
    area_levels[RENDERER] = eDebug;
    area_levels[REMOTE_SRV] = eDebug;
    area_levels[REMOTE_MGR] = eDebug;
    area_levels[REMOTE_EDITOR] = eDebug;
  }
};
LoggerInitializer loggerInitializer;

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#endif
#include <atlimpl.cpp>


///////////////////////////////////// loggermanager.cpp ///////////////////////////
using namespace std;

namespace Logging {

//************************************************************************************************
//Constructor()
//************************************************************************************************
LoggerManager::LoggerManager()
:mDefaultLogLevel(eError), mInternalLogger(new LoggerImpl("")), mFile("internals.log")
{ 
  mInternalLogger->registerConsumer(make_smart_ptr(new StreamConsumer(mFile)));
}

//************************************************************************************************
//Constructor()
//************************************************************************************************
LoggerManager& LoggerManager::instance()
{
  static LoggerManager theInstance;
  return theInstance;
}


//************************************************************************************************
//map()
//************************************************************************************************
void LoggerManager::map(const string &newLabel, const string &existingLabel)
{	

  LoggerDictionary::iterator logger = mLoggers.find(existingLabel);
	
	if(logger != mLoggers.end()) {
		mLoggers[newLabel] = logger->second;
		
	} else {
    //LogStream s(eError, "internal", mInternalLogger);
		//s << "Could not map new label \"" << newLabel << "\" on label \"" << existingLabel << "\"because old label doesn't exist!!!" << endlog;
	}
}

//************************************************************************************************
//map()
//************************************************************************************************
void LoggerManager::map(const string &label, const smart_ptr<LoggerImpl> &logger)
{
	mLoggers[label] = logger;	
}


//************************************************************************************************
//unmap()
//************************************************************************************************
void LoggerManager::unmap(const string &label)
{
	//AutoLock a(&m_protector);
	mLoggers.erase(label);
}	

//************************************************************************************************
//unmap()
//************************************************************************************************
void LoggerManager::unmap(const smart_ptr<LoggerImpl> &logger)
{
	//AutoLock a(&m_protector);

/*	string name(loggerToRemove->name());

	if(mLoggers.erase(name)) {
		LabelDictionary::iterator current = mLabels.begin();
		LabelDictionary::iterator end = mLabels.end();
		Logger *logger = 0;
	
		while(current != end) {
			logger = current->second;
			if(logger && (logger->name() == loggerToRemove->name())) {
				current = mLabels.erase(current);

			} else {
				current++;
			}
		}
*/	
}

}; // namespace Logging