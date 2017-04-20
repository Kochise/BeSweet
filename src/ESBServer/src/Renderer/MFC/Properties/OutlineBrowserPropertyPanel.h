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

#if !defined(AFX_Browsers::OutlinePropertyPanel_H__AE36C842_6100_47D4_9350_C994D6AC62B7__INCLUDED_)
#define AFX_Browsers::OutlinePropertyPanel_H__AE36C842_6100_47D4_9350_C994D6AC62B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Browsers::OutlinePropertyPanel.h : header file
//
#include "OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// Browsers::OutlinePropertyPanel dialog

namespace Browsers {

class OutlinePropertyPanel : public COptionPage
{
// Construction
public:
	OutlinePropertyPanel();   // standard constructor

	//{{AFX_DATA(Browsers::OutlinePropertyPanel)
	enum { IDD = IDD_OULINE_BROWSER_PROPERTY_PANEL };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(Browsers::OutlinePropertyPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(Browsers::OutlinePropertyPanel)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

} //namespace Browser

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Browsers::OutlinePropertyPanel_H__AE36C842_6100_47D4_9350_C994D6AC62B7__INCLUDED_)
