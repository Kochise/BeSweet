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
#include "afxinet.h"

#include "ESBServerManagerRenderer.h"

#include <Renderer/MFC/3rdParty/Hyperlink.h>

#include <Renderer/MFC/3rdParty/OptionPage/OptionSheet.h>
#include <Renderer/MFC/3rdParty/OptionPage/OptionPage.h>
#include <Renderer/MFC/3rdParty/OptionPage/OptionPageStaticText.h>

#include <Renderer/MFC/ESBServerCtrlPanel.h>

#include <Renderer/MFC/Properties/AppearancePropertyPanel.h>
#include <Renderer/MFC/Properties/CTagsPropertyPanel.h>
#include <Renderer/MFC/Properties/OutlinePropertyPanel.h>
#include <Renderer/MFC/Properties/HierarchyPropertyPanel.h>
#include <Renderer/MFC/Properties/LoggerPropertiesPanel.h>

#include <Renderer/MFC/Properties/PropertiesTree.h>
#include <Renderer/MFC/VersionCheckPanel.h>

#include <Renderer/UIProperties.h>

#include <Utilities/Properties.h>

#include <string>
#include <vector>
#include <sstream>

#ifndef HTTP_VERB_GET
 const short HTTP_VERB_GET = 1;
#endif

using namespace std;

class AboutDlg : public COptionPage
{
public:
  AboutDlg(bool hasOkButton = true);

  std::string version() 
  {
    if(mVersion.empty()) {
      mVersion = VersionCheckPanel().fetchVersion();
    }
    return mVersion;
  }
  std::string latestVersion()
  {
    if(mLatestVersion.empty() && PropertiesManager::instance().getProperty(BeSweet::CHECK_4_NEW_VERSION, "1") == "1") {
      mLatestVersion = VersionCheckPanel().fetchLatestVersion().first;
    }
    return mLatestVersion;
  }

  void OnOK() { CDialog::OnOK(); }
  void OnCancel() { CDialog::OnCancel(); }

  //{{AFX_DATA(AboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	mDownloadPage;
  CHyperLink mHomePage;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(AboutDlg)
  afx_msg BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  bool isOnlineCheckAllowed() 
  { return PropertiesManager::instance().getProperty(BeSweet::CHECK_4_NEW_VERSION, "1") == "1"; }

  
  bool mHasOkButton;
  string mVersion;
  string mLatestVersion;
  
};

AboutDlg::AboutDlg(bool hasOkButton)
 : COptionPage(AboutDlg::IDD), mHasOkButton(hasOkButton)
{
  //{{AFX_DATA_INIT(AboutDlg)
	//}}AFX_DATA_INIT
  mHomePage.SetURL("http://esbrowsers.sourceforge.net");
  mDownloadPage.SetURL("http://esbrowsers.sourceforge.net/downloads.shtml");
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AboutDlg)
	DDX_Control(pDX, IDC_DOWNLOADPAGE, mDownloadPage);
	DDX_Control(pDX, IDC_HOMEPAGE, mHomePage);
	//}}AFX_DATA_MAP
}

BOOL AboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

  if(!mHasOkButton) {
    GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
  }
  SetDlgItemText(IDC_THIS_VERSION, ("Version : " + version()).c_str());
  SetDlgItemText(IDC_LATEST_VERSION, ("Latest version : " + latestVersion()).c_str());

  return TRUE;
}


BEGIN_MESSAGE_MAP(AboutDlg, COptionPage)
	//{{AFX_MSG_MAP(AboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


ESBServerManagerRenderer::ESBServerManagerRenderer()
{
	SetListControl(&mListbox);

  //thats a real hack here !!! we are going to remove the dummy page 
  //in OnInitDialog()
  AddPage(new ESBServerControlPanel(NULL));
}

BOOL ESBServerManagerRenderer::OnInitDialog()
{

  COptionSheet::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, "About BeSweet");
		}
	}
  
  //http://www.codeproject.com/shell/systemtray.asp
  HICON icon = AfxGetApp()->LoadIcon(IDI_ESBSERVER_MANAGER);
  getSystemTray().Create(this, WM_SYSTRAY_NOTIFY, "ESB-Server", icon, IDR_ESBSERVER_POPUPMENU);
  getSystemTray().SetIcon(icon);
  
  #ifndef DEBUG
    onMinimize();
  #endif

  SetIcon(icon, TRUE);			// Set big icon
	SetIcon(icon, FALSE);		// Set small icon

  SetWindowText("Be Sweet Control Panel");

  SetDlgItemText(IDCANCEL, "Minimize");
  SetDlgItemText(IDOK, "Exit");
  GetDlgItem(ID_APPLY_NOW)->ModifyStyle(WS_VISIBLE, 0,0);

  //thats a real hack here !!! we remove the dummy page inserter
  //in Constructor() 
  if(GetPageCount() != 0) {
    COptionPage *page = GetPage(0);
    RemovePage(0);
    delete page;
  }
  
  mLoggerPanel = smart_ptr<LoggerPanel>(new LoggerPanel());
  mLoggerPanel->Create(LoggerPanel::IDD, this);
  smart_ptr<LoggerImpl> logger(new LoggerImpl(GENERAL));
  logger->registerConsumer(mLoggerPanel);
#ifdef DEBUG
  logger->logLevel(Logging::eTrace);
#else
  logger->logLevel(Logging::eInfo);
#endif

  LoggerManager::instance().map(GENERAL, logger);
  LoggerManager::instance().map(NO_LABEL, GENERAL);

  return TRUE;
}

void ESBServerManagerRenderer::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		onAbout();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

LRESULT ESBServerManagerRenderer::onSystemTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// Delegate all the work back to the default implementation in CSystemTray
	return getSystemTray().OnTrayNotification(wParam, lParam);
}

void ESBServerManagerRenderer::onAbout() 
{
	AboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void ESBServerManagerRenderer::onMaximize() 
{
  getSystemTray().MaximiseFromTray(this);
}

void ESBServerManagerRenderer::onMinimize() 
{
	getSystemTray().MinimiseToTray(this);
}

void ESBServerManagerRenderer::OnCancel()
{
  onMinimize();
}

void ESBServerManagerRenderer::onExit() 
{
  OnOK();
}

void ESBServerManagerRenderer::onPreferences() 
{
  COptionSheet          optionSheet;
  CTags::PropertyPanel  ctags;

  AboutDlg browserGroup(false);

  AppearancePropertyPanel appearance;
  Browsers::OutlinePropertyPanel	outline;
  Browsers::HierarchyPropertyPanel	hierarchy;
  LoggerPropertiesPanel	logger;
  
  PropertiesTree tree;
	optionSheet.SetListControl(&tree);
	optionSheet.AddGroup(&browserGroup);
	optionSheet.AddPage(&ctags);
	optionSheet.AddPage(&logger);
  optionSheet.AddPage(&appearance,&browserGroup);
  optionSheet.AddPage(&outline,&browserGroup);
  optionSheet.AddPage(&hierarchy,&browserGroup);
  
	optionSheet.DoModal();
}

#include <RemoteInterface/COM/stubs/IESBServer.h>
#include <ComHelper.h>
LRESULT ESBServerManagerRenderer::onCloseWorkspace(WPARAM wParam, LPARAM lParam)
{
  const static string function = "ESBServerManagerRenderer::onCloseWorkspace(): ";
  try {
    CComPtr<IESBServerManager> mgr;
    cex_ = mgr.CoCreateInstance(CLSID_ESBServerManagerCoClass);
    auto_ptr<string> name(reinterpret_cast<string*>(lParam));
    cex_ = mgr->removeWorkspaceServer(CComBSTR(name->c_str()));
    
  } catch(std::exception &e) {
    log(GENERAL, RENDERER, eError) << function << e << endlog;

  } catch(...) {
    log(GENERAL, RENDERER, eError) << function << "unknown exception occurred" << endlog;
  }

  return S_OK;
}

void ESBServerManagerRenderer::onLogWindow()
{
  mLoggerPanel->ShowWindow(SW_SHOW);
  mLoggerPanel->SetWindowPos(&wndTop, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
}

BEGIN_MESSAGE_MAP(ESBServerManagerRenderer, COptionSheet)
	ON_WM_SYSCOMMAND()
  ON_MESSAGE(WM_SYSTRAY_NOTIFY, onSystemTrayNotification)  
  ON_MESSAGE(WM_CLOSE_WORKSPACE, onCloseWorkspace)  
  ON_COMMAND(ID_ESBSERVER_ABOUT, onAbout)
  ON_COMMAND(ID_ESBSERVER_MAXIMIZE, onMaximize)
	ON_COMMAND(ID_ESBSERVER_MINIMIZE, onMinimize)
  ON_COMMAND(ID_ESBSERVER_EXIT, OnOK)
	ON_COMMAND(ID_ESBSERVER_PREFERENCES, onPreferences)
  ON_COMMAND(ID_ESBSERVER_LOGWINDOW, onLogWindow)
END_MESSAGE_MAP()
