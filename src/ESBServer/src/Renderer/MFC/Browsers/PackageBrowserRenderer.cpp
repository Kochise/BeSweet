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

// PackageBrowserRenderer.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PackageBrowserRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PackageBrowserRenderer dialog


PackageBrowserRenderer::PackageBrowserRenderer(CWnd* pParent /*=NULL*/)
	: RaisableDialog(PackageBrowserRenderer::IDD, pParent)
{
	//{{AFX_DATA_INIT(PackageBrowserRenderer)
	//}}AFX_DATA_INIT
}


void PackageBrowserRenderer::DoDataExchange(CDataExchange* pDX)
{
	RaisableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PackageBrowserRenderer)
	DDX_Control(pDX, IDC_NAMESPACES, mNamespaces);
	DDX_Control(pDX, IDC_MEMBERS1, mMembers);
	DDX_Control(pDX, IDC_TYPES, mTypes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PackageBrowserRenderer, RaisableDialog)
	//{{AFX_MSG_MAP(PackageBrowserRenderer)
	ON_WM_KEYDOWN()
	ON_WM_VKEYTOITEM()
	ON_NOTIFY(NM_SETFOCUS, IDC_CLASSES, OnSetfocusClasses)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PackageBrowserRenderer message handlers

BOOL PackageBrowserRenderer::OnInitDialog() 
{
	RaisableDialog::OnInitDialog();
	
  mNamespaces.InsertColumn(0, "Namespaces", LVCFMT_LEFT, 300);
  mNamespaces.InsertItem(0, "namespaces");
  mNamespaces.SetColumnWidth(0, LVSCW_AUTOSIZE); 

  mTypes.InsertColumn(0, "Types", LVCFMT_LEFT, 300);
  mTypes.InsertItem(0, "types");
  mTypes.SetColumnWidth(0, LVSCW_AUTOSIZE); 

  mMembers.InsertColumn(0, "Members", LVCFMT_LEFT, 300);
  mMembers.InsertItem(0, "members");
  mMembers.SetColumnWidth(0, LVSCW_AUTOSIZE); 

  mSplitter1.BindWithControl(this, IDC_SPLITTERBAR1);
  mSplitter1.AttachAsLeftPane(IDC_NAMESPACES);
  mSplitter1.AttachAsLeftPane(IDC_NSPACE_CAPTION);
  mSplitter1.AttachAsRightPane(IDC_TYPES); 
  mSplitter1.AttachAsRightPane(IDC_TYPES_CAPTION); 
  mSplitter1.RecalcLayout();	

  mSplitter2.BindWithControl(this, IDC_SPLITTERBAR2);
  mSplitter2.AttachAsLeftPane(IDC_TYPES); 
  mSplitter2.AttachAsLeftPane(IDC_TYPES_CAPTION); 
  mSplitter2.AttachAsRightPane(IDC_MEMBERS1);
  mSplitter2.AttachAsRightPane(IDC_MEMBERS_CAPTION); 
  mSplitter2.RecalcLayout();	


  AllowSizing(sizeResize,sizeResize);
  AddControl(IDC_NAMESPACES,sizeNone,sizeResize);
  AddControl(IDC_TYPES,sizeNone,sizeResize);
	AddControl(IDC_MEMBERS1,sizeResize,sizeResize);
  HideSizeIcon();

	return TRUE;
}

void PackageBrowserRenderer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	RaisableDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

int PackageBrowserRenderer::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	// TODO: Add your message handler code here and/or call default
	
	return RaisableDialog::OnVKeyToItem(nKey, pListBox, nIndex);
}

void PackageBrowserRenderer::OnSetfocusClasses(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
