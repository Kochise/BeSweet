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

// ESBAddInSimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ESBAddInSimulator.h"
#include "ESBAddInSimulatorDlg.h"
#include "ESBDSAddInImplX.h"

#include <Utilities/StringTokenizer.h>

#include <Model/Workspace.h>
#include <Model/VSWorkspaceLoader.h>

#include <stubs/ESBServer.h>
#include <stubs/ESBServer_i.c>
#include <stubs/ESBDSAddin_i.c>

//#include <atlbase.h>

#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char *WORKSPACE_NAME = "AddinSimulator";

CComPtr<IESBDSAddin> addin;
COMExceptionThrower cex_;

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// ESBAddInSimulatorDlg dialog

ESBAddInSimulatorDlg::ESBAddInSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ESBAddInSimulatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ESBAddInSimulatorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ESBAddInSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ESBAddInSimulatorDlg)
	DDX_Control(pDX, IDC_WORKSPACE, mTree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ESBAddInSimulatorDlg, CDialog)
	//{{AFX_MSG_MAP(ESBAddInSimulatorDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_WM_CONTEXTMENU()
  ON_WM_DROPFILES()
  ON_NOTIFY(NM_CLICK, IDC_WORKSPACE, onFileSelected)
	ON_COMMAND(ID_REPARSE, onReparseFile)
	ON_COMMAND(ID_SHOW, onShowFileOutline)
	ON_COMMAND(ID_XXX_LOADWORKSPACE, onLoadworkspace)
	ON_COMMAND(ID_XXX_RAISEOUTLINEBROWSER, onRaiseOutlineBrowser)
	ON_COMMAND(ID_XXX_RAISEMETHODBROWSER, onRaiseMethodBrowser)
	ON_COMMAND(ID_XXX_RAISETYPEBROWSER, onRaiseTypebrowser)
	ON_COMMAND(ID_XXX_RAISEWORKSPACEBROWSER, onRaiseWorkspacebrowser)
	ON_COMMAND(ID_XXX_RAISEHIERARCHYBROWSER, onRaiseHierarchyBrowser)
	ON_COMMAND(ID_XXX_CLOSEWORKSPACE, onCloseWorkspace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ESBAddInSimulatorDlg message handlers

BOOL ESBAddInSimulatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	mTree.InsertItem("drop *.dsp or *.vcproj file here", 0,0);

  cex_ = addin.CoCreateInstance(CLSID_ESBDSAddin);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void ESBAddInSimulatorDlg::OnPaint() 
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

HCURSOR ESBAddInSimulatorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

CComPtr<IESBServerManager> getWorkspaceManager()
{
  CComPtr<IESBServerManager> mgr;
  cex_ = mgr.CoCreateInstance(CLSID_ESBServerManagerCoClass); 
  return mgr;  
}

CComPtr<IESBServer> getWorkspaceServer()
{ 
  
  CComPtr<IESBServer> workspaceServer;  
  try {
    CComPtr<IESBServerManager> mgr = getWorkspaceManager();
    cex_ = mgr->getWorkspaceServer(CComBSTR(WORKSPACE_NAME), &workspaceServer);
  
  } catch(_com_error &e) {
    AfxMessageBox(("getWorkspaceServer():" + asString(e)).c_str());
    cex_ = getWorkspaceManager()->newWorkspaceServer(CComBSTR(WORKSPACE_NAME), addin, &workspaceServer);
  }
  
  return workspaceServer;
}

void ESBAddInSimulatorDlg::onFileSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW *tree = (NMTREEVIEW*)pNMHDR;
  
	*pResult = 0;
  CPoint p = GetMessagePos();
  mTree.ScreenToClient(&p);

  UINT htFlags = 0;
  HTREEITEM item = mTree.HitTest(p, &htFlags);   
  
  if(item == NULL || mTree.GetItemData(item) == NULL) {
    return;
  }

  string file = (const char*)mTree.GetItemData(item);
  if(file.rfind(".cpp") != string::npos || file.rfind(".h") != string::npos) {
    mSelectedFile = file;
    onShowFileOutline();
  }
}


string extractFileName(const std::string &full)
{
	size_t pos = full.rfind("\\");
	if(pos == string::npos) {
		return full;
	} 

	return full.substr(pos+1, string::npos);
}

void ESBAddInSimulatorDlg::openWorkspace(const pair<string/*name*/, string/*projects*/> &wspace)
{

  try {    
    mWorkspace = wspace;
    list<string> projects = StringListTokenizer(mWorkspace.second, ";").tokens();
    static Workspace ws;
    ws = *VisualStudioWorkspaceLoader(mWorkspace.first).load(projects);

    // TODO: Add extra initialization here
    mTree.SetRedraw(false);
    mTree.DeleteAllItems();
    for(list<Package>::iterator package = ws.getPackages().begin(); package != ws.getPackages().end(); package++) {
  
	    HTREEITEM parent = mTree.InsertItem(extractFileName(package->getName()).c_str());
      mTree.SetItemData(parent, 0);
      
      for(Package::FileSet::iterator file = package->getFiles().begin(); file != package->getFiles().end(); file++) {
        HTREEITEM item = mTree.InsertItem(extractFileName(*file).c_str(), parent);
        mTree.SetItemData(item, (DWORD)file->c_str());
      }
      mTree.Expand(parent,TVE_EXPAND);	  
    }
    
    mTree.SetRedraw(true);
    
    if(!mWorkspace.first.empty()) {
      cex_ = getWorkspaceManager()->removeWorkspaceServer(CComBSTR(mWorkspace.first.c_str()));
    }
    getWorkspaceServer()->loadWorkspace(CComBSTR(mWorkspace.second.c_str()));
    
  } catch(std::exception &e) {
    AfxMessageBox(e.what());
  } catch(...) {
    AfxMessageBox("could not open workspace");
  }

}

void ESBAddInSimulatorDlg::closeWorkspace()
{
  getWorkspaceManager()->removeWorkspaceServer(CComBSTR(WORKSPACE_NAME));
}

void ESBAddInSimulatorDlg::OnContextMenu(CWnd* pWnd, CPoint ptMousePos) 
{
  mTree.ScreenToClient(&ptMousePos);

	UINT uFlags;
	HTREEITEM htItem = mTree.HitTest( ptMousePos, &uFlags );

	if( htItem == NULL || mTree.GetItemData(htItem) == NULL)
		return;
	
	mSelectedFile = (const char*)mTree.GetItemData(htItem);
  SetWindowText(("ESB-Faker: "+mSelectedFile.substr(mSelectedFile.rfind("\\")+1, string::npos)).c_str());
  mTree.Select(htItem, TVGN_CARET);

	CMenu menu;
	menu.LoadMenu(IDR_MENU);
	CMenu *pPopup = menu.GetSubMenu(0);
  
	mTree.ClientToScreen(&ptMousePos);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );	
}

void ESBAddInSimulatorDlg::onReparseFile() 
{
  try {
    getWorkspaceServer()->fileContentChanged( CComBSTR(mSelectedFile.c_str()));

  } catch(...) { AfxMessageBox("OnReparseFile(): failure"); }
}

void ESBAddInSimulatorDlg::onShowFileOutline() 
{
  try {
    getWorkspaceServer()->showFileOutline( CComBSTR(mSelectedFile.c_str()));

  } catch(...) { AfxMessageBox("OnShowFileOutline(): failure"); }
}

void ESBAddInSimulatorDlg::onLoadworkspace() 
{
  try {
    openWorkspace(mWorkspace);

  } catch(...) { AfxMessageBox("OnLoadWorkspace(): failure"); }
}

void ESBAddInSimulatorDlg::OnDropFiles(HDROP hDropInfo )
{
  // Get the number of pathnames that have been dropped
  unsigned int uiNFilesDropped = DragQueryFile( hDropInfo, -1, NULL, 0 );

  char buffer[1024];
  string file;
  stringstream ss;

  for( unsigned int i=0; i<uiNFilesDropped; ++i )
  {
    DragQueryFile(hDropInfo, i, buffer, sizeof(buffer) );
    file = buffer; 
    if(file.find(".dsp") == file.size()-4 || file.find(".vcproj") == file.size()-7) {
      ss << buffer << ";";
    }
  }

  // Free the memory block containing the dropped-file
  // information
  DragFinish( hDropInfo );

  if(!ss.str().empty()) {
    openWorkspace(make_pair(WORKSPACE_NAME, ss.str()));
  }
}

void ESBAddInSimulatorDlg::onRaiseOutlineBrowser() 
{
  showBrowser("outline");
}

void ESBAddInSimulatorDlg::onRaiseHierarchyBrowser() 
{
  showBrowser("hierarchy");
}

void ESBAddInSimulatorDlg::onRaiseMethodBrowser() 
{
  showBrowser("methods");
}

void ESBAddInSimulatorDlg::onRaiseTypebrowser() 
{
  showBrowser("types");
}

void ESBAddInSimulatorDlg::onRaiseWorkspacebrowser() 
{
	showBrowser("workspace");
}

void ESBAddInSimulatorDlg::showBrowser(const std::string &browser)
{
  try {
    getWorkspaceServer()->showBrowser(CComBSTR(browser.c_str()), CComBSTR(""));

  } catch(...) { 
    AfxMessageBox(("showBrowser(" + browser + "): failure").c_str()); 
  }
}

void ESBAddInSimulatorDlg::onCloseWorkspace() 
{
  try {
    closeWorkspace();
    
  } catch(...) { 
    AfxMessageBox("closeWorkspace(): failure"); 
  }
}
