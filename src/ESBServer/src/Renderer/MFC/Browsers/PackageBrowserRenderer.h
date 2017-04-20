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

#if !defined(AFX_PACKAGEBROWSERRENDERER_H__FC2B2974_FAD9_4015_8673_88CE34DBDD77__INCLUDED_)
#define AFX_PACKAGEBROWSERRENDERER_H__FC2B2974_FAD9_4015_8673_88CE34DBDD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PackageBrowserRenderer.h : header file
//

#include <Renderer/MFC/3rdParty/xySplitterWnd.h>
#include <Renderer/MFC/Base/CustomDialog.h>

/////////////////////////////////////////////////////////////////////////////
// PackageBrowserRenderer dialog

class PackageBrowserRenderer : public RaisableDialog
{
private:
  CxSplitterWnd mSplitter1;
  CxSplitterWnd mSplitter2;

public:
	PackageBrowserRenderer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PackageBrowserRenderer)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	mNamespaces;
	CListCtrl	mMembers;
	CListCtrl	mTypes;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(PackageBrowserRenderer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(PackageBrowserRenderer)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	afx_msg void OnSetfocusClasses(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKAGEBROWSERRENDERER_H__FC2B2974_FAD9_4015_8673_88CE34DBDD77__INCLUDED_)
