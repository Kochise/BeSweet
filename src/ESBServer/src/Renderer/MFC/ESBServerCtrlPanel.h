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

// ESBServerCtrlPanel.h : header file
//

#if !defined(AFX_ESBSERVERCTRLPANEL_H__B7D8D929_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_)
#define AFX_ESBSERVERCTRLPANEL_H__B7D8D929_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Renderer/MFC/3rdParty/OptionPage/OptionPage.h>
#include <Renderer/MFC/3rdParty/SystemTray.h>

class ESBServer;


/////////////////////////////////////////////////////////////////////////////
// ESBServerControlPanel dialog

//typedef CustomDialog ESBParent;
typedef COptionPage ESBParent;

class ESBServerControlPanel : public ESBParent
{
private:
  ESBServer *mController;

  // Construction
public:
	ESBServerControlPanel(ESBServer*, CWnd* pParent = NULL);	// standard constructor
  ~ESBServerControlPanel();
  
// Dialog Data
	//{{AFX_DATA(ESBServerControlPanel)
	enum { IDD = IDD_ESBSERVER_CONRTOL_PANEL };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ESBServerControlPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
  std::string mName;
  
	void mouseAt(CPoint&);

  LRESULT onShowFileOutline(WPARAM wParam, LPARAM lParam);
  LRESULT onReparseFile(WPARAM wParam, LPARAM lParam);
  LRESULT onOpenWorkspace(WPARAM wParam, LPARAM lParam);
  LRESULT onShowBrowser(WPARAM wParam, LPARAM lParam);

  ESBServer* getController() { return mController; }

	// Generated message map functions
	//{{AFX_MSG(ESBServerControlPanel)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void onTypeBrowser();
	afx_msg void onOutlineBrowser();
	afx_msg void onReparseWorkspace();
	afx_msg void onWorkspaceBrowser();
	afx_msg void onMethodBrowser();
	afx_msg void onHierarchyBrowser();
	afx_msg void onPackagesBrowser();
  afx_msg void onExit();
  afx_msg void OnTimer(UINT);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESBSERVERCTRLPANEL_H__B7D8D929_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_)
