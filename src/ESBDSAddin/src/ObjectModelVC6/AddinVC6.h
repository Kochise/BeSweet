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

#ifndef __AddinVC6_H_
#define __AddinVC6_H_

#include "resource.h"       // main symbols
//#import "C:\Program Files\Development\Microsoft Visual Studio\Common\IDE\IDE98\DTE.TLB" raw_interfaces_only, raw_native_types, no_namespace, named_guids 

#include "DSAddin.h"
#include "EventHandlerVC6.h"
#include "CommandHandlerVC6.h"

#include "ESBServer.h"

namespace VisualStudio6 {

class ATL_NO_VTABLE Addin : public BeSweetAddin,
	public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<VisualStudio6::Addin, &CLSID_ESBDSAddin>,
	//public IDispatchImpl<IESBDSAddin, &IID_IESBDSAddin, &LIBID_ESBDSADDINLib>,
  public IDSAddIn
{
private:
  CComObject<VisualStudio6::CommandHandler> *mCommandHandler;
  //CComPtr<VisualStudio6::CommandHandler>	mCommandHandler;
  CComPtr<VisualStudio6::ApplicationEventHandler> mEventHandler;
  
  CComPtr<IApplication> mApplication;
  DWORD mClassRegistrationCookie;
  DWORD mCookie;
  int mTimerID;


public:
	Addin();

public:
  //IDSAddIn
	HResult OnConnection(IApplication* pApp, VARIANT_BOOL bFirstTime,	long dwCookie, VARIANT_BOOL* OnConnection);
	HResult OnDisconnection(VARIANT_BOOL bLastTime);

  // IESBDSAddInImpl
  void gotoLine(const std::string & filename, long line);

  CComPtr<IApplication>& getApplication()
  { return mApplication; }

  CComPtr<VisualStudio6::ApplicationEventHandler>& getEventHandler()
  { return mEventHandler; }
    

private:
  void setupComStuff();
  void setupAddIn(bool);

  void onTimer();
  friend void __stdcall onTimerX(HWND, UINT, UINT, unsigned long);
  
public:
  //the atl crap goes here
  DECLARE_REGISTRY_RESOURCEID(IDR_ADDIN_VC6)
  DECLARE_CLASSFACTORY_SINGLETON(VisualStudio6::Addin);

  BEGIN_COM_MAP(VisualStudio6::Addin)
	  COM_INTERFACE_ENTRY(IESBDSAddin)
    COM_INTERFACE_ENTRY(IDSAddIn)
	  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

};

} // namespace VisualStudio6

#endif //__ESBDSADDIN_H_
