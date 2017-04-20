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

#if !defined(AFX_BESWEETPREFERENCESPANEL_H__18CD50D8_E9C0_4E7B_B46A_6AFBF20B7EE0__INCLUDED_)
#define AFX_BESWEETPREFERENCESPANEL_H__18CD50D8_E9C0_4E7B_B46A_6AFBF20B7EE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BeSweetPreferencesPanel.h : header file
//

#include <string>
#include <list>
#include <map>

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// BeSweetPreferencesPanel dialog

class BeSweetPreferencesPanel : public CDialog
{
private:
  bool mDoReload;
  struct Properties
  {
    int isEnabled;
    int isSilent;
    std::string workspace;
    std::list<std::string> options;
    std::map<std::string, int> areaLogLevels;
    std::string server;
  };

public:
	BeSweetPreferencesPanel(CWnd* pParent = NULL);   // standard constructor

  bool enabled() const;
  void enabled(bool);

  const std::string& getWorkspace() const;
  void setWorkspace(const std::string&);

  const std::list<std::string>& getOptions() const;
  void setOptions(const std::list<std::string>&);
  
  bool needsReload() { return mDoReload; }
  bool  isSilent() { return mProperties.isSilent != 0; };

  const std::string& getServer()
  { return mProperties.server; }

  void setServer(const std::string &s)
  { mProperties.server = s; saveProperties(); }
  
  //{{AFX_DATA(BeSweetPreferencesPanel)
	enum { IDD = IDD_ESBADDIN_PREFERENCES };
	CTreeCtrl	mWorkspaceCtrl;
  CComboBox	mAreas;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(BeSweetPreferencesPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
  BOOL OnInitDialog();
	//{{AFX_MSG(BeSweetPreferencesPanel)
	afx_msg void onCancel();
	afx_msg void onOk();
	afx_msg void onReload();
  afx_msg void onLogLevelChanged();
  afx_msg void onAreaSelectionChanged();
  afx_msg void onBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void renderWorkspace();

  void loadProperties();
  void saveProperties();
  void createProperties();

  std::string getArea();
  std::string getDefaultAreaLevels();

private:
  std::string getDll();
  std::string getServerExe();
  
private:
  Properties mProperties;
  Properties mOldProperties;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BESWEETPREFERENCESPANEL_H__18CD50D8_E9C0_4E7B_B46A_6AFBF20B7EE0__INCLUDED_)
