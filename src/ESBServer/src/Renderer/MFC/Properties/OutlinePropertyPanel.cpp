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

// OutlinePropertyPanel.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "OutlinePropertyPanel.h"

#include <Renderer/UIProperties.h>
#include <Utilities/Properties.h>
#include <Utilities/stl_ext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// OutlinePropertyPanel dialog

namespace Browsers {

OutlinePropertyPanel::OutlinePropertyPanel()
	: COptionPage(OutlinePropertyPanel::IDD, (UINT)0)
{
	//{{AFX_DATA_INIT(Browsers::OutlinePropertyPanel)
	mHoverTime = "";
	mShowHeaderAndSource = FALSE;
	mShowNamespaces = FALSE;
	//}}AFX_DATA_INIT
}


void OutlinePropertyPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Browsers::OutlinePropertyPanel)
	DDX_Text(pDX, IDC_HOVER, mHoverTime);
	DDX_Check(pDX, IDC_SHOW_HEADER_AND_SOURCE_FILE_CONETENT, mShowHeaderAndSource);
	DDX_Check(pDX, IDC_SHOW_NAMESPACES, mShowNamespaces);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OutlinePropertyPanel, CDialog)
	//{{AFX_MSG_MAP(Browsers::OutlinePropertyPanel)
	ON_EN_CHANGE(IDC_HOVER, onPropertyChanged)
	ON_BN_CLICKED(IDC_SHOW_HEADER_AND_SOURCE_FILE_CONETENT, onPropertyChanged)
	ON_BN_CLICKED(IDC_SHOW_NAMESPACES, onPropertyChanged)
	ON_BN_CLICKED(IDC_BOTTOMLEFT1, onPropertyChanged)
	ON_BN_CLICKED(IDC_BOTTOMRIGHT1, onPropertyChanged)
	ON_BN_CLICKED(IDC_TOPLEFT1, onPropertyChanged)
	ON_BN_CLICKED(IDC_TOPRIGHT1, onPropertyChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OutlinePropertyPanel message handlers

BOOL OutlinePropertyPanel::OnInitDialog()
{
  COptionPage::OnInitDialog();
  string property;
    
  mHoverTime = getProperty(Outline::HOVERTIME, Browsers::HOVERTIME).c_str();
  mShowHeaderAndSource = PropertiesManager::instance().getProperty(Outline::SHOW_HEADER_AND_SOURCE, "1") == "1";
  mShowNamespaces = PropertiesManager::instance().getProperty(Outline::SHOW_NAMESPACES, "0") == "1";

  string corner = getProperty(Outline::RAISECORNER, Browsers::RAISECORNER);
  CheckRadioButton(IDC_TOPLEFT,IDC_BOTTOMRIGHT,IDC_TOPLEFT + lexical_cast<int>(corner));
  
  UpdateData(false);
  SetModified(false);
  return TRUE;
}

BOOL OutlinePropertyPanel::OnApply()
{
  if(m_hWnd) {
    UpdateData(TRUE);
    PropertiesManager::instance().setProperty(Outline::HOVERTIME, mHoverTime.GetBuffer(0));
    PropertiesManager::instance().setProperty(Outline::SHOW_HEADER_AND_SOURCE, lexical_cast<string>(mShowHeaderAndSource));
    PropertiesManager::instance().setProperty(Outline::SHOW_NAMESPACES, lexical_cast<string>(mShowNamespaces));
    
    string corner = lexical_cast<string>(GetCheckedRadioButton(IDC_TOPLEFT,IDC_BOTTOMRIGHT) - IDC_TOPLEFT);
    PropertiesManager::instance().setProperty(Outline::RAISECORNER, corner);
    PropertiesManager::instance().dump();
  }

  return COptionPage::OnApply();
}

std::string OutlinePropertyPanel::getProperty(const std::string &key, const std::string &key2)
{
  string value = PropertiesManager::instance().getProperty(key, "");
  return value.empty() ? PropertiesManager::instance().getProperty(key2, "") : value;
}

} // namespace Browsers

void Browsers::OutlinePropertyPanel::onPropertyChanged() 
{
  SetModified();
}

