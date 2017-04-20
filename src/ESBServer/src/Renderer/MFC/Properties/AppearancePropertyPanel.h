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

#if !defined(AFX_APPEARANCERROPERTIESPANEL_H__C2339AF9_7B09_4BC8_B306_1C5E182DB4CF__INCLUDED_)
#define AFX_APPEARANCERROPERTIESPANEL_H__C2339AF9_7B09_4BC8_B306_1C5E182DB4CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Renderer/MFC/3rdParty/OptionPage/OptionPage.h>

#include <string>

class AppearancePropertyPanel : public COptionPage
{
public:
	AppearancePropertyPanel(CWnd* pParent = NULL); 

  virtual BOOL OnApply();

  //{{AFX_DATA(AppearancerRopertiesPanel)
	enum { IDD = IDD_APPEARANCE_PROPERTIES_PANEL };
  BOOL	mShowReturnTypes;
  BOOL	mCloseTagBrowsersOnReturn;
  BOOL	mRaiseOnMouseHoover;
  BOOL	mHideOnMouseExit;
  BOOL  mShowTooltips;
  BOOL  mStaysAllwaysOnTop;
  BOOL  mRoll;
  int   mRollWidth;
  int   mRollHeight;
  int   mRollTime;
  BOOL  mFade;
  int   mMinOpacity;
  int   mMaxOpacity;
  int   mFadeTime;
  int   mEnlargeSize;
  //}}AFX_DATA


	//{{AFX_VIRTUAL(AppearancerRopertiesPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
  BOOL OnInitDialog();
	//{{AFX_MSG(AppearancerRopertiesPanel)
  afx_msg void onPropertyChanged();
  afx_msg void onRollAnimationChanged();
  afx_msg void onFadeAnimationChanged();
  //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPEARANCERROPERTIESPANEL_H__C2339AF9_7B09_4BC8_B306_1C5E182DB4CF__INCLUDED_)
