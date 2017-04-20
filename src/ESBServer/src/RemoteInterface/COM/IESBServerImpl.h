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

// ESBServerImpl.h : Declaration of the ESBServer

#ifndef __ESBSERVER_H_
#define __ESBSERVER_H_

class ESBServer;

/////////////////////////////////////////////////////////////////////////////
// ESBServer
class ATL_NO_VTABLE IESBServerImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<IESBServerImpl, &CLSID_ESBServerCoClass>,
	public IDispatchImpl<IESBServer, &IID_IESBServer, &LIBID_ESBServerLib>
{
private:
  ESBServer *mServer;
  
public:
  IESBServerImpl() : mServer(NULL) { }
  ~IESBServerImpl() { mServer = NULL; }
  
  void setServer(ESBServer *s) { mServer = s; }
  ESBServer* getServer() { return mServer; };

DECLARE_REGISTRY_RESOURCEID(IDR_ESBSERVER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(IESBServerImpl)
	COM_INTERFACE_ENTRY(IESBServer)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IESBServer
public:
	STDMETHOD(loadWorkspace)(BSTR packages);
	STDMETHOD(showFileOutline)(BSTR file);
	STDMETHOD(fileContentChanged)(BSTR file);
  STDMETHOD(showBrowser)(BSTR browser, BSTR what);
};


template<class Param>
void emit(int msgID, Param p, CWnd *wnd = NULL)
{
  CWnd * window = wnd == NULL ? AfxGetMainWnd() : wnd;
  
  if(!wnd->PostMessage(msgID, 0, (LPARAM)new Param(p))) {
    LPVOID lpMsgBuf;
    FormatMessage( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM | 
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &lpMsgBuf,
      0,
      NULL 
      );
    MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
    LocalFree( lpMsgBuf );
  }
}

#endif //__ESBSERVER_H_
