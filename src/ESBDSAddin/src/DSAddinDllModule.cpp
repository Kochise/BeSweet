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

// ESBDSAddin.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ESBDSAddinps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>

#include <ObjectModelVC6/AddinVC6.h>
#include <ObjectModelVC7/AddinVC7.h>
#include <ObjectModelVC7/CommandHandlerVC7.h>

#include <stubs/ESBDSAddin.h>
#include <stubs/ESBDSAddin_i.c>
#include <stubs/ESBServer_i.c>

#include "ComHelper.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
  OBJECT_ENTRY(CLSID_ESBDSAddin, VisualStudio6::Addin)
//  OBJECT_ENTRY(CLSID_Commands, VisualStudio6::CommandHandler)
  OBJECT_ENTRY(CLSID_ESBDSAddin, VisualStudio7::Addin)
//  OBJECT_ENTRY(CLSID_Connect, VisualStudio7::CommandHandler) 
END_OBJECT_MAP()

BEGIN_OBJECT_MAP(VC6ObjectMap)
  OBJECT_ENTRY(CLSID_ESBDSAddin, VisualStudio6::Addin)
END_OBJECT_MAP()

BEGIN_OBJECT_MAP(VC7ObjectMap)
  OBJECT_ENTRY(CLSID_ESBDSAddin, VisualStudio7::Addin)
//  OBJECT_ENTRY(CLSID_Connect, VisualStudio7::CommandHandler) 
END_OBJECT_MAP()

class TheAddinApp : public CWinApp
{
  BOOL InitInstance()
  {
    if (GetModuleHandle("MSDEV.EXE") != NULL  || GetModuleHandle("EVC.EXE") != NULL) {
      _Module.Init(VC6ObjectMap, m_hInstance);

    } else if(GetModuleHandle("devenv.exe") != NULL) {
      _Module.Init(VC7ObjectMap, m_hInstance);

    } else {
      _Module.Init(ObjectMap, m_hInstance);
    }

    //_Module.Init(ObjectMap, m_hInstance);
    return CWinApp::InitInstance();
  }
  
  int ExitInstance()
  {
    _Module.Term();
    return CWinApp::ExitInstance();
  }

  
};

TheAddinApp theapp;

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

//static AFX_EXTENSION_MODULE extensionDLL;

//extern "C" 
//int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
//{
//  switch(dwReason)
//  {
//    case DLL_PROCESS_ATTACH:
//      {
//        DebugBox("DllMain(): attach");
//        
//        // Extension DLL one-time initialization 
//        if (!AfxInitExtensionModule(extensionDLL, hInstance)) {
//          return 0;
//        }
//
//        _Module.Init(ObjectMap, hInstance, &LIBID_ESBDSADDINLib);
//        DisableThreadLibraryCalls(hInstance);
//        break;
//      }
//    case DLL_PROCESS_DETACH:
//      {
//        _Module.Term();
//        break;
//      }
//  }
//  
//  return TRUE;
//}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    //::MessageBox(NULL, "DllGetClassObject", "", MB_OK);
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}

