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

#include <stdafx.h>

#include "PropertiesTree.h"

BEGIN_MESSAGE_MAP(PropertiesTree, CTreeCtrl)
	//{{AFX_MSG_MAP(PropertiesTree)
	ON_NOTIFY_REFLECT(NM_CLICK, onSelectionChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL PropertiesTree::RemovePage(int index)
{
  return FALSE;
}

BOOL  PropertiesTree::Create (COptionSheet *parent, UINT nID)
{
  SetSelection(0);
  return CTreeCtrl::Create(WS_BORDER | WS_TABSTOP | WS_CHILD |
							TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS/* | TVS_NOTIFY*/,
							CRect(0,0,0,0),
							parent,
							nID);
}

BOOL PropertiesTree::AddGroup (COptionPage *page)
{
  CString text;
  page->GetCaption(text);
  HTREEITEM item = InsertItem(text.IsEmpty() ? "???" : text, -1, -1, TVI_ROOT);
  SetItemData(item, (LPARAM)page);
  mGroupMap[page] = item;
  return true;
}

BOOL PropertiesTree:: AddPage (COptionPage *page, COptionPage *group)
{
  ASSERT(page);
  CString text;
  page->GetCaption(text);

  HTREEITEM parent = mGroupMap[group];
  HTREEITEM item = InsertItem(text, -1, -1, parent/*TVI_ROOT*/);
  SetItemData(item, (LPARAM)page);


  return TRUE;
}

BOOL  PropertiesTree::SetSelection(COptionPage *selection)
{
  mSelection = selection;
  return TRUE;
}

COptionPage*  PropertiesTree::GetSelection()
{
  return mSelection;
}

CWnd *  PropertiesTree::GetControlWnd()
{
  return this;
}

void PropertiesTree::onSelectionChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
  CPoint p(GetMessagePos()); 
  ScreenToClient(&p);

  UINT htFlags = 0;
  HTREEITEM item = HitTest(p, &htFlags);
  if(!item) {
    return;
  }


  if(htFlags==TVHT_ONITEMICON || htFlags==TVHT_ONITEMLABEL) {
    SetSelection(reinterpret_cast<COptionPage*>(GetItemData(item)));
    NotifyParentOfSelChanging();
    NotifyParentOfSelChange();
  }
}
