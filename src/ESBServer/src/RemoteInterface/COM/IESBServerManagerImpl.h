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

// IESBServerManagerImpl.h : Declaration of the IESBServerManagerImpl

#ifndef __ESBSERVERMANAGER_H_
#define __ESBSERVERMANAGER_H_

#include <map>
#include <string>

/////////////////////////////////////////////////////////////////////////////
// IESBServerManagerImpl
class ATL_NO_VTABLE IESBServerManagerImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<IESBServerManagerImpl, &CLSID_ESBServerManagerCoClass>,
	public IDispatchImpl<IESBServerManager, &IID_IESBServerManager, &LIBID_ESBServerLib>
{
private:
  typedef std::map<std::string, CComPtr<IESBServer> > ContainerType;
  ContainerType mWorkspaceServers;

public:
	IESBServerManagerImpl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ESBSERVER_MANAGER)

DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_CLASSFACTORY_SINGLETON(IESBServerManagerImpl);

BEGIN_COM_MAP(IESBServerManagerImpl)
	COM_INTERFACE_ENTRY(IESBServerManager)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
	STDMETHOD(newWorkspaceServer)(BSTR name, IESBDSAddin *vs, IESBServer **srv);
  STDMETHOD(getWorkspaceServer)(BSTR name, IESBServer **server);
  STDMETHOD(removeWorkspaceServer)(BSTR name);
  
private:
  ContainerType::iterator getServer(const BSTR&);
};


#endif //__ESBSERVERMANAGER_H_
