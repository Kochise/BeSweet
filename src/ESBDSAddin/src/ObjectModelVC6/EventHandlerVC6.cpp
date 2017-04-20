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
#include "resource.h"
#include <stubs/ESBDSAddin.h>

#include "VSObjectModelHelper.h"

#include "AddinVC6.h"

#include <string>
#include <vector>
#include <iterator>
#include <sstream>

using namespace std;

#define function sbox_class_name+"::"+sbox_method_name

namespace VisualStudio6 {

HRESULT ApplicationEventHandler::DocumentSave(IDispatch* theDocument)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  SANDBOX_BEGIN("ApplicationEventHandler", "DocumentSave")
  Tracer trc(BE_SWEET, EVENT, function);

  GenericDocumentSaved(CComQIPtr<IGenericDocument>(theDocument));

  SANDBOX_END()
    
  return S_OK;
}

void ApplicationEventHandler::GenericDocumentSaved(CComQIPtr<IGenericDocument> &theDocument)
{
  if(is_instance_of<ITextDocument>(theDocument)) {
    getAddIn()->documentChanged(getFullName(theDocument));

  } else {
    log(BE_SWEET, EVENT, eInfo) << "\"" <<  getFullName(theDocument) << "\" not a text document" << endlog;
  }
}

HRESULT ApplicationEventHandler::WindowActivate(IDispatch* theWindow)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  SANDBOX_BEGIN("ApplicationEventHandler", "WindowActivate")
  Tracer trc(BE_SWEET, EVENT, function);

  GenericWindowActivated(CComQIPtr<IGenericWindow>(theWindow));
 

  SANDBOX_END()
    
  return S_OK;
}

void ApplicationEventHandler::GenericWindowActivated(CComQIPtr<IGenericWindow> &theWindow)
{
  Tracer trc(BE_SWEET, EVENT, "ApplicationEventHandler::GenericWindowActivated");
  log(BE_SWEET, EVENT, eInfo) << "ApplicationEventHandler::GenericWindowActivated() " << getCaption(theWindow) << endlog;
    
  if(is_instance_of<ITextDocument>(getDocument<IGenericDocument>(theWindow))) {
    getAddIn()->documentActivated(getFullName(getDocument<IGenericDocument>(theWindow)));

  } else {
    log(BE_SWEET, EVENT, eInfo) << "\"" <<  getCaption(theWindow) << "\" not a text-editor" << endlog;
  }     
}

vector<string> findFile(string file) 
{
  vector<string> result;
  
  WIN32_FIND_DATA rWFD;
  HANDLE hFF= FindFirstFile( file.c_str(), &rWFD );
  if ( hFF == INVALID_HANDLE_VALUE ) {
    return result;
  }
  
  do {
    result.push_back(rWFD.cFileName);
  } while ( FindNextFile( hFF, &rWFD) );
  
  FindClose( hFF );
  return result;
}

HRESULT ApplicationEventHandler::WorkspaceOpen()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  SANDBOX_BEGIN("ApplicationEventHandler", "WorkspaceOpen");
  Tracer trc(BE_SWEET, EVENT, function);

  BSTR bstr;
  cex_ = getAddIn()->getApplication()->get_CurrentDirectory(&bstr);
  string cwd = bstr_t(bstr);

  vector<string> wspaces(findFile("*.dsw"));
  string workspace;

  if(wspaces.size() > 1) {
    workspace = selectWorkspace(wspaces, cwd);
    
  } else if(wspaces.empty()) {
    workspace = getWorkspace(cwd);
    
  } else {
    workspace = cwd + "\\" + wspaces.front();
  }

  WorkspaceOpened(workspace);

  SANDBOX_END()

  return S_OK;
}

void ApplicationEventHandler::WorkspaceOpened(const std::string &workspace)
{
  CComPtr<IDispatch> tmp;
  cex_ = getAddIn()->getApplication()->get_Projects(&tmp.p);
  CComQIPtr<IProjects> projects(tmp);

  long cnt;
  cex_ = projects->get_Count(&cnt);

  list<string> projectNames;
  for(int i = 1; i <= cnt; i++) {
    CComPtr<IGenericProject> project;
    cex_ = projects->Item(CComVariant(i), &project.p);

    projectNames.push_back(getFullName(project));
  }
  
  getAddIn()->loadWorkspace(workspace, projectNames);
}

HRESULT ApplicationEventHandler::WorkspaceClose()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  SANDBOX_BEGIN("ApplicationEventHandler", "WorkspaceClose")
  Tracer trc(BE_SWEET, EVENT, function);

	getAddIn()->closeWorkspace();

  SANDBOX_END()
    
  return S_OK;
}

static char filter[] = "VC6 Workspaces (*.dsw)|*.dsw|VC7 Workspaces (*.sln)|*.sln|All Files (*.*)|*.*||";

std::string ApplicationEventHandler::selectWorkspace(const std::vector<std::string> &workspaces, const std::string &cwd)
{
  stringstream ss;
  ss << "found several .dsw files in directory " << cwd << endl << endl;
  copy(workspaces.begin(), workspaces.end(), ostream_iterator<string>(ss, "\n"));
  //ss << endl << endl << "please select open " << workspaces.front();
  
  ::MessageBox(0, ss.str().c_str(), "Be Sweet", MB_OK);
  
  CFileDialog dlg(true, NULL, (cwd + "\\" + workspaces.front()).c_str(), 0, filter);
  return dlg.DoModal() == IDOK ? dlg.GetPathName().GetBuffer(0) : "";
}

std::string ApplicationEventHandler::getWorkspace(const std::string &cwd)
{
  ::MessageBox(0, ("no workspace file (*.dsw) found in directory\n" + cwd).c_str(), "Be Sweet Addin", MB_OK | MB_ICONERROR | MB_TOPMOST);
  CFileDialog dlg(true, NULL, (cwd + "\\*.dsw").c_str(), 0, filter);
  
  return dlg.DoModal() == IDOK ? dlg.GetPathName().GetBuffer(0) : "";
}

} // namespace VisualStudio6