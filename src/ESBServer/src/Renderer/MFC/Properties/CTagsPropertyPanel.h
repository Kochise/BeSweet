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

#if !defined(AFX_CTAGSPROPERTYPANEL_H__ED9BA538_E04D_4BD6_AD72_C1EDF6E82342__INCLUDED_)
#define AFX_CTAGSPROPERTYPANEL_H__ED9BA538_E04D_4BD6_AD72_C1EDF6E82342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 
#include <Renderer/MFC/3rdParty/OptionPage/OptionPage.h>

#include <string>


namespace CTags
{
   extern const std::string CTAGS_EXE;
   extern const std::string SHOW_CTAGS_WINDOW;
   extern const std::string ADDITIONAL_OPTIONS;

class PropertyPanel : public COptionPage
{
private:

public:
  PropertyPanel();

protected:
  virtual BOOL OnApply();

	//{{AFX_DATA(CTags::PropertyPanel)
	enum { IDD = IDD_CTAGS_PROPERTIES_PANEL };
	CString	mAdditionalOptions;
	CString	mExe;
	BOOL	mShowCTags;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CTags::PropertyPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CTags::PropertyPanel)
	afx_msg void onPropertyChanged();
  afx_msg void onBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

} //namespace CTags

//{{AFX_INSERT_LOCATION}}

#endif
