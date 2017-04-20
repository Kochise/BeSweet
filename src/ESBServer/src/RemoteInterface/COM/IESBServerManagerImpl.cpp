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
#include "resource.h"       // main symbols

#include "ESBServerComModule.h"
extern ESBServerComModule _Module;
#include <atlcom.h>

#include <RemoteInterface/COM/stubs/IESBServer.h>
#include <RemoteInterface/COM/IESBServerManagerImpl.h>
#include <RemoteInterface/COM/IESBServerImpl.h>
#include <RemoteInterface/COM/VSEditor.h>
#include <Controller/ESBServerManager.h>

#include <ComHelper.h>

COMExceptionThrower cex_;

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// IESBServerManagerImpl

using namespace std;

STDMETHODIMP IESBServerManagerImpl::newWorkspaceServer(BSTR n, IESBDSAddin *vs, IESBServer **srv)
{
  Tracer trc(GENERAL, REMOTE_MGR, "IESBServerManagerImpl::newWorkspaceServer");
  log(GENERAL, REMOTE_MGR, eInfo) << "creating server " << n << endlog;

  try {
    //AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    string name(asString(n));
    ContainerType::iterator server = getServer(n);
    if(server != mWorkspaceServers.end()) {
      throw runtime_error("workspace <" + name + "> already exists !");
      //mWorkspaceServers.erase(server);
    }

    //message_box(("creating workspace <" + name + ">").c_str());
    ESBServer *real = &ESBServerManager::instance().newWorkspaceServer(name);
    real->setEditorBroker(make_smart_ptr(new VisualStudioEditorBroker(CComPtr<IESBDSAddin>(vs))));

    CComQIPtr<IESBServer> comServer;
    cex_ = comServer.CoCreateInstance(CLSID_ESBServerCoClass);
    dynamic_cast<IESBServerImpl*>(comServer.p)->setServer(real);
    server = mWorkspaceServers.insert(server, make_pair(name, comServer));

    *srv = server->second;
    (*srv)->AddRef();
    return S_OK;
    
  } catch(std::exception &e) {
    log(GENERAL, REMOTE_MGR, eError) << "IESBServerManagerImpl::newWorkspaceServer(): \n" << e.what() << endlog;
  }
	return E_FAIL;
}

STDMETHODIMP IESBServerManagerImpl::getWorkspaceServer(BSTR n, IESBServer **srv)
{
  Tracer trc(GENERAL, REMOTE_MGR, "IESBServerManagerImpl::getWorkspaceServer");
  log(GENERAL, REMOTE_MGR, eInfo) << "quering for server '" << n << "'...." << endlog;
  
  try {
    ContainerType::iterator server = getServer(n);
    if(server != mWorkspaceServers.end()) {
      *srv = server->second;
      (*srv)->AddRef();
      return S_OK;
    }
    
    log(GENERAL, REMOTE_MGR, eInfo) << "...server '" << n << "' doesn't exist" << endlog;
  
  } catch(std::exception &e) {
    log(GENERAL, REMOTE_MGR, eError) << "IESBServerManagerImpl::getWorkspaceServer(): \n" << e.what() << endlog;
  }
  return E_FAIL;
}

STDMETHODIMP IESBServerManagerImpl::removeWorkspaceServer(BSTR n)
{
  Tracer trc(GENERAL, REMOTE_MGR, "IESBServerManagerImpl::removeWorkspaceServer");
  log(GENERAL, REMOTE_MGR, eInfo) << "removing server " << n << endlog;
  
  try {
    mWorkspaceServers.erase(asString(n));
    ESBServerManager::instance().removeWorkspaceServer(asString(n));
    return S_OK;
    
  } catch(std::exception &e) {
    log(GENERAL, REMOTE_MGR, eError) << "IESBServerManagerImpl::removeWorkspaceServer(): \n" << e.what() << endlog;
  }
  return E_FAIL;
}

IESBServerManagerImpl::ContainerType::iterator IESBServerManagerImpl::getServer(const BSTR &n)
{
  return mWorkspaceServers.find(asString(n));
}
