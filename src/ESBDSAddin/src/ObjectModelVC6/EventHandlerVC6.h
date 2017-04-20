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

#ifndef ESB_EventHandler_H
#define ESB_EventHandler_H

#include "ComHelper.h"
#include <string>
#include <vector>


namespace VisualStudio6 {

class Addin;

template <class IEvents,const GUID* plibid, class XEvents, const CLSID* pClsidEvents>
class XEventHandler :
                      public CComDualImpl<IEvents, &__uuidof(IEvents), plibid>,
	                    public CComObjectRoot,
	                    public CComCoClass<XEvents, pClsidEvents>
{
private:
  //CComPtr<IApplication> mApplication;
  VisualStudio6::Addin *mAddin;
  
public :
  XEventHandler() : mAddin(NULL) {}

  void setAddIn(VisualStudio6::Addin *addin)
  { 
    if(mAddin != NULL)
    {
      disconnect(mAddin->getApplication());
    }
    mAddin = addin;

    connect(mAddin->getApplication());
  }

  VisualStudio6::Addin* getAddIn()
  { return mAddin; }

	void connect(IUnknown* pUnk)
	{ cex_ = AtlAdvise(pUnk, this, __uuidof(IEvents), &mCookie); }

	void disconnect(IUnknown* pUnk)
	{ AtlUnadvise(pUnk, __uuidof(IEvents), mCookie); }

public:
	BEGIN_COM_MAP(XEvents)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY_IID(__uuidof(IEvents), IEvents)
	END_COM_MAP()
	DECLARE_NOT_AGGREGATABLE(XEvents)

protected:
	DWORD mCookie;
};


  //  object handles events fired by the Application object
class ApplicationEventHandler : public XEventHandler<IApplicationEvents, &LIBID_ESBDSADDINLib, ApplicationEventHandler, &CLSID_ApplicationEvents>
{
  
public:
	// IApplicationEvents methods
	HResult DocumentSave(IDispatch * theDocument);
	HResult WindowActivate(IDispatch * theWindow);
  HResult WorkspaceOpen();
	HResult WorkspaceClose();

  void GenericDocumentSaved(CComQIPtr<IGenericDocument> &theDocument);
  void GenericWindowActivated(CComQIPtr<IGenericWindow> & theWindow);
  void WorkspaceOpened(const std::string&);

private:
  std::string selectWorkspace(const std::vector<std::string> &, const std::string&);
  std::string getWorkspace(const std::string&);

    //unused
  HResult BeforeBuildStart() { return S_OK; };
	HResult BuildFinish(long nNumErrors, long nNumWarnings) { return S_OK; };
	HResult BeforeApplicationShutDown() { return S_OK; };
	HResult DocumentOpen(IDispatch * theDocument) { return S_OK; };
	HResult BeforeDocumentClose(IDispatch * theDocument) { return S_OK; };
  HResult NewDocument(IDispatch * theDocument) { return S_OK; };
  HResult WindowDeactivate(IDispatch * theWindow) { return S_OK; };
	HResult NewWorkspace() { return S_OK; };
};

} // namespace VisualStudio6

#endif