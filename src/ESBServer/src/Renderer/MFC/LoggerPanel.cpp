#include "stdafx.h"
#include "resource.h"

#include "LoggerPanel.h"
#include <stl_ext.h>
#include <StringTokenizer.h>

#include <string>
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// LoggerPanel dialog


LoggerPanel::LoggerPanel(CWnd* pParent /*=NULL*/)
	: super(LoggerPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(LoggerPanel)
	//}}AFX_DATA_INIT
}


void LoggerPanel::DoDataExchange(CDataExchange* pDX)
{
	super::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoggerPanel)
	DDX_Control(pDX, IDC_LOG_ENTRY_LIST, mRenderer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoggerPanel, super)
	//{{AFX_MSG_MAP(LoggerPanel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL LoggerPanel::OnInitDialog()
{
  super::OnInitDialog();

  HICON icon = AfxGetApp()->LoadIcon(IDI_LOG_WINDOW);
  SetIcon(icon, FALSE);  // Set big icon
	SetIcon(icon, TRUE);		// Set small icon

  mRenderer.InsertColumn(0, "Time", LVCFMT_LEFT, 80);
  mRenderer.InsertColumn(1, "Thread", LVCFMT_LEFT, 50);
  mRenderer.InsertColumn(2, "Label", LVCFMT_LEFT, 50);
  mRenderer.InsertColumn(3, "Area", LVCFMT_LEFT, 50);
  mRenderer.InsertColumn(4, "Level", LVCFMT_LEFT, 50);
  mRenderer.InsertColumn(5, "Message", LVCFMT_LEFT, 300);
  
  AllowSizing(sizeResize,sizeResize);
  AddControl(IDC_LOG_ENTRY_LIST, sizeResize,sizeResize);
  HideSizeIcon();
  
  return TRUE;
}

const std::string levels[] = { "Error", "Warning", "Info", "Debug", "Trace"};


void LoggerPanel::logEntry(unsigned long timeStamp, long threadId, short level, const std::string &label, const std::string &area, const std::string &message) throw(LoggingException)
{
  try {
  
  mRenderer.SetRedraw(false);

  int first, i;
  first = i = mRenderer.GetItemCount();
  if(i > 1000) {
    mRenderer.DeleteItem(0);
    i--;
  }

  list<string> tokens = StringListTokenizer(message, "\n").tokens();
  for(list<string>::iterator token = tokens.begin(); token != tokens.end(); ++token, ++i) {
    
    LVITEM item;
    item.mask = LVIF_TEXT;
    item.pszText = "";
    item.iItem = i;
    item.iSubItem = 0;
    mRenderer.InsertItem(&item);

    if(token == tokens.begin()) {
      mRenderer.SetItemText(i,0, lexical_cast<string>(timeStamp).c_str());
      mRenderer.SetItemText(i,1, lexical_cast<string>(threadId).c_str());
      mRenderer.SetItemText(i,2, label.c_str());
      mRenderer.SetItemText(i,3, area.c_str());
      mRenderer.SetItemText(i,4, levels[level/10].c_str());
    }
    mRenderer.SetItemText(i,5, token->c_str());
  }

  mRenderer.SetColumnWidth( 2, LVSCW_AUTOSIZE );
  mRenderer.SetColumnWidth( 3, LVSCW_AUTOSIZE );
  mRenderer.SetColumnWidth( 5, LVSCW_AUTOSIZE );
  
  mRenderer.SetRedraw(true);

  mRenderer.EnsureVisible(first, false);

  if(level < eWarning) {
    ShowWindow(SW_SHOW);
    SetWindowPos(&wndTop, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
  }

  } catch(...) {
    MessageBox("LoggerPanel::logEntry(): unknown exception");
  }
}
