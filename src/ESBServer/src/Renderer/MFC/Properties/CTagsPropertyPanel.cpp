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

// CTags::PropertyPanel.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CTagsPropertyPanel.h"

#include <Utilities/Properties.h>
#include <Utilities/stl_ext.h>

#include <algorithm>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CTags::PropertyPanel dialog
namespace CTags
{

const std::string CTAGS_EXE("ctags.exe");
const std::string SHOW_CTAGS_WINDOW("ctags.showwindow");
const std::string ADDITIONAL_OPTIONS("ctags.additionalOptions");

using namespace CTags;

PropertyPanel::PropertyPanel()
	: COptionPage(PropertyPanel::IDD, (UINT)0)
{

  //{{AFX_DATA_INIT(CTags::PropertyPanel)
	mShowCTags = FALSE;
	//}}AFX_DATA_INIT

  mExe = PropertiesManager::instance().getProperty(CTAGS_EXE, "").c_str();
  mAdditionalOptions = PropertiesManager::instance().getProperty(ADDITIONAL_OPTIONS, "").c_str();  
  mShowCTags = lexical_cast<int>(PropertiesManager::instance().getProperty(SHOW_CTAGS_WINDOW, "0").c_str());
}


void PropertyPanel::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTags::PropertyPanel)
	DDX_Text(pDX, IDC_ADDITIONAL_CTAGS_OPTIONS, mAdditionalOptions);
	DDX_Text(pDX, IDC_CTAGS_EXE, mExe);
	DDX_Check(pDX, IDC_SHOW_CTAGGER, mShowCTags);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertyPanel, COptionPage)
	//{{AFX_MSG_MAP(CTags::PropertyPanel)
	ON_EN_CHANGE(IDC_ADDITIONAL_CTAGS_OPTIONS, onPropertyChanged)
	ON_EN_CHANGE(IDC_CTAGS_EXE, onPropertyChanged)
  ON_BN_CLICKED(IDC_BROWSE, onBrowse)
  ON_BN_CLICKED(IDC_SHOW_CTAGGER, onPropertyChanged)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
BOOL PropertyPanel::OnApply()
{
  if(m_hWnd) {
    UpdateData(TRUE);
    PropertiesManager::instance().setProperty(CTAGS_EXE, mExe.GetBuffer(0));
    PropertiesManager::instance().setProperty(ADDITIONAL_OPTIONS, mAdditionalOptions.GetBuffer(0));
    PropertiesManager::instance().setProperty(SHOW_CTAGS_WINDOW, lexical_cast<string>(mShowCTags));
    PropertiesManager::instance().dump();
  }

  return COptionPage::OnApply();
}

void PropertyPanel::onBrowse()
{
  static char filter[] = "CTags (*.exe;*.bat)|*.exe;*.bat|All Files (*.*)|*.*||";

  CFileDialog dlg(true, NULL, mExe, 0, filter);

  if(dlg.DoModal() == IDOK) {
    SetDlgItemText(IDC_CTAGS_EXE, dlg.GetPathName());
  }
}

void PropertyPanel::onPropertyChanged() 
{
	SetModified();
}

}
