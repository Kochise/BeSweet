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

// ESBServerCtrlPanel.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"


#include "ESBServerCtrlPanel.h"
#include "ESBServer.h"

#include "Properties.h"
#include "Workspace.h"

#include "StringTokenizer.h"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <memory>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// ESBServerControlPanel dialog

string make_name(ESBServer *c)
{
  return c ? file_name_is::nameof(c->getName()) : string("???");
}

ESBServerControlPanel::ESBServerControlPanel(ESBServer *c, CWnd* pParent /*=NULL*/)
: ESBParent(ESBServerControlPanel::IDD, make_name(c).c_str()), mController(c)
{
	//{{AFX_DATA_INIT(ESBServerControlPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ESBSERVER_MANAGER);
  mName = make_name(c);
}

ESBServerControlPanel::~ESBServerControlPanel()
{
//  if(!getController()) {
//    message_box("destroying with null controller");
//  } else {
//    message_box("destroying controller" + getController()->getName());
//    
//  }
}

void ESBServerControlPanel::DoDataExchange(CDataExchange* pDX)
{
	ESBParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ESBServerControlPanel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ESBServerControlPanel, ESBParent)
	//{{AFX_MSG_MAP(ESBServerControlPanel)
	ON_WM_PAINT()
  ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TYPE_BROWSER, onTypeBrowser)
	ON_BN_CLICKED(IDC_OUTLINE_BROWSER, onOutlineBrowser)
	ON_COMMAND(ID_ESBSERVER_REPARSE, onReparseWorkspace)
	ON_BN_CLICKED(IDC_WORKSPACE, onWorkspaceBrowser)
	ON_COMMAND(ID_ESBSERVER_METHODBROWSER, onMethodBrowser)
	ON_BN_CLICKED(IDC_HIERARCHY_BROWSER, onHierarchyBrowser)
	ON_BN_CLICKED(IDC_PACKAGES_BROWSER, onPackagesBrowser)
	ON_COMMAND(ID_ESBSERVER_TYPEBROWSER, onTypeBrowser)
	ON_COMMAND(ID_ESBSERVER_WORKSPACEBROWSER, onWorkspaceBrowser)
	ON_BN_CLICKED(IDC_METHOD_BROWSER, onMethodBrowser)
  ON_BN_CLICKED(IDOK, onExit)
	//}}AFX_MSG_MAP

  ON_MESSAGE(WM_SHOW_FILEOUTLINE, onShowFileOutline)
  ON_MESSAGE(WM_OPEN_WORKSPACE, onOpenWorkspace)
  ON_MESSAGE(WM_REPARSE_FILE, onReparseFile)
  ON_MESSAGE(WM_SHOW_BROWSER, onShowBrowser)
 

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ESBServerControlPanel message handlers

#define TIMER_ID 12345

BOOL ESBServerControlPanel::OnInitDialog()
{
	ESBParent::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog	
  CString name;
  GetCaption(name);
  SetDlgItemText(IDC_MOUSE_POS, name);
  
  //SetTimer(TIMER_ID, 1000, NULL);
	return TRUE;
}



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ESBServerControlPanel::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ESBServerControlPanel::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void ESBServerControlPanel::onTypeBrowser() 
{
 	const static string function = "ESBServerControlPanel::onTypeBrowser(): "; 
  Tracer trc(mName, RENDERER, function);
  
  try {
    getController()->showBrowser("types", "");

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
}

void ESBServerControlPanel::onWorkspaceBrowser() 
{
  static const string function = "ESBServerControlPanel::onWorkspaceBrowser(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    getController()->showBrowser("workspace", "");
    
  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
}

void ESBServerControlPanel::onMethodBrowser() 
{
  static const string function = "ESBServerControlPanel::onMethodBrowser(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    getController()->showBrowser("methods", "");
    
  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
}

void ESBServerControlPanel::onOutlineBrowser() 
{
 	static const string function = "ESBServerControlPanel::onOutlineBrowser(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
	  getController()->showBrowser("outline", "");

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
}

#include "HierarchyBrowser.h"
#include "HierarchyLoaderSimulator.h"
void ESBServerControlPanel::onHierarchyBrowser() 
{
 	static const string function = "ESBServerControlPanel::onHierarchyBrowser(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    /*Simulators::HierarchyLoader hierarchyLoader;
    smart_ptr<Inheritable> hierarchy(new Class("TestClass", ePublic, TagLocation()));
    hierarchy->setExtendors(hierarchyLoader.loadSubtypeHierarchy(*hierarchy));
    hierarchy->setParents(hierarchyLoader.loadSupertypeHierarchy(*hierarchy));
  
    HierarchyBrowser hierarchyBrowser;
    hierarchyBrowser.setModel(hierarchy);
    hierarchyBrowser.show(true);*/
    getController()->showBrowser("hierarchy", "");

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }

}

#include "PackageBrowserRenderer.h"
void ESBServerControlPanel::onPackagesBrowser() 
{
 	static const string function = "ESBServerControlPanel::onPackagesBrowser(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    PackageBrowserRenderer dlg;
    dlg.DoModal();

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
}

LRESULT ESBServerControlPanel::onShowFileOutline(WPARAM wParam, LPARAM lParam)
{
 	static const string function = "ESBServerControlPanel::onShowFileOutline(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    auto_ptr<string> file((string*)lParam);
    getController()->showFileOutline(*file);
    return S_OK;

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }

  return E_FAIL;
}

LRESULT ESBServerControlPanel::onReparseFile(WPARAM wParam, LPARAM lParam)
{
 	static const string function = "ESBServerControlPanel::onReparseFile(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    auto_ptr<string> file((string*)lParam);
    getController()->reparseFile(*file);
    return S_OK;

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
  
  return E_FAIL;
}

LRESULT ESBServerControlPanel::onOpenWorkspace(WPARAM wParam, LPARAM lParam)
{
 	static const string function = "ESBServerControlPanel::onOpenWorkspace(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    getController()->openWorkspace(*auto_ptr<Workspace>((Workspace*)lParam));
    return S_OK;

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }

  return E_FAIL;
}

LRESULT ESBServerControlPanel::onShowBrowser(WPARAM wParam, LPARAM lParam)
{
 	static const string function = "ESBServerControlPanel::onShowBrowser(): "; 
  Tracer trc(mName, RENDERER, function);

  try {
    auto_ptr<pair<string, string> > browser((pair<string, string>*)lParam);
    getController()->showBrowser(browser->first, browser->second);
    return S_OK;

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
  
  return E_FAIL;
}

void ESBServerControlPanel::mouseAt(CPoint &point)
{
	std::stringstream ss;
	ss << point.x << ":" << point.y;
	SetDlgItemText(IDC_MOUSE_POS, ss.str().c_str());
}

void ESBServerControlPanel::onReparseWorkspace() 
{
  static const string function = "ESBServerControlPanel::onReparseWorkspace(): ";
  Tracer trc(mName, RENDERER, function);
  
  try {
    getController()->reparseWorkspace();

  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
}


void ESBServerControlPanel::onExit() 
{
  static const string function = "ESBServerControlPanel::onExit(): "; 
  Tracer trc(mName, RENDERER, function);
  
  try {
    string name = getController()->getName();
    AfxGetApp()->GetMainWnd()->PostMessage(WM_CLOSE_WORKSPACE, 0, reinterpret_cast<LPARAM>(new string(name)));
    
  } catch(std::exception &e) {
    log(mName, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(mName, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }
}

void ESBServerControlPanel::OnTimer(UINT id) 
{
  if(id == TIMER_ID) {
    //message_box("on timer");
    CPoint pos;
    GetCursorPos(&pos);
    CRect topLeft(0,0, 10, 10);
    if(topLeft.PtInRect(pos)) {
      getController()->showBrowser("outline", "");
    }
    
  }
}