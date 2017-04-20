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

#ifndef CommandHandlerVC6_H
#define CommandHandlerVC6_H

#include "resource.h"       // main symbols

#include <CommandHandler.h>
#include <ComHelper.h>

#include <string>

namespace VisualStudio6 {

class ATL_NO_VTABLE CommandHandler : public ::CommandHandler,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CommandHandler, &CLSID_Commands>,
	public IDispatchImpl<ICommands, &IID_ICommands, &LIBID_ESBDSADDINLib>
{
public:
  typedef ::CommandHandler super;

public:
  CommandHandler();
  ~CommandHandler();

  //ICommands
  HResult setProperties();
  HResult showOutlineBrowser();
  HResult showHierarchyBrowser();
  HResult showTypeBrowser();
  HResult showMethodBrowser();
  HResult showWorkspaceBrowser();

private:
  std::string getSelectedText();

public:
  //here comes the atl crap
  //DECLARE_REGISTRY_RESOURCEID(IDR_COMMANDS)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_CLASSFACTORY_SINGLETON(VisualStudio6::CommandHandler)

  BEGIN_COM_MAP(VisualStudio6::CommandHandler)
	  COM_INTERFACE_ENTRY(ICommands)
	  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

};

} // namespace  VisualStudio6

#endif
