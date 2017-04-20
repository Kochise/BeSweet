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

// Addin.cpp : Implementation of Addin
#include "stdafx.h"
#include <stubs/ESBDSAddin.h>
#include "AddinVC6.h"

#include "CommandHandler.h"
#include "EventHandler.h"

#include "ComHelper.h"
#include "LoggerConsumerVC6.h"

#include <string>
#include <iterator>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Addin

namespace VisualStudio6 {

static Addin *addin = NULL;

#define function sbox_class_name+"::"+sbox_method_name+"()"

Addin::Addin() 
: mApplication(NULL), mCommandHandler(NULL), mEventHandler(NULL), 
  mCookie(0), mClassRegistrationCookie(0)
{
  log(BE_SWEET, ADDIN, eDebug) << "Addin::Addin()" << endlog;
}

void __stdcall onTimerX(HWND, UINT, UINT, unsigned long)
{
  if(addin) {
    addin->onTimer();
  }
}

HRESULT Addin::OnConnection(IApplication* application, VARIANT_BOOL isFirstTime, long dwCookie, VARIANT_BOOL* pbOnConnection)
{
  addin = this;
  _logger = new VisualStudio6::LoggerConsumer(application);

  SANDBOX_BEGIN("VisualStudio6::Addin", "OnConnection")

  Tracer trc(BE_SWEET, ADDIN, function);
 
  *pbOnConnection = VARIANT_FALSE;

  setupComStuff();
  mApplication = application;
  mCookie = dwCookie;

  setupAddIn(isFirstTime == VARIANT_TRUE);

  mTimerID = SetTimer(NULL, 0, 1000, onTimerX);

  *pbOnConnection = VARIANT_TRUE;
  return S_OK;

  SANDBOX_END()
  
  return E_FAIL;
}

HRESULT Addin::OnDisconnection(VARIANT_BOOL bLastTime)
{
  SANDBOX_BEGIN("VisualStudio6::Addin", "OnDisconnection")

  mApplication = NULL;
  Tracer trc(BE_SWEET, ADDIN, function);
  
  CoRevokeClassObject(mClassRegistrationCookie);
  mCommandHandler = NULL;
  mEventHandler = NULL;

  KillTimer(NULL, mTimerID);

  SANDBOX_END()

  delete _logger;
  _logger = 0;

  return S_OK;
}

void Addin::onTimer()
{    
  static bool isMinimized = false;
  
  DsWindowState state;
  mApplication->get_WindowState(&state);
  
  bool minimized = state == dsWindowStateMinimized;
  if(isMinimized != minimized) {
    getESBServer()->showBrowser(CComBSTR(minimized ? "minimize" : "restore"), CComBSTR(""));
    isMinimized = minimized;
  }
}

void Addin::gotoLine(const std::string &filename, long line)
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::gotoLine");
  log(BE_SWEET, ADDIN, eInfo) << "open file \"" << filename << "\" at line " << line << endlog;
  
  CComPtr< IDispatch> tmp = 0;

  cex_ = getApplication()->get_Documents(&tmp.p);
  CComQIPtr< IDocuments> documents = tmp;
  
  tmp = 0;
  cex_ = documents->Open( CComBSTR(filename.c_str()), CComVariant(), CComVariant(), &tmp.p);
  CComQIPtr< ITextDocument> document = tmp;
  
  tmp = 0;
  cex_ = document->get_Selection( &tmp.p);
  CComQIPtr< ITextSelection> selection = tmp;

  cex_ = selection->GoToLine(line, CComVariant(1));
}

void Addin::setupComStuff()
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::setupComStuff");
  
  CComPtr<IUnknown> pIUnk = 0;
  cex_ = _Module.GetClassObject(GetObjectCLSID(), IID_IUnknown, reinterpret_cast<void**>(&pIUnk));
  cex_ = CoRegisterClassObject(GetObjectCLSID(), pIUnk, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &mClassRegistrationCookie);

  log(BE_SWEET, ADDIN, eDebug) << "setupComStuff succeeded" << endlog;
}

void Addin::setupAddIn(bool isFirstTime)
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::setupAddIn");

  //mCommandHandler = new_instance<VisualStudio6::CommandHandler>();
  CComObject<VisualStudio6::CommandHandler>::CreateInstance(&mCommandHandler);
  mCommandHandler->setAddIn(this);
//  try {
//    mCommandHandler->showHierarchyBrowser();
//  } catch(...) {
//    log(BE_SWEET, ADDIN, eError) << "failed to call" << endlog;
//  }
  
  mEventHandler = new_instance<VisualStudio6::ApplicationEventHandler>();
  mEventHandler->setAddIn(this);

  //tell vc about us....
  cex_ = getApplication()->SetAddInInfo((long)_Module.GetModuleInstance(), mCommandHandler, IDR_TOOLBAR_MEDIUM, IDR_TOOLBAR_LARGE, mCookie);
  //the event handler is already registered --> mApplicationEventHandler->setAddIn(this);
	
  // register our commands with VC
	VARIANT_BOOL bRet;

  const size_t commandCnt = 6;
  string commands[6][3] = { 
                          {"ESBProperties", "\n\n\nEnable/Disable","setProperties"},
                          {"ESBShowOutlineBrowser", "\n\n\nShow Outline", "showOutlineBrowser"}, 
                          {"ESBShowHierarchyBrowser", "\n\n\nShow Hierarchy", "showHierarchyBrowser"}, 
                          {"ESBShowTypeBrowser", "\n\n\nShow Types", "showTypeBrowser"}, 
                          {"ESBShowMethodBrowser", "\n\n\nShow Methods", "showMethodBrowser"}, 
                          {"ESBShowWorkspaceBrowser", "\n\n\nShow Workspace", "showWorkspaceBrowser"}, 
                         };

  for(int i = 0; i < commandCnt; i++) {
    cex_ = getApplication()->AddCommand(CComBSTR((commands[i][0]+ commands[i][1]).c_str()), CComBSTR(commands[i][2].c_str()), i, mCookie, &bRet);
	  if(isFirstTime) {
		  cex_ = getApplication()->AddCommandBarButton(dsGlyph, CComBSTR(commands[i][0].c_str()), mCookie);
	  }
  }
}

} //namespace VisualStudio6
