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

#include <DSAddin.h>

#ifndef BeSweetAddinVC7_H
#define BeSweetAddinVC7_H

#pragma once
#include "resource.h"       // main symbols

//#pragma warning( disable : 4278 )
//The following #import imports DTE based on it's LIBID
//#import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" version("7.0") lcid("0") raw_interfaces_only named_guids
//#pragma warning( default : 4278 )

#include <DSAddin.h>
#include <ObjectModelVC7/ObjectModelVC7.h>
#include <ObjectModelVC7/EventHandlerVC7.h>
#include <ObjectModelVC7/CommandHandlerVC7.h>

#include <list>

namespace VisualStudio7
{

typedef IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &AddInDesignerObjects::IID__IDTExtensibility2, &AddInDesignerObjects::LIBID_AddInDesignerObjects, 1, 0> IAddin;

class ATL_NO_VTABLE Addin : public BeSweetAddin,
	public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<VisualStudio7::Addin, &CLSID_ESBDSAddin>,
  public VisualStudio7::CommandHandler,
	public IAddin
{
public:
	Addin();

  DECLARE_REGISTRY_RESOURCEID(IDR_ADDIN_VC7)
  DECLARE_NOT_AGGREGATABLE(VisualStudio7::Addin)
  DECLARE_CLASSFACTORY_SINGLETON(VisualStudio7::Addin);


BEGIN_COM_MAP(VisualStudio7::Addin)
	COM_INTERFACE_ENTRY(AddInDesignerObjects::IDTExtensibility2)
	COM_INTERFACE_ENTRY2(IDispatch, AddInDesignerObjects::IDTExtensibility2)
  COM_INTERFACE_ENTRY(EnvDTE::IDTCommandTarget)
  COM_INTERFACE_ENTRY(IESBDSAddin)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
  { return S_OK; }
	
	void FinalRelease() 
  { }

public:
	//IDTExtensibility2 implementation:
	STDMETHOD(OnConnection)(IDispatch * Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch *AddInInst, SAFEARRAY **custom);
	STDMETHOD(OnDisconnection)(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY **custom );
	STDMETHOD(OnAddInsUpdate)(SAFEARRAY **custom );
	STDMETHOD(OnStartupComplete)(SAFEARRAY **custom );
	STDMETHOD(OnBeginShutdown)(SAFEARRAY **custom );
	

  // IESBDSAddInImpl
  void gotoLine(const std::string & filename, long line);
  
  const CComQIPtr<EnvDTE::_DTE>& getApplication() { return mDTE; }

  VisualStudio7::EventHandler* getEventHandler() { return &mEventHandler; }
  VisualStudio7::CommandHandler* getCommandHandler() { return mCommandHandler; }

private:
  CComQIPtr<EnvDTE::_DTE> mDTE;
	CComQIPtr<EnvDTE::AddIn> mAddin;

  VisualStudio7::EventHandler mEventHandler; 
//  VisualStudio7::WindowEventHandler WinEvenH;
  VisualStudio7::CommandHandler *mCommandHandler;
  //CComObject<VisualStudio7::CommandHandler> *mCommandHandler;

  void registerCommands();
  void registerEvents();
  void deregisterEvents();

  std::string mWorkspace;
  std::list<std::string> mOptions;

	CComPtr<EnvDTE::Command> newCommand(const Cmd &cmd, CComPtr<EnvDTE::Commands> &commands);
  CComPtr<Office::CommandBar> getToolbar();
};

} //namespace VisualStudio7

#endif