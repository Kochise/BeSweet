#ifndef LoggerPanel_H
#define LoggerPanel_H

#include <Renderer/MFC/3rdParty/ResizingDialog.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LoggerPanel : public CResizingDialog, public LoggerConsumer
{
private:
  typedef CResizingDialog super;

public:
	LoggerPanel(CWnd* pParent = NULL);   // standard constructor

  virtual	void logEntry(unsigned long timeStamp, long tid, short level, const std::string &label, const std::string &area, const std::string &text) throw(LoggingException);

  //{{AFX_DATA(LoggerPanel)
	enum { IDD = IDD_LOGGER_PANEL };
	CListCtrl	mRenderer;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(LoggerPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(LoggerPanel)
  afx_msg BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
