#ifndef Progressbar_H
#define Progressbar_H

#if _MSC_VER > 1000
#pragma once
#endif 


class Progressbar : public CDialog
{
public:
	Progressbar(CWnd* pParent = NULL);

	//{{AFX_DATA(Progressbar)
  CProgressCtrl mProgressbar;
	enum { IDD = IDD_PROGRESS };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(Progressbar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(Progressbar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif