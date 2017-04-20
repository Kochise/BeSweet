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

#include "stdafx.h"
#include "resource.h"

#include "ESBServerComModule.h"
ESBServerComModule _Module;
#include <atlcom.h>

#include <vector>
#include <process.h>

#include <RemoteInterface/COM/stubs/IESBServer.h>
#include <RemoteInterface/COM/stubs/ESBServer_i.c>

#include <RemoteInterface/COM/IESBServerImpl.h>
#include <RemoteInterface/COM/IESBServerManagerImpl.h>

#include <io.h>

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_ESBServerCoClass, IESBServerImpl)
OBJECT_ENTRY(CLSID_ESBServerManagerCoClass, IESBServerManagerImpl)
END_OBJECT_MAP()

using namespace std;

ComModuleGuard::ComModuleGuard(CComModule & module, UINT id)
: mModule(module), mResourceID(id)
{
  if(::access(getFileName().c_str(), 0) == 0) {
    string text = 
      "<" + getFileName() + "> already exists\n\n"
      "This means that either the server is still/already running\n"
      "or was not terminated properly. Check for an besweet icon\n"
      "in the systray and terminate the server manualy\n\n"
      "Ok to coninue startup\n"
      "Cancel to cancel startup\n";

    if(MessageBox(NULL, text.c_str(), "BeSweet", MB_OKCANCEL | MB_TOPMOST | MB_ICONWARNING) != IDOK) {
      throw runtime_error("startup canceled");
    }
  }
  
  if(mModule.UpdateRegistryFromResource(mResourceID, TRUE) != S_OK ||
	   mModule.RegisterServer(TRUE) != S_OK) 
  {
    deinit();
    throw runtime_error("failed to register esbserver.exe"); 
  }
  mPIDFile = getFileName();
  std::ofstream pidfile(mPIDFile.c_str());
  pidfile << getpid();
}

ComModuleGuard::~ComModuleGuard()
{
  deinit(); 
}

void ComModuleGuard::deinit()
{
  mModule.UpdateRegistryFromResource(mResourceID, FALSE);
  mModule.UnregisterServer(TRUE); //TRUE means typelib is unreg'd
  if(!mPIDFile.empty()){
    DeleteFile(mPIDFile.c_str());
  }
}

std::string ComModuleGuard::getFileName()
{
  std::vector<char> buf(1024);
  GetModuleFileName(NULL, &buf[0], buf.size());
  return &buf[0] + std::string(".pid");
}

///////////////////////////////////////////////////////////////////////////////////
//  [9/30/2003]
///////////////////////////////////////////////////////////////////////////////////
ESBServerComModule::ESBServerComModule()
: mIsATLInitialized(false) 
{
}

ESBServerComModule& ESBServerComModule::instance()
{
	return _Module;
}

extern ESBServerComModule _Module;

LONG ESBServerComModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG ESBServerComModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR ESBServerComModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}

void ESBServerComModule::UninitializeATL()
{
	if(mIsATLInitialized)
	{
		RevokeClassObjects();
		Term();
		CoUninitialize();
		mIsATLInitialized = false;
	}
}

bool ESBServerComModule::InitializeATL()
{
	mIsATLInitialized = true;

#if _WIN32_WINNT >= 0x0400
	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = CoInitialize(NULL);
#endif

	if (FAILED(hRes))
	{
		mIsATLInitialized = FALSE;
		return FALSE;
	}

	Init(ObjectMap, AfxGetInstanceHandle());
	dwThreadID = GetCurrentThreadId();

	LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			UpdateRegistryFromResource(IDR_ESBSERVER, FALSE);
			UnregisterServer(TRUE); //TRUE means typelib is unreg'd
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			UpdateRegistryFromResource(IDR_ESBSERVER, TRUE);
			RegisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = FindOneOf(lpszToken, szTokens);
	}

	if (!bRun)
	{
		mIsATLInitialized = FALSE;
		Term();
		CoUninitialize();
		return FALSE;
	}

	hRes = RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		mIsATLInitialized = FALSE;
		CoUninitialize();
		return FALSE;
	}	

	return TRUE;

}
