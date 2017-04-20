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

// ESBServerManager.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "Resource.h"

#include "ESBServerManager.h"
#include <RemoteInterface/COM/ESBServerComModule.h>
#include <Controller/ESBServer.h>
#include <Renderer/MFC/VersionCheckPanel.h>

#include <Utilities/Stacktrace.h>

//#include <initguid.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESBServerManager

BEGIN_MESSAGE_MAP(ESBServerManager, CWinApp)
	//{{AFX_MSG_MAP(ESBServerManager)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ESBServerManager construction

ESBServerManager::ESBServerManager()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only ESBServerManager object

struct AppCreator { AppCreator() { ESBServerManager::instance(); }; };
AppCreator theApp;
SEHTranslator theTranslator;

/////////////////////////////////////////////////////////////////////////////
// ESBServerManager initialization
ESBServerManager& ESBServerManager::instance()
{
  static ESBServerManager theInstance;
  return theInstance;
}

template<class T>
struct compare1st : public binary_function<T, typename T::first_type, bool>
{
  result_type operator()(const first_argument_type &p, const second_argument_type &x) const
  { return p.first == x; }
};

ESBServerManager::ServerContainerType::iterator ESBServerManager::find(const std::string &wspace)
{
  return find_if(mWorkspaceServers.begin(), mWorkspaceServers.end(), bind2nd(compare1st<ServerContainerType::value_type>(), wspace));
}

ESBServer& ESBServerManager::getWorkspaceServer(const std::string &wspace)
{
  ServerContainerType::iterator server = find(wspace);
  if(server == mWorkspaceServers.end()) {
    throw runtime_error("no server for workspace <" + wspace + ">");
  }

  return *server->second;
}

ESBServer& ESBServerManager::newWorkspaceServer(const std::string &wspace)
{
  ServerContainerType::iterator server = find(wspace);
  if(server != mWorkspaceServers.end()) {
    throw runtime_error("server already exists for workspace <" + wspace + ">");
  }
  
  mWorkspaceServers.push_back(make_pair(wspace, smart_ptr<ESBServer>(new ESBServer(wspace))));
  mRenderer->AddPage(dynamic_cast<COptionPage*>(mWorkspaceServers.back().second->getRenderer()));
  mRenderer->SetActivePage(mRenderer->GetPageCount()-1);
  
  return *mWorkspaceServers.back().second;
}

bool ESBServerManager::removeWorkspaceServer(std::string wspace)
{
  ServerContainerType::iterator server = find(wspace);
  if(server == mWorkspaceServers.end()) {
   return false;
  }
  int active = mRenderer->GetActiveIndex();
  int index = distance(mWorkspaceServers.begin(), server);

  if(active == index) {
    mRenderer->SetActivePage(0);
  }
  mRenderer->RemovePage(index);
  mWorkspaceServers.erase(server);
  return true;
}


DWORD WINAPI check4newVersion(void*)
{
  try {
    VersionCheckPanel checker;
    checker.Create(VersionCheckPanel::IDD);
    checker.ShowWindow(SW_SHOW);
    //checker.SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
    checker.run();
    checker.DestroyWindow();
    
  } catch(std::exception &e) {
    log(GENERAL, CONTROLLER, eError) << "check4newVersion(): " << e.what() << endlog;

  } catch(...) {
    log(GENERAL, CONTROLLER, eError) << "check4newVersion(): unknown exception occurred" << endlog;
  }

  return 0;
}

void makeOnlineVersionCheck()
{
  int cnt = lexical_cast<int>(PropertiesManager::instance().getProperty(BeSweet::START_COUNTER, "0"));
  
  string installTime; 
  try {
      installTime = PropertiesManager::instance().getProperty(BeSweet::INSTALL_TIME);
  } catch(std::exception &) {
      installTime = lexical_cast<string>(time(NULL));
  }
  
  PropertiesManager::instance().setProperty(BeSweet::INSTALL_TIME, installTime);
  PropertiesManager::instance().setProperty(BeSweet::START_COUNTER, lexical_cast<string>(cnt+1));
  PropertiesManager::instance().dump();

  if(PropertiesManager::instance().getProperty(BeSweet::CHECK_4_NEW_VERSION, "1") == "1") {
    DWORD tid;
    CloseHandle(::CreateThread(NULL, 0, check4newVersion, NULL, 0/*DETACHED_THREAD*/, &tid));
  }
}

BOOL ESBServerManager::InitInstance()
{
  AfxEnableControlContainer();
  
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	try {
    if(!ESBServerComModule::instance().InitializeATL()) {
      throw runtime_error("InitializeATL() failed");
    }

    ComModuleGuard guard(ESBServerComModule::instance(), IDR_ESBSERVER_MANAGER);
  
    makeOnlineVersionCheck();
  
    mRenderer = make_smart_ptr(new ESBServerManagerRenderer());
    m_pMainWnd = mRenderer.get();
		mRenderer->DoModal();
    mRenderer->DestroyWindow();

	} catch(std::exception &e) {
		message_box(string("ESBServerManager::InitInstance():") + e.what());
	
	} catch(...) {
		message_box("ESBServerManager::InitInstance(): unknown exception");
	}

  // Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

	
int ESBServerManager::ExitInstance()
{
	ESBServerComModule::instance().UninitializeATL();
	return CWinApp::ExitInstance();
}
