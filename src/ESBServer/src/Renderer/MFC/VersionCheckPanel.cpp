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
#include "afxinet.h"
#include "resource.h"
#include "VersionCheckPanel.h"

#include <Renderer/UIProperties.h>
#include <Utilities/Properties.h>
#include <Utilities/StringTokenizer.h>
#include <Utilities/stl_ext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef HTTP_VERB_GET
 const short HTTP_VERB_GET = 1;
#endif

#include <vector>

using namespace std;


VersionCheckPanel::VersionCheckPanel(std::pair<std::string, std::string> version)
	: CDialog(VersionCheckPanel::IDD, NULL), mLatestVersion(version)
{
	//{{AFX_DATA_INIT(VersionCheckPanel)
	mIsEnabled = FALSE;
	//}}AFX_DATA_INIT
}


void VersionCheckPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(VersionCheckPanel)
	DDX_Control(pDX, IDC_PROGRESS, mProgressbar);
	DDX_Check(pDX, IDC_DISABLE, mIsEnabled);
	DDX_Control(pDX, IDC_LINK, mDownloadPage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(VersionCheckPanel, CDialog)
	//{{AFX_MSG_MAP(VersionCheckPanel)
	ON_BN_CLICKED(IDC_DISABLE, onDisableOnlineCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL VersionCheckPanel::OnInitDialog()
{
  super::OnInitDialog();

  if(!mLatestVersion.first.empty()) {
    GetDlgItem(IDC_PROGRESS)->ModifyStyle(WS_VISIBLE, 0, 0);
    GetDlgItem(IDC_DISABLE)->ModifyStyle(WS_VISIBLE, 0, 0);
  
    mDownloadPage.SetWindowText(mLatestVersion.first.c_str());
    mDownloadPage.SetURL(mLatestVersion.second.c_str());
    
    GetDlgItem(IDOK)->ModifyStyle(0, WS_VISIBLE, 0);
    GetDlgItem(IDC_PREFIX)->ModifyStyle(0, WS_VISIBLE, 0);
    GetDlgItem(IDC_LINK)->ModifyStyle(0, WS_VISIBLE, 0);
    GetDlgItem(IDC_POSTFIX)->ModifyStyle(0, WS_VISIBLE, 0);

  } else {
    mProgressbar.SetRange(0,100);
  }
  return TRUE;
}

void VersionCheckPanel::run()
{
  UpdateWindow();
  vector<string> current = StringVectorTokenizer(fetchVersion(), ".").tokens();
  
  pair<string, string> version = fetchLatestVersion(true);
  vector<string> latest = StringVectorTokenizer(version.first, ".").tokens();

  if(current.size() == latest.size()) {
    for(size_t i = 0; i < latest.size(); ++i) {
      if(lexical_cast<int>(latest[i]) > lexical_cast<int>(current[i])){
        VersionCheckPanel dlg(version);
        dlg.DoModal();
        return;
      }
    }
  }
  
}

std::string VersionCheckPanel::fetchVersion()
{
  string version;
  
  try {
 
    vector<char> buf(1024);
    GetModuleFileName(NULL, &buf[0], buf.size());
    string exe = &buf[0];

    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSize((char*)exe.c_str(), &handle); 
    buf.resize(size);
    GetFileVersionInfo((char*)exe.c_str(), handle, size, &buf[0]); 

    VS_FIXEDFILEINFO *info;
    UINT versionLength;
    VerQueryValue(&buf[0],_T("\\"),(void**)&info,(UINT *)&versionLength);  
	  stringstream ss;
    ss << HIWORD (info->dwFileVersionMS) << "." << LOWORD (info->dwFileVersionMS) << "."
		   << HIWORD (info->dwFileVersionLS) << "." << LOWORD (info->dwFileVersionLS);
  
    version = ss.str();

  } catch(...) {
  }

  return version;
}

pair<string,string> VersionCheckPanel::fetchLatestVersion(bool showProgress)
{
  const static char function[] = "VersionCheckPanel::fetchLatestVersion(): ";
    
  try {
    DWORD flags;
    if(InternetGetConnectedState(&flags, 0)) {
      CInternetSession session;
      CHttpConnection *http = session.GetHttpConnection("esbrowsers.sourceforge.net");
      CHttpFile *request = http->OpenRequest(HTTP_VERB_GET, (
                                                        "/version.shtml?version=" + fetchVersion() + "." +
                                                        PropertiesManager::instance().getProperty(BeSweet::INSTALL_TIME, "0") + "." +
                                                        PropertiesManager::instance().getProperty(BeSweet::START_COUNTER, "1")
                                                        ).c_str());
      
      if(showProgress)mProgressbar.SetPos(30);
      request->SendRequest();
      
      if(showProgress)mProgressbar.SetPos(60);
      vector<char> buffer(1024);
      request->Read(&buffer[0], buffer.size());

      string version = &buffer[0];
      size_t start = version.find("[");
      size_t end = version.find("]", start);
      version = (start != string::npos && end != string::npos)  ? version.substr(start+1, end-start-1) : "";
      
      string url = &buffer[0];
      start = url.find("[url=");
      end = url.find("]", start);
      url = (start != string::npos && end != string::npos)  ? url.substr(start+1, end-start-1) : "http://osdn.dl.sourceforge.net/sourceforge/esbrowsers/besweet-latest-greatest.zip";
      
      if(showProgress)mProgressbar.SetPos(100);
      return make_pair(version, url);
    }

  } catch(CInternetException &e) {
    vector<char> buffer(1000);
    e.GetErrorMessage(&buffer[0], buffer.size());
    log(GENERAL, RENDERER, eError) << function << " CInternetException " << endl << &buffer[0] << endlog;

  } catch(std::exception &e) {
    log(GENERAL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(GENERAL, RENDERER, eError) << function << "unknown exception occurred while checking for new verions" << endlog;
  }
  
  return make_pair(string(), string());
}

void VersionCheckPanel::onDisableOnlineCheck() 
{
  PropertiesManager::instance().setProperty(BeSweet::CHECK_4_NEW_VERSION, lexical_cast<string>(mIsEnabled));
  PropertiesManager::instance().dump();
}
