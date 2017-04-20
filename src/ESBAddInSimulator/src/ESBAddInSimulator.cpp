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

// ESBAddInSimulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ESBAddInSimulator.h"
#include "ESBAddInSimulatorDlg.h"
#include "ESBDSAddInImplX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ESBAddInSimulatorApp

BEGIN_MESSAGE_MAP(ESBAddInSimulatorApp, CWinApp)
	//{{AFX_MSG_MAP(ESBAddInSimulatorApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ESBAddInSimulatorApp construction

ESBAddInSimulatorApp::ESBAddInSimulatorApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only ESBAddInSimulatorApp object

ESBAddInSimulatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// ESBAddInSimulatorApp initialization

BOOL ESBAddInSimulatorApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  //SimulatorComModule::instance().UpdateRegistryFromResource(IDR_ESBDSADDIN, TRUE);
	//SimulatorComModule::instance().RegisterServer(TRUE);
  SimulatorComModule::instance().InitializeATL();
  //AddinSimulator::setupComStuff();
  
  ESBAddInSimulatorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

 // SimulatorComModule::instance().UpdateRegistryFromResource(IDR_ESBDSADDIN, FALSE);
  //SimulatorComModule::instance().UnregisterServer(TRUE); //TRUE means typelib is unreg'd
// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
