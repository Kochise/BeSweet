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

// ESBAddInSimulatorDlg.h : header file
//

#if !defined(AFX_ESBADDINSIMULATORDLG_H__A3BB5106_D328_49BF_B247_FCB886D2D7E8__INCLUDED_)
#define AFX_ESBADDINSIMULATORDLG_H__A3BB5106_D328_49BF_B247_FCB886D2D7E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <map>

/////////////////////////////////////////////////////////////////////////////
// ESBAddInSimulatorDlg dialog

class ESBAddInSimulatorDlg : public CDialog
{
// Construction
public:
	ESBAddInSimulatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(ESBAddInSimulatorDlg)
	enum { IDD = IDD_ESBADDINSIMULATOR_DIALOG };
	CTreeCtrl	mTree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ESBAddInSimulatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

  void openWorkspace(const std::pair<std::string, std::string> &);
  void closeWorkspace();
  void showBrowser(const std::string&);

  std::string mSelectedFile;
  std::pair<std::string/*name*/, std::string/*projects*/> mWorkspace;

	// Generated message map functions
	//{{AFX_MSG(ESBAddInSimulatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnDropFiles(HDROP hDropInfo );
  afx_msg void onFileSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void onReparseFile();
	afx_msg void onShowFileOutline();
	afx_msg void onLoadworkspace();
	afx_msg void onRaiseOutlineBrowser();
	afx_msg void onRaiseMethodBrowser();
	afx_msg void onRaiseTypebrowser();
	afx_msg void onRaiseWorkspacebrowser();
	afx_msg void onRaiseHierarchyBrowser();
	afx_msg void onCloseWorkspace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESBADDINSIMULATORDLG_H__A3BB5106_D328_49BF_B247_FCB886D2D7E8__INCLUDED_)
