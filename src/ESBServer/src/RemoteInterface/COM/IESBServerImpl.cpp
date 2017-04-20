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
#include <comutil.h>

#include <RemoteInterface/COM/stubs/IESBServer.h>
#include <RemoteInterface/COM/IESBServerImpl.h>

#include <Controller/ESBServer.h>
#include <Utilities/StringTokenizer.h>
#include <Utilities/stl_ext.h>

#include <ComHelper.h>

/////////////////////////////////////////////////////////////////////////////
// ESBServer



#include <map>
#include <string>
using namespace std;


STDMETHODIMP IESBServerImpl::fileContentChanged(BSTR file)
{
  Tracer trc(getServer()->getName(), REMOTE_SRV, "IESBServerImpl::fileContentChanged");
  log(getServer()->getShortName(), REMOTE_SRV, eInfo) << "reparsing file " << file << endlog;

  try {
	  //AFX_MANAGE_STATE(AfxGetStaticModuleState())
    //getServer()->reparseFile(to_lower(asString(file)));
    emit(WM_REPARSE_FILE, to_lower(asString(file)), getServer()->getRenderer());
    return S_OK;
	
  } catch(std::exception &e) {
    //message_box("IESBServerImpl::showFileOutline(): " + string(e.what()));
    log(GENERAL, REMOTE_MGR, eError) << "IESBServerImpl::showFileOutline(): \n" << e.what() << endlog;
  }
  return E_FAIL;
}

STDMETHODIMP IESBServerImpl::showFileOutline(BSTR file)
{
  Tracer trc(getServer()->getName(), REMOTE_SRV, "IESBServerImpl::showFileOutline");
  log(getServer()->getShortName(), REMOTE_SRV, eInfo) << "showing file outline" << file << endlog;

  try {
	  //AFX_MANAGE_STATE(AfxGetStaticModuleState())
    //getServer()->showFileOutline(to_lower(asString(file)));
    emit(WM_SHOW_FILEOUTLINE, to_lower(asString(file)), getServer()->getRenderer());
    return S_OK;

  } catch(std::exception &e) {
    //message_box("IESBServerImpl::showFileOutline(): " + string(e.what()));
    log(GENERAL, REMOTE_MGR, eError) << "IESBServerImpl::showFileOutline(): \n" << e.what() << endlog;
  }
  return E_FAIL;
}

STDMETHODIMP IESBServerImpl::loadWorkspace(BSTR pkgs)
{
  Tracer trc(getServer()->getName(), REMOTE_SRV, "IESBServerImpl::loadWorkspace");
  log(getServer()->getShortName(), REMOTE_SRV, eInfo) << "loading workspace " << pkgs << endlog;
  //AFX_MANAGE_STATE(AfxGetStaticModuleState())

  try {
    StringListTokenizer tokenizer(asString(pkgs), ";");
    list<Package> packages;
    transform(tokenizer.tokens().begin(), tokenizer.tokens().end(), inserter(packages, packages.begin()), ptr_fun(Package::create));
    
    //getServer()->openWorkspace(Workspace(getServer()->getName(), packages));
    emit(WM_OPEN_WORKSPACE, Workspace(getServer()->getName(), packages), getServer()->getRenderer());
    return S_OK;

  } catch(std::exception &e) {
    //message_box("IESBServerImpl::loadWorkspace(): " + string(e.what()));
    log(GENERAL, REMOTE_MGR, eError) << "IESBServerImpl::loadWorkspace(): \n" << e.what() << endlog;
  }

	return E_FAIL;
}

STDMETHODIMP IESBServerImpl::showBrowser(BSTR browser, BSTR what)
{
  Tracer trc(getServer()->getName(), REMOTE_SRV, "IESBServerImpl::showBrowser");
  log(getServer()->getShortName(), REMOTE_SRV, eInfo) << "showing browser <" << browser << "><" << what << ">" << endlog;

  //AFX_MANAGE_STATE(AfxGetStaticModuleState())
  try {
    //getServer()->showBrowser(asString(browser), asString(what));
    emit(WM_SHOW_BROWSER, make_pair(asString(browser), asString(what)), getServer()->getRenderer());
    
    return S_OK;

  } catch(std::exception &e) {
    //message_box("IESBServerImpl::showBrowser(): " + string(e.what()));
    log(GENERAL, REMOTE_MGR, eError) << "IESBServerImpl::showBrowser(): \n" << e.what() << endlog;
  }

	return E_FAIL;	
}
