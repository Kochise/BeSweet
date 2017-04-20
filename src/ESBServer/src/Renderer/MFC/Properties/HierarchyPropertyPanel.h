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

#if !defined(AFX_X_H__567711CE_416F_49D6_A325_6FCEA431DEEA__INCLUDED_)
#define AFX_X_H__567711CE_416F_49D6_A325_6FCEA431DEEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Renderer/MFC/3rdParty/OptionPage/OptionPage.h>

namespace Browsers {

class HierarchyPropertyPanel : public COptionPage
{
public:
	HierarchyPropertyPanel(CWnd* pParent = NULL); 

	//{{AFX_DATA(HierarchyPropertyPanel)
  CString	mHoverTime;
  BOOL	mShowNamespaces;
  BOOL	mWarnMultipleParents;
  enum { IDD = IDD_HIERARCHY_BROWSER_PROPERTIES_PANEL };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(HierarchyPropertyPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
  virtual BOOL OnInitDialog();
  virtual BOOL OnApply();
  
	//{{AFX_MSG(HierarchyPropertyPanel)
  afx_msg void onPropertyChanged();
  //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

}
  
#endif // !defined(AFX_X_H__567711CE_416F_49D6_A325_6FCEA431DEEA__INCLUDED_)
