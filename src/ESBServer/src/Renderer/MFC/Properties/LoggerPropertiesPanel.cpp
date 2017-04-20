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

#include "stdafx.h"
#include "resource.h"
#include "LoggerPropertiesPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LoggerPropertiesPanel::LoggerPropertiesPanel(CWnd* pParent)
	: COptionPage(LoggerPropertiesPanel::IDD, (UINT)0)
{
	//{{AFX_DATA_INIT(LoggerPropertiesPanel)
	//}}AFX_DATA_INIT
}


void LoggerPropertiesPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoggerPropertiesPanel)
	DDX_Control(pDX, IDC_LOGLEVEL, mLogLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoggerPropertiesPanel, CDialog)
	//{{AFX_MSG_MAP(LoggerPropertiesPanel)
	ON_CBN_SELCHANGE(IDC_LOGLEVEL, onLogLevelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL LoggerPropertiesPanel::OnInitDialog()
{
  super::OnInitDialog();

  short level = LoggerManager::instance().logger(NO_LABEL)->logLevel()/10;
  mLogLevel.SetCurSel(level);

  return TRUE;
}

void LoggerPropertiesPanel::onLogLevelChanged() 
{
   LoggerManager::instance().logger(NO_LABEL)->logLevel(mLogLevel.GetCurSel()*10);
}
