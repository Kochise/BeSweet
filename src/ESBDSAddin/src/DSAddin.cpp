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

// BeSweetAddin.cpp : Implementation of BeSweetAddin
#include "stdafx.h"
#include <stubs/ESBDSAddin.h>
#include "DSAddin.h"

#include "CommandHandler.h"
#include "EventHandler.h"

#include "ComHelper.h"

#include <iterator>
#include <vector>
#include <io.h>

using namespace std;

void startServer(const std::string &exe)
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::startServer");

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  string path = exe.substr(0, exe.rfind('\\')+1);
  vector<char> cmd(exe.begin(), exe.end());
  cmd.push_back(0);

  // Start the child process. 
  if(!CreateProcess( 
      &cmd[0], // No module name (use command line). 
      NULL, // Command line. 
      NULL,             // Process handle not inheritable. 
      NULL,             // Thread handle not inheritable. 
      FALSE,            // Set handle inheritance to FALSE. 
      DETACHED_PROCESS, // start detached (no FreeHandle(h) needed)
      NULL,             // Use parent's environment block. 
      path.c_str(),     // starting directory. 
      &si,              // Pointer to STARTUPINFO structure.
      &pi )) {          // Pointer to PROCESS_INFORMATION structure.

    LPVOID lpMsgBuf;
    FormatMessage( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM | 
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &lpMsgBuf,
      0,
      NULL 
      );
    string reason = (char*)lpMsgBuf;
    LocalFree( lpMsgBuf );
    throw runtime_error("failed to start server-exe\n'" + exe + "'\nReason: " + reason);
  }

  Sleep(1000);
   
}

class Lockable
{
public:
  virtual void lock() = 0;
  virtual void unlock() = 0;
  virtual bool isLocked() = 0;
};

class CriticalSection : public Lockable
{
private:
  CRITICAL_SECTION h;

public:
  CriticalSection()  { InitializeCriticalSection(&h);}
  ~CriticalSection() { DeleteCriticalSection(&h);}

  void lock() { EnterCriticalSection(&h); }
  void unlock(){ LeaveCriticalSection(&h);}

  bool isLocked() 
  { 
    if(TryEnterCriticalSection(&h) == TRUE) {
      LeaveCriticalSection(&h); 
      return false;
    }
    return true;
  }

};

class LockGuard
{
private:
  Lockable *mLockable;

public:
  LockGuard(Lockable *mutex) : mLockable(mutex) { mLockable->lock(); }
  ~LockGuard() { mLockable->unlock(); }
};

CComPtr<IESBServerManager> getMgr()
{
  CComPtr<IESBServerManager> mgr;
  cex_ = mgr.CoCreateInstance(CLSID_ESBServerManagerCoClass); 
  return mgr;
}


bool BeSweetAddin::retry()
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::retry");
  static bool isLocked = false;
  
  bool doRetry = !isLocked;

  if(!isLocked) {
    isLocked = true;

    string pidFile = getProperties().getServer() + ".pid";
    if(access(pidFile.c_str(), 0) == 0) {
      string text = 
        "<" + pidFile + "> already exists\n\n"
        "This means that either the server is still/already running\n"
        "or that was not terminated properly. Check for an besweet icon\n"
        "in the systray and terminate the server manualy\n\n"
        "Abort to avoid server startup\n"
        "Retry to remove the pid file and continue startup\n"
        "Ignore to procceed anyway\n";

      switch(MessageBox(NULL, text.c_str(), "BeSweet", MB_ABORTRETRYIGNORE  | MB_ICONWARNING | MB_TOPMOST )) {
        case IDABORT: doRetry = false; break;
        
        case IDRETRY : DeleteFile(pidFile.c_str());  
        case IDIGNORE :
          doRetry = true;
          break;
      }

    } else if(access(getProperties().getServer().c_str(), 0) == 0) {
      doRetry = true;

    } else if(!getProperties().isSilent()) {
      MessageBox(NULL, ("server exe '" + getProperties().getServer() + "' not found").c_str(), "BeSweet", MB_OK | MB_TOPMOST);
      CFileDialog dlg(TRUE);
      if(dlg.DoModal()) {
        getProperties().setServer(dlg.GetPathName().GetBuffer(0));
        doRetry = true;
      }  
    }
  }
  
  isLocked = false;
  return doRetry;
}

CComPtr<IESBServerManager> BeSweetAddin::getServerManager(bool start)
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::getServerManager");
  
  try {
    return getMgr();

  } catch(_com_error &e) {
    log(BE_SWEET, ADDIN, eInfo) << "esbserver seems to be down " << endlog;

    if(start && retry()) { 
      try {
        log(BE_SWEET, ADDIN, eDebug) << "rechecking if server is running...." << endlog;
        return getMgr();
        
      } catch(...) {
         log(BE_SWEET, ADDIN, eDebug) << "...nope, server still down" << endlog;
         startServer(getProperties().getServer());
         reopenWorkspace();
      }

      return getMgr();
    }
    
    throw runtime_error(string("BeSweet server not running\n") + e.ErrorMessage());
  }

  return NULL;
}

CComPtr<IESBServer> BeSweetAddin::getESBServer()
{
  if(getProperties().getWorkspace().empty()) {
    log(BE_SWEET, ADDIN, eError) << "BeSweetAddin::getESBServer(): why is workspace-name empty ???" << endlog;
    return CComPtr<IESBServer>();
  }

  CComPtr<IESBServer> server;
  string workspace= getProperties().getWorkspace();
  cex_ = getServerManager(true)->getWorkspaceServer(CComBSTR(workspace.c_str()), &server);
  return server;
}

void BeSweetAddin::enable(bool enable)
{
  getProperties().enabled(true);
}

void BeSweetAddin::loadWorkspace(const std::string &workspace, const std::list<std::string> &options)
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::loadWorkspace");
  log(BE_SWEET, ADDIN, eInfo) << "loading workspace " << workspace << endlog;
  for(list<string>::const_iterator x = options.begin(); x != options.end(); ++x) {
    log(BE_SWEET, ADDIN, eInfo) << *x << endlog;
  }
  string oldWorkspace = getProperties().getWorkspace();
  list<string> oldOptions = getProperties().getOptions();

  getProperties().setWorkspace(workspace);
  getProperties().setOptions(options);

  if(getProperties().getOptions().empty()) {
    ::MessageBox(NULL, "No workspaces open !!", "Be Sweet Addin", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
    throw runtime_error("No workspaces open !!");
  }

  if(handleDisabledState()) {
    CComPtr<IESBServerManager> mgr = getServerManager(true);
    if(!oldWorkspace.empty()) {
      mgr->removeWorkspaceServer(CComBSTR(oldWorkspace.c_str()));
    }
    
    if(isWorkspaceValid()) {
      CComPtr<IESBServer> server;
      mgr->newWorkspaceServer(CComBSTR(getProperties().getWorkspace().c_str()), dynamic_cast<IESBDSAddin*>(this), &server);
      
      stringstream options;
      copy(getProperties().getOptions().begin(), getProperties().getOptions().end(), ostream_iterator<string>(options, ";"));
      server->loadWorkspace(CComBSTR(options.str().c_str()));
    
    } else {
      MessageBox(0, ("workspace -" + getProperties().getWorkspace() + "- name empty...\nreopen Visual Studio workspace again!\n (File->Close Workspace, File->Recent Workspace)").c_str(), "Be Sweet Error", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
    }
  }
}

void BeSweetAddin::closeWorkspace()
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::closeWorkspace");
  log(BE_SWEET, ADDIN, eInfo) << "closing workspace " << getProperties().getWorkspace() << endlog;

  try {
    if(isWorkspaceValid()) {
      getServerManager(false)->removeWorkspaceServer(CComBSTR(getProperties().getWorkspace().c_str()));
    }
    getProperties().setOptions(list<string>());
    getProperties().setWorkspace("");

  } catch(...) {}
}

void BeSweetAddin::reopenWorkspace()
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::reopenWorkspace");

  if(handleDisabledState()) {
    string wspace = getProperties().getWorkspace();
    list<string> options = getProperties().getOptions();
    closeWorkspace();
    loadWorkspace(wspace, options);
  }
}
  
void BeSweetAddin::documentChanged(const std::string &filename)
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::documentChanged");
  log(BE_SWEET, ADDIN, eInfo) << "changed document " << filename << endlog;
  
  if(isEnabled() && isWorkspaceValid()) {
    getESBServer()->fileContentChanged(CComBSTR(filename.c_str()));
  }
}

void BeSweetAddin::documentActivated(const std::string &filename)
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::documentActivated");
  log(BE_SWEET, ADDIN, eInfo) << "document activated " << filename << endlog;

  if(isEnabled() && isWorkspaceValid()) {
    getESBServer()->showFileOutline(CComBSTR(filename.c_str()));
  }
}

void BeSweetAddin::showBrowser(const std::string &browser, const std::string arg)
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::showBrowser");
  log(BE_SWEET, ADDIN, eInfo) << "showBrowser <" << browser << ">|<" << arg << ">" << endlog;

  if(handleDisabledState()) {
    getESBServer()->showBrowser(CComBSTR(browser.c_str()), CComBSTR(arg.c_str()));
  }
}

bool BeSweetAddin::handleDisabledState()
{
  if(!isEnabled()) {
    if(mProperties.isSilent()) {
      return false;
    }

    if(MessageBox(NULL, "Enable the BeSweet Addin", "BeSweet", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST) == IDNO) {
      return false;
    }
    enable(true);
    if(!isWorkspaceValid()) {
      MessageBox(NULL, "Workspace empty\n Please reload Visual Studio workspace again!\n (File->Close Workspace, File->Recent Workspace)", "BeSweet", MB_OK | MB_ICONERROR);
      return false;
    }

    reopenWorkspace();
  }

  return true;
}

void BeSweetAddin::modifyProperties()
{
  Tracer trc(BE_SWEET, ADDIN, "BeSweetAddin::modifyProperties");
  
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if(mProperties.DoModal() == IDOK && mProperties.needsReload()) {
    reopenWorkspace();
  }
}

HRESULT BeSweetAddin::gotoLine(BSTR filename, long line)
{
  Tracer trc(BE_SWEET, "Addin", "BeSweetAddin::gotoLine");
  SANDBOX_BEGIN("ESBDSAddinImpl", "gotoLine")

  gotoLine(string(bstr_t(filename)), line);
  return S_OK;

  SANDBOX_END()
  return E_FAIL;
}

void log_std_exception(const std::string &class_name, const std::string &method_name, const std::exception &e)
{
  log(BE_SWEET, "", eError) << "exception occurred " << endl << class_name << "::" << method_name << endl
                            << typeid(e).name() << " " << e.what() << endlog;
}

void log_com_exception(const std::string &class_name, const std::string &method_name, const _com_error &e)
{
  log(BE_SWEET, "", eError) << "exception occurred " << endl << class_name << "::" << method_name << endl
                            << typeid(e).name() << " " << e.ErrorMessage() << endlog;
}

void log_unknown_exception(const std::string &class_name, const std::string &method_name)
{
  log(BE_SWEET, "", eError) << "unknown exception occurred " << endl << class_name << "::" << method_name << endlog;
}





















