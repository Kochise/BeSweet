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

// ESBDSAddinImpl.cpp : Implementation of ESBDSAddinImpl
#include "stdafx.h"
#include "ESBDSAddinImplX.h"

#include "ComHelper.h"

SimulatorComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
  OBJECT_ENTRY(CLSID_ESBDSAddin, AddinSimulator)
END_OBJECT_MAP()

#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// ESBDSAddinImpl

DWORD AddinSimulator::mClassRegistrationCookie = 0;

void DebugBox(const std::string &text) 
{
#ifdef DEBUG
  ::MessageBox(NULL, text.c_str(), "BeSweet Addin Simulator", MB_OK | MB_TOPMOST);
#endif
}

AddinSimulator::AddinSimulator() 
: mCookie(0)
{
  DebugBox("AddinSimulator::AddinSimulator()");
}


HRESULT AddinSimulator::gotoLine(BSTR filename, long line)
{
  const static string function = "AddinSimulator::gotoLine(): ";
  DebugBox("AddinSimulator::gotoLine(): ");

  try {

  } catch(std::exception &e) {
		DebugBox(function + e.what());

	} catch(_com_error &e) {
		DebugBox(function + asString(e));
		
	} catch(...) {
    DebugBox(function + " unknown error");
  }

  return E_FAIL;
}
/*
void AddinSimulator::setupComStuff()
{
  CComPtr<IUnknown> pIUnk = 0;
  cex_ = _Module.GetClassObject( GetObjectCLSID(), IID_IUnknown, reinterpret_cast<void**>(&pIUnk));
  cex_ = CoRegisterClassObject(GetObjectCLSID(), pIUnk, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &mClassRegistrationCookie);
}

void AddinSimulator::tearDownComStuff()
{
  cex_ = CoRevokeClassObject(mClassRegistrationCookie);
}*/


/////////////////////////////////////////////////////////////////////////
SimulatorComModule::SimulatorComModule()
: mIsATLInitialized(false) 
{
}

SimulatorComModule& SimulatorComModule::instance()
{
	return _Module;
}

extern SimulatorComModule _Module;

LONG SimulatorComModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG SimulatorComModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR SimulatorComModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
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

void SimulatorComModule::UninitializeATL()
{
	if(mIsATLInitialized)
	{
		RevokeClassObjects();
		Term();
		CoUninitialize();
		mIsATLInitialized = false;
	}
}

bool SimulatorComModule::InitializeATL()
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
			UpdateRegistryFromResource(IDR_ESBDSADDIN, FALSE);
			UnregisterServer(TRUE); //TRUE means typelib is unreg'd
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			UpdateRegistryFromResource(IDR_ESBDSADDIN, TRUE);
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