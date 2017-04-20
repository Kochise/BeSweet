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

#ifndef CommandHandlerVC7_H
#define CommandHandlerVC7_H

#pragma once
#include "resource.h"

//#pragma warning( disable : 4278 )
//The following #import imports DTE based on it's LIBID
//#import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" version("7.0") lcid("0") raw_interfaces_only named_guids
//#pragma warning( default : 4278 )

#include <CommandHandler.h>
#include <ObjectModelVC7/ObjectModelVC7.h>

#include <vector>
#include <string>

namespace VisualStudio7
{

struct Cmd
{
  std::wstring full_name;
  std::wstring name;
  std::wstring text;
  std::wstring tooltip;
  UINT bitmap;

  Cmd(const std::wstring& f, const std::wstring& s, const std::wstring& t, const std::wstring& x, UINT bmp)
    : full_name(f), name(s), text(t), tooltip(x), bitmap(bmp) {}
};

class ATL_NO_VTABLE CommandHandler : public ::CommandHandler,
	//public CComObjectRootEx<CComSingleThreadModel>,
  //public CComCoClass<VisualStudio7::CommandHandler, &CLSID_Connect>,
	public IDispatchImpl<EnvDTE::IDTCommandTarget, &EnvDTE::IID_IDTCommandTarget, &EnvDTE::LIBID_EnvDTE, 7, 0>
{
public:
	CommandHandler();
  ~CommandHandler(){};

  //DECLARE_REGISTRY_RESOURCEID(IDR_COMMANDS_VC7)
  //DECLARE_NOT_AGGREGATABLE(VisualStudio7::CommandHandler)


  //BEGIN_COM_MAP(VisualStudio7::CommandHandler)
	//  COM_INTERFACE_ENTRY(EnvDTE::IDTCommandTarget)
  //END_COM_MAP()



	//DECLARE_PROTECT_FINAL_CONSTRUCT()

	//HRESULT FinalConstruct()
	//{
	//	return S_OK;
	//}
	
	//void FinalRelease() 
	//{
	//}

public:	
	//IDTCommandTarget implementation:
	STDMETHOD(QueryStatus)(BSTR CmdName, EnvDTE::vsCommandStatusTextWanted NeededText, EnvDTE::vsCommandStatus *StatusOption, VARIANT *CommandText);
	STDMETHOD(Exec)(BSTR CmdName, EnvDTE::vsCommandExecOption ExecuteOption, VARIANT *VariantIn, VARIANT *VariantOut, VARIANT_BOOL *Handled);

  const std::vector<Cmd>& getCommands() const
  { return mCommands; }

protected:
  virtual std::string getSelectedText();

  std::vector<Cmd>::const_iterator findCommand(const BSTR&);

private:
  std::vector<Cmd> mCommands;
};


} //namespace VisualStudio7

#endif