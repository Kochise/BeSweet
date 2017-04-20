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

#include <stdafx.h>
#include <resource.h>

#include "HierarchyPropertyPanel.h"
#include <Renderer/UIProperties.h>
#include <Utilities/Properties.h>
#include <Utilities/stl_ext.h>

#include <string>

using namespace std;

namespace Browsers {

HierarchyPropertyPanel::HierarchyPropertyPanel(CWnd* pParent /*=NULL*/)
	: COptionPage(HierarchyPropertyPanel::IDD, (UINT)0)
{
	//{{AFX_DATA_INIT(HierarchyPropertyPanel)
  mHoverTime = "";
  mShowNamespaces = FALSE;
  mWarnMultipleParents = TRUE;
  //}}AFX_DATA_INIT
}


void HierarchyPropertyPanel::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HierarchyPropertyPanel)
  DDX_Text(pDX, IDC_HOVER, mHoverTime);
  DDX_Check(pDX, IDC_SHOW_NAMESPACES, mShowNamespaces);
  DDX_Check(pDX, IDC_WARN_MULTIPLE_PARENTS, mWarnMultipleParents);
  
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HierarchyPropertyPanel, COptionPage)
	//{{AFX_MSG_MAP(HierarchyPropertyPanel)
  ON_EN_CHANGE(IDC_HOVER, onPropertyChanged)
  ON_BN_CLICKED(IDC_SHOW_HEADER_AND_SOURCE_FILE_CONETENT, onPropertyChanged)
  ON_BN_CLICKED(IDC_SHOW_NAMESPACES, onPropertyChanged)
  ON_BN_CLICKED(IDC_WARN_MULTIPLE_PARENTS, onPropertyChanged)
  ON_BN_CLICKED(IDC_BOTTOMLEFT1, onPropertyChanged)
  ON_BN_CLICKED(IDC_BOTTOMRIGHT1, onPropertyChanged)
  ON_BN_CLICKED(IDC_TOPLEFT1, onPropertyChanged)
  ON_BN_CLICKED(IDC_TOPRIGHT1, onPropertyChanged)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

std::string getProperty(const std::string &key, const std::string &key2)
{
  string value = PropertiesManager::instance().getProperty(key, "");
  return value.empty() ? PropertiesManager::instance().getProperty(key2, "") : value;
}

BOOL HierarchyPropertyPanel::OnInitDialog()
{
  COptionPage::OnInitDialog();
  string property;
  
  mHoverTime = getProperty(Browsers::Hierarchy::HOVERTIME, Browsers::HOVERTIME).c_str();
  mShowNamespaces = PropertiesManager::instance().getProperty(Hierarchy::SHOW_FQ_NAMES, "0") == "1";
  mShowNamespaces = PropertiesManager::instance().getProperty(Hierarchy::WARN_MULTIPLE_PARENTS, "1") == "1";
  
  string corner = getProperty(Hierarchy::RAISECORNER, Browsers::RAISECORNER);
  CheckRadioButton(IDC_TOPLEFT,IDC_BOTTOMRIGHT,IDC_TOPLEFT + lexical_cast<int>(corner));
  
  UpdateData(false);
  SetModified(false);
  return TRUE;
}

BOOL HierarchyPropertyPanel::OnApply()
{
  if(m_hWnd) {
    UpdateData(TRUE);
    PropertiesManager::instance().setProperty(Hierarchy::HOVERTIME, mHoverTime.GetBuffer(0));
    PropertiesManager::instance().setProperty(Hierarchy::SHOW_FQ_NAMES, lexical_cast<string>(mShowNamespaces));
    PropertiesManager::instance().setProperty(Hierarchy::WARN_MULTIPLE_PARENTS, lexical_cast<string>(mWarnMultipleParents));
    
    string corner = lexical_cast<string>(GetCheckedRadioButton(IDC_TOPLEFT,IDC_BOTTOMRIGHT) - IDC_TOPLEFT);
    PropertiesManager::instance().setProperty(Hierarchy::RAISECORNER, corner);
    PropertiesManager::instance().dump();
  }
  
  return COptionPage::OnApply();
}


void HierarchyPropertyPanel::onPropertyChanged() 
{
  SetModified();
}

}