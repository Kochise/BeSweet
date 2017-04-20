#include "stdafx.h"
#include "resource.h"
#include "Progressbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Progressbar::Progressbar(CWnd* pParent /*=NULL*/)
	: CDialog(Progressbar::IDD, pParent)
{
	//{{AFX_DATA_INIT(Progressbar)
	//}}AFX_DATA_INIT
}


void Progressbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Progressbar)
	DDX_Control(pDX, IDC_PROGRESS, mProgressbar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Progressbar, CDialog)
	//{{AFX_MSG_MAP(Progressbar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
