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

#include "AppearancePropertyPanel.h"

#include <Renderer/UIProperties.h>

#include <Utilities/Properties.h>
#include <Utilities/stl_ext.h>
#include <Utilities/StringTokenizer.h>

#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// AppearancePropertyPanel dialog


AppearancePropertyPanel::AppearancePropertyPanel(CWnd* pParent /*=NULL*/)
	: COptionPage(AppearancePropertyPanel::IDD, (UINT)0)
{
	//{{AFX_DATA_INIT(AppearancePropertyPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  mShowReturnTypes = PropertiesManager::instance().getProperty(Browsers::SHOW_RETURNTYPES, "1") == "1";
  mCloseTagBrowsersOnReturn = PropertiesManager::instance().getProperty(Browsers::CLOSE_ON_RETURN, "1") == "1";
  mRaiseOnMouseHoover = PropertiesManager::instance().getProperty(Browsers::RAISE_ON_MOUSE_HOOVER, "1") == "1";
  mHideOnMouseExit = PropertiesManager::instance().getProperty(Browsers::HIDE_ON_MOUSE_EXIT, "1") == "1";
  mShowTooltips = PropertiesManager::instance().getProperty(Browsers::SHOW_TOOLTIPS, "1") == "1";
  mStaysAllwaysOnTop = PropertiesManager::instance().getProperty(Browsers::STAY_ALLWAYS_ONTOP, "0") == "1";
  mRoll = PropertiesManager::instance().getProperty(Browsers::ROLL_UNROLL, "0") == "1";
  mFade = PropertiesManager::instance().getProperty(Browsers::FADE_UNFADE, "0") == "1";

  vector<string> roll = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::ROLL_ANIMATION, ""), ":").tokens();
  if(roll.size() != 3) {
    mRollTime = 300;
    mRollWidth = 30;
    mRollHeight = 30;
  } else {
    mRollWidth = lexical_cast<UINT>(roll[0]);
    mRollHeight = lexical_cast<UINT>(roll[1]);
    mRollTime = lexical_cast<UINT>(roll[2]);
  }

  vector<string> fade = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::FADE_ANIMATION, ""), ":").tokens();
  if(fade.size() != 3) {
    mMinOpacity = 30;
    mMaxOpacity = 100;
    mFadeTime = mRollTime;
  } else {
    mMinOpacity = lexical_cast<UINT>(fade[0]);
    mMaxOpacity = lexical_cast<UINT>(fade[1]);
    mFadeTime = lexical_cast<UINT>(fade[2]);
  }

  mEnlargeSize = lexical_cast<int>(PropertiesManager::instance().getProperty(Browsers::FUZZY_SIZE, "0"));
}


void AppearancePropertyPanel::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AppearancePropertyPanel)
	DDX_Check(pDX, IDC_SHOW_RETURN_TYPES, mShowReturnTypes);
  DDX_Check(pDX, IDC_CLOSE_ON_RETURN, mCloseTagBrowsersOnReturn);
  DDX_Check(pDX, IDC_AUTO_RAISE, mRaiseOnMouseHoover);
  DDX_Check(pDX, IDC_HIDE, mHideOnMouseExit);
  DDX_Check(pDX, IDC_SHOW_TOOLTIPS, mShowTooltips);
  DDX_Check(pDX, IDC_ROLL, mRoll);
  DDX_Check(pDX, IDC_FADE, mFade);
  DDX_Check(pDX, IDC_ALLWAYS_ONTOP, mStaysAllwaysOnTop);
  DDX_Text(pDX, IDC_ROLL_WIDTH, mRollWidth);
  DDX_Text(pDX, IDC_ROLL_HEIGHT, mRollHeight);
  DDX_Text(pDX, IDC_ROLL_TIME, mRollTime);
  DDX_Text(pDX, IDC_MIN_OPACITY, mMinOpacity);
  DDX_Text(pDX, IDC_MAX_OPACITY, mMaxOpacity);
  DDX_Text(pDX, IDC_FADE_TIME, mFadeTime);
  DDX_Text(pDX, IDC_ENLARGE, mEnlargeSize);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AppearancePropertyPanel, CDialog)
	//{{AFX_MSG_MAP(AppearancePropertyPanel)
	ON_BN_CLICKED(IDC_SHOW_RETURN_TYPES, onPropertyChanged)
  ON_BN_CLICKED(IDC_CLOSE_ON_RETURN, onPropertyChanged)
  ON_BN_CLICKED(IDC_AUTO_RAISE, onPropertyChanged)
  ON_BN_CLICKED(IDC_HIDE, onPropertyChanged)
  ON_BN_CLICKED(IDC_SHOW_TOOLTIPS, onPropertyChanged)
  ON_BN_CLICKED(IDC_ALLWAYS_ONTOP, onPropertyChanged)
  ON_BN_CLICKED(IDC_ROLL, onRollAnimationChanged)
  ON_BN_CLICKED(IDC_FADE, onFadeAnimationChanged)
	ON_EN_CHANGE(IDC_ROLL_TIME, onPropertyChanged)
	ON_EN_CHANGE(IDC_ROLL_WIDTH, onPropertyChanged)
  ON_EN_CHANGE(IDC_ROLL_HEIGHT, onPropertyChanged)
  ON_EN_CHANGE(IDC_MIN_OPACITY, onPropertyChanged)
	ON_EN_CHANGE(IDC_MAX_OPACITY, onPropertyChanged)
	ON_EN_CHANGE(IDC_FADE_TIME, onPropertyChanged)
	ON_EN_CHANGE(IDC_ENLARGE, onPropertyChanged)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL AppearancePropertyPanel::OnInitDialog()
{
  COptionPage::OnInitDialog();
  onRollAnimationChanged();
  onFadeAnimationChanged();
  SetModified(false);
  return TRUE;
}

BOOL AppearancePropertyPanel::OnApply()
{
  if(m_hWnd) {
    UpdateData(TRUE);
    PropertiesManager::instance().setProperty(Browsers::SHOW_RETURNTYPES, lexical_cast<string>(mShowReturnTypes));    
    PropertiesManager::instance().setProperty(Browsers::CLOSE_ON_RETURN, lexical_cast<string>(mCloseTagBrowsersOnReturn));    
    PropertiesManager::instance().setProperty(Browsers::RAISE_ON_MOUSE_HOOVER, lexical_cast<string>(mRaiseOnMouseHoover));    
    PropertiesManager::instance().setProperty(Browsers::HIDE_ON_MOUSE_EXIT, lexical_cast<string>(mHideOnMouseExit));    
    PropertiesManager::instance().setProperty(Browsers::SHOW_TOOLTIPS, lexical_cast<string>(mShowTooltips));    
    PropertiesManager::instance().setProperty(Browsers::STAY_ALLWAYS_ONTOP, lexical_cast<string>(mStaysAllwaysOnTop));    

    PropertiesManager::instance().setProperty(Browsers::ROLL_UNROLL, lexical_cast<string>(mRoll));    
    PropertiesManager::instance().setProperty(Browsers::ROLL_ANIMATION, lexical_cast<string>(mRollWidth) + ":" + lexical_cast<string>(mRollHeight) + ":" + lexical_cast<string>(mRollTime));    
    
    PropertiesManager::instance().setProperty(Browsers::FADE_UNFADE, lexical_cast<string>(mFade));    
    PropertiesManager::instance().setProperty(Browsers::FADE_ANIMATION, lexical_cast<string>(mMinOpacity) + ":" + lexical_cast<string>(mMaxOpacity) + ":" + lexical_cast<string>(mFadeTime));    

    PropertiesManager::instance().setProperty(Browsers::FUZZY_SIZE, lexical_cast<string>(mEnlargeSize));    

    PropertiesManager::instance().dump();
  }
  
  return COptionPage::OnApply();
}

void AppearancePropertyPanel::onRollAnimationChanged()
{
  BOOL enable = static_cast<CButton*>(GetDlgItem(IDC_ROLL))->GetCheck();
  GetDlgItem(IDC_ROLL_TIME)->EnableWindow(enable);
  GetDlgItem(IDC_ROLL_WIDTH)->EnableWindow(enable);
  GetDlgItem(IDC_ROLL_HEIGHT)->EnableWindow(enable);
  SetModified();
}

void AppearancePropertyPanel::onFadeAnimationChanged()
{
  BOOL enable = static_cast<CButton*>(GetDlgItem(IDC_FADE))->GetCheck();
  GetDlgItem(IDC_FADE_TIME)->EnableWindow(enable);
  GetDlgItem(IDC_MIN_OPACITY)->EnableWindow(enable);
  GetDlgItem(IDC_MAX_OPACITY)->EnableWindow(enable);
  SetModified();
}

void AppearancePropertyPanel::onPropertyChanged() 
{
  SetModified();
}

