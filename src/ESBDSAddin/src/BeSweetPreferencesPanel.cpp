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

// BeSweetPreferencesPanel.cpp : implementation file
//

#include "stdafx.h"
#include "BeSweetPreferencesPanel.h"

#include <Utilities/StringTokenizer.h>
#include <Utilities/stl_ext.h>

#include <list>
#include <sstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BeSweetPreferencesPanel dialog

const char BESWEET_REG_KEY[] = "software\\Ciderware\\BeSweet\\vsaddin";
const char ENABLE_REG_VALUE_KEY[] = "enabled";
const char SILENT_REG_VALUE_KEY[] = "silent";
const char LOGLEVEL_REG_VALUE_KEY[] = "loglevel";
const char SERVER_EXE_REG_VALUE_KEY[] = "serverexe";

BeSweetPreferencesPanel::BeSweetPreferencesPanel(CWnd* pParent /*=NULL*/)
	: CDialog(BeSweetPreferencesPanel::IDD, pParent)
{
  //{{AFX_DATA_INIT(BeSweetPreferencesPanel)
	//}}AFX_DATA_INIT

  if(CRegKey().Open(HKEY_CURRENT_USER, BESWEET_REG_KEY) != ERROR_SUCCESS ){
    createProperties();
  }

  loadProperties(); 
}


void BeSweetPreferencesPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BeSweetPreferencesPanel)
	DDX_Control(pDX, IDC_WORKSPACE, mWorkspaceCtrl);
  DDX_Control(pDX, IDC_AREA, mAreas);
	DDX_Check(pDX, IDC_ENABLE, mProperties.isEnabled);
  DDX_Check(pDX, IDC_SILENT, mProperties.isSilent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BeSweetPreferencesPanel, CDialog)
	//{{AFX_MSG_MAP(BeSweetPreferencesPanel)
	ON_BN_CLICKED(IDCANCEL, onCancel)
	ON_BN_CLICKED(IDOK, onOk)
	ON_BN_CLICKED(IDC_RELOAD, onReload)
	ON_BN_CLICKED(IDC_BROWSE, onBrowse)
  ON_BN_CLICKED(IDC_LEVEL_ERROR, onLogLevelChanged)
  ON_BN_CLICKED(IDC_LEVEL_WARNING, onLogLevelChanged)
  ON_BN_CLICKED(IDC_LEVEL_INFO, onLogLevelChanged)
  ON_BN_CLICKED(IDC_LEVEL_DEBUG, onLogLevelChanged)
  ON_BN_CLICKED(IDC_LEVEL_TRACE, onLogLevelChanged)
  ON_CBN_SELCHANGE(IDC_AREA, onAreaSelectionChanged)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BeSweetPreferencesPanel message handlers
/////////////////////////////////////////////////////////////////////////////
// BeSweetPreferencesPanel dialog

std::string version()
{
  string version;

  try {
  
    vector<char> buf(1024);
    GetModuleFileName(GetModuleHandle("ESBDSAddin.dll"), &buf[0], buf.size());
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

bool BeSweetPreferencesPanel::enabled() const
{
  return mProperties.isEnabled != 0;
}

void BeSweetPreferencesPanel::enabled(bool b)
{
  mProperties.isEnabled = b;
}

const std::string& BeSweetPreferencesPanel::getWorkspace() const
{
  return mProperties.workspace;
}

void BeSweetPreferencesPanel::setWorkspace(const std::string &wspace)
{
  mProperties.workspace = wspace;
}

const std::list<std::string>& BeSweetPreferencesPanel::getOptions() const
{
  return mProperties.options;
}
void BeSweetPreferencesPanel::setOptions(const std::list<std::string> &options)
{
  mProperties.options = options;
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

BOOL BeSweetPreferencesPanel::OnInitDialog()
{
  loadProperties();

  CDialog::OnInitDialog();

  renderWorkspace();
  if(mProperties.workspace.empty() || mProperties.options.empty()) {
    GetDlgItem(IDC_RELOAD)->EnableWindow(FALSE);
  }

  SetDlgItemText(IDC_ESBSERVER_EXE, mProperties.server.c_str());

  mDoReload = false;
  
  mAreas.AddString(ADDIN.c_str());
  mAreas.AddString(COMMAND.c_str());
  mAreas.AddString(EVENT.c_str());
  mAreas.SetCurSel(0);
  onAreaSelectionChanged();

  SetWindowText(("Be Sweet Addin (version " + version() + ") Preferences").c_str());
  return TRUE;
}

void BeSweetPreferencesPanel::renderWorkspace()
{
  if(m_hWnd) {
    mWorkspaceCtrl.SetRedraw(false);
    mWorkspaceCtrl.DeleteAllItems();

    if(!mProperties.workspace.empty()) {
      HTREEITEM root = mWorkspaceCtrl.InsertItem(mProperties.workspace.c_str());
    
      for(list<string>::iterator option = mProperties.options.begin(); option != mProperties.options.end(); ++option) {
        mWorkspaceCtrl.InsertItem(option->c_str(), root);
      }
      mWorkspaceCtrl.Expand(root, TVE_EXPAND);

    } else {
      mWorkspaceCtrl.InsertItem("no workspace loaded");
    }
    
    mWorkspaceCtrl.SetRedraw(true);
  }
}

void BeSweetPreferencesPanel::onCancel() 
{
  mProperties = mOldProperties;
  CDialog::OnCancel();
}

void BeSweetPreferencesPanel::onOk() 
{
	UpdateData(true);
 
  saveProperties();

  CDialog::OnOK();
}

void BeSweetPreferencesPanel::onReload() 
{
  mDoReload = true;
}

std::string BeSweetPreferencesPanel::getArea()
{
  CString text;
  mAreas.GetLBText(mAreas.GetCurSel(), text);
  return text.GetBuffer(0);
}

void BeSweetPreferencesPanel::onLogLevelChanged() 
{
  int level = (GetCheckedRadioButton(IDC_LEVEL_ERROR, IDC_LEVEL_TRACE)-IDC_LEVEL_ERROR)*10;
  mProperties.areaLogLevels[getArea()] = level;
}

void BeSweetPreferencesPanel::onAreaSelectionChanged() 
{
  int level = mProperties.areaLogLevels[getArea()]/10 + IDC_LEVEL_ERROR;
  CheckRadioButton(IDC_LEVEL_ERROR, IDC_LEVEL_TRACE, level);
}

std::string BeSweetPreferencesPanel::getDll()
{
  vector<char>buffer(1024);
  GetModuleFileName(GetModuleHandle("ESBDSAddin.dll"), &buffer[0], buffer.size());
 return &buffer[0];
}

std::string BeSweetPreferencesPanel::getServerExe()
{
  string dll = getDll();
  return dll.substr(0, dll.rfind('\\')) + "\\ESBServer.exe";
}

void BeSweetPreferencesPanel::onBrowse()
{
  static char filter[] = "ESBServer (*.exe)|*.exe|All Files (*.*)|*.*||";

  CFileDialog dlg(true, NULL, getServerExe().c_str(), 0, filter);

  if(dlg.DoModal() == IDOK) {
    mProperties.server = dlg.GetPathName().GetBuffer(0);
    SetDlgItemText(IDC_ESBSERVER_EXE, dlg.GetPathName());
  }
}

extern map<string, int> area_levels;

void BeSweetPreferencesPanel::loadProperties()
{
  CRegKey root;
  root.Open(HKEY_CURRENT_USER, BESWEET_REG_KEY);

  DWORD dw;
  mProperties.isEnabled = root.QueryValue(dw, ENABLE_REG_VALUE_KEY) == ERROR_SUCCESS ? dw : 1;

  mProperties.isSilent = root.QueryValue(dw, SILENT_REG_VALUE_KEY) == ERROR_SUCCESS ? dw : 0;

  vector<char> buf(1024);
  unsigned long size = buf.size();
  string key = root.QueryValue(&buf[0], LOGLEVEL_REG_VALUE_KEY, &size) == ERROR_SUCCESS ? string(&buf[0]) : getDefaultAreaLevels();

  list<string> areas = StringListTokenizer(key, ";").tokens();
  for(list<string>::iterator area = areas.begin(); area != areas.end(); ++area) {
    vector<string> x = StringVectorTokenizer(*area, ":").tokens();
    if(x.size() == 2) {
      mProperties.areaLogLevels[x[0]] = lexical_cast<int>(x[1]);
    }
  }

  size = buf.size();
  key = root.QueryValue(&buf[0], SERVER_EXE_REG_VALUE_KEY, &size) == ERROR_SUCCESS ? string(&buf[0]) : getServerExe();
  log(BE_SWEET, ADDIN, eDebug) << "registry key " << SERVER_EXE_REG_VALUE_KEY << " is " << key << endlog;
  mProperties.server = key;

  mOldProperties = mProperties;
  area_levels = mProperties.areaLogLevels;
}


void BeSweetPreferencesPanel::saveProperties()
{
  CRegKey root;
  root.Open(HKEY_CURRENT_USER, BESWEET_REG_KEY);
  
  root.SetValue(mProperties.isEnabled, ENABLE_REG_VALUE_KEY);
  root.SetValue(mProperties.isSilent, SILENT_REG_VALUE_KEY);
  root.SetValue(mProperties.server.c_str(), SERVER_EXE_REG_VALUE_KEY);

  stringstream ss;
  for(map<string, int>::iterator entry = mProperties.areaLogLevels.begin(); entry != mProperties.areaLogLevels.end(); ++entry) {
    ss << entry->first << ":" << entry->second << ";"; 
  }
  root.SetValue(ss.str().c_str(), LOGLEVEL_REG_VALUE_KEY);

  area_levels = mProperties.areaLogLevels;

  mOldProperties = mProperties;  
}

void BeSweetPreferencesPanel::createProperties()
{
  CRegKey root;
  if(root.Create(HKEY_CURRENT_USER, BESWEET_REG_KEY) != ERROR_SUCCESS) {
    string text = "could not create registry key " + string(BESWEET_REG_KEY) + " in current-user";
    ::MessageBox(NULL, text.c_str(), "Be Sweet", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
    throw runtime_error(text);
  }

  root.SetValue((DWORD)1, ENABLE_REG_VALUE_KEY);
  root.SetValue((DWORD)1, SILENT_REG_VALUE_KEY);

  root.SetValue(getDefaultAreaLevels().c_str(), LOGLEVEL_REG_VALUE_KEY);
}

std::string BeSweetPreferencesPanel::getDefaultAreaLevels()
{
  stringstream ss;
  ss << ADDIN << ":" << eInfo << ";" << COMMAND << ":" << eInfo << ";" << EVENT << ":" << eInfo;
  return ss.str();
}
