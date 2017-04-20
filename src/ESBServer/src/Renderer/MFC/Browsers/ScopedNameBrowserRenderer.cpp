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

#include "ScopedNameBrowserRenderer.h"
#include "ScopedNameBrowser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <algorithm>
#include <sstream>
#include <exception>

using namespace std;


ScopedNameBrowserRenderer::ScopedNameBrowserRenderer(ScopedNameBrowser *ctrl, CWnd* pParent /*=NULL*/)
	: super(ScopedNameBrowserRenderer::IDD, pParent), mController(ctrl)
{
	//{{AFX_DATA_INIT(ScopedNameBrowserRenderer)
	//}}AFX_DATA_INIT
  mImages = &ESBImageList::instance(); 
  setIcon(IDI_TYPEBROWSER);
}


void ScopedNameBrowserRenderer::DoDataExchange(CDataExchange* pDX)
{
	super::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ScopedNameBrowserRenderer)
  DDX_Control(pDX, IDC_FILTER, mFilter);
	DDX_Control(pDX, IDC_TYPES, mNameListRenderer);
	DDX_Control(pDX, IDC_QUALIFIERS, mScopeListRenderer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ScopedNameBrowserRenderer, super)
	//{{AFX_MSG_MAP(ScopedNameBrowserRenderer)
  ON_WM_KEYDOWN()
	ON_NOTIFY(NM_CLICK, IDC_TYPES, onNameSelectionChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_TYPES, onNameSelectionChanged)
	ON_NOTIFY(NM_CLICK, IDC_QUALIFIERS, onScopeSelectionChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_QUALIFIERS, onScopeSelectionChanged)
	ON_EN_CHANGE(IDC_FILTER, onFilterChanged)
	ON_BN_CLICKED(IDC_CLOSE, onClose)
  ON_WM_SHOWWINDOW()
  ON_NOTIFY(NM_SETFOCUS, IDC_TYPES, onFocusNames)
  ON_NOTIFY(NM_SETFOCUS, IDC_QUALIFIERS, onFocusQualifiers)
  //}}AFX_MSG_MAP
  ON_NOTIFY(LVN_KEYDOWN, IDC_TYPES, onNameKeyPressed)
  ON_NOTIFY(LVN_KEYDOWN, IDC_QUALIFIERS, onQualifierKeyPressed)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ScopedNameBrowserRenderer message handlers

BOOL ScopedNameBrowserRenderer::OnInitDialog() 
{
	super::OnInitDialog();


  AllowSizing(sizeResize,sizeResize);
  AddControl(IDC_CHOOSE,sizeResize,sizeNone);
  AddControl(IDC_TYPEFILTER,sizeResize,sizeNone);
	AddControl(IDC_TYPES,sizeResize,sizeResize);
  AddControl(IDC_QUALIFIER_LABEL,sizeNone,sizeRepos);
  AddControl(IDC_QUALIFIERS,sizeResize,sizeRepos);
  AddControl(IDC_CLOSE,sizeRepos,sizeRepos);
  AddControl(IDCANCEL,sizeRepos,sizeRepos);
  HideSizeIcon();

  getNameListRenderer().SetImageList(getImages(), LVSIL_SMALL);
  getNameListRenderer().InsertColumn(0, "dummy text", LVCFMT_LEFT, 300);

  getScopeListRenderer().SetImageList(getImages(), LVSIL_SMALL);
  getScopeListRenderer().InsertColumn(0, "dummy text", LVCFMT_LEFT, 300);
  getScopeListRenderer().SetColumnWidth(0, LVSCW_AUTOSIZE); 
  
  SetDlgItemText(IDC_CHOOSE, mFilterDescription.c_str());
  
  /*mSplitter.BindWithControl(this, IDC_QUALIFIER_LABEL);
  mSplitter.AttachAsAbovePane(IDC_TYPES);
  mSplitter.AttachAsBelowPane(IDC_QUALIFIERS);
  mSplitter.RecalcLayout();	*/

  setFocusableWindow(GetDlgItem(IDC_FILTER));

  SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
  SetWindowPos(&wndNoTopMost, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);

  return TRUE;
}

void ScopedNameBrowserRenderer::onNameSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
  const static string function = "ScopedNameBrowserRenderer::onNameSelectionChanged()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    *pResult = 0;
    if(isSelected(pNMHDR)) {
      ScopedNameBrowserItem *item = getData(getNameListRenderer(), pNMHDR);
      switch(pNMHDR->code) {
        case NM_CLICK: getController().typeSelectionChanged(item); break;
        case NM_DBLCLK : getController().typeSelected(item); break;
      }
	  }

  } catch(std::out_of_range&) {
    //ignore..user clicked somewhere but neither on the item label nor iten icon

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::onScopeSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
  const static string function = "ScopedNameBrowserRenderer::onScopeSelectionChanged()";
  Tracer trc(NO_LABEL, RENDERER, function);

  *pResult = 1;
  try {
    if(isSelected(pNMHDR)) {
      ScopedNameBrowserItem *item = getData(getScopeListRenderer(), pNMHDR);
      switch(pNMHDR->code) {
        case NM_CLICK: getController().qualifierSelectionChanged(item); break;
        case NM_DBLCLK : getController().typeSelected(item); break;
      }
	  }
    *pResult = 0;

  } catch(std::out_of_range&) {
    //ignore..user clicked somewhere but not on the item label or iten icon

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::onFilterChanged() 
{
  const static string function = "ScopedNameBrowserRenderer::onFilterChanged()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    CString text;
    GetDlgItemText(IDC_TYPEFILTER, text);

    getController().filterChanged(text.GetBuffer(0));
    
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::OnKeyDown(UINT c, UINT nRepCnt, UINT nFlags) 
{
  const static string function = "ScopedNameBrowserRenderer::OnKeyDown()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    message_box("key pressed");
    if(c == VK_DOWN || c == VK_UP) {
      message_box("up/down key pressed");
    }

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}


void ScopedNameBrowserRenderer::onNameKeyPressed(NMHDR* pNMHDR, LRESULT* pResult)
{
  const static string function = "ScopedNameBrowserRenderer::onNameKeyPressed()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    
    LV_KEYDOWN* key = (LV_KEYDOWN*)pNMHDR;
    *pResult = 0; 
    
    ScopedNameBrowserItem *item = getData((char)key->wVKey, getNameListRenderer().GetNextItem(-1, LVIS_SELECTED), getNameListRenderer());
    if(item) {
       getController().typeSelectionChanged(item);

    } else if(isalpha(key->wVKey)) {
      mNameFilter.insert(mNameFilter.end(), key->wVKey);
      mNameFilter = to_lower(mNameFilter);
      setFilter(mNameFilter);
      
    } else if(key->wVKey == VK_BACK) {
      mNameFilter.erase(mNameFilter.size()-1);
      GetDlgItem(IDC_FILTER)->SetWindowText(mNameFilter.c_str());      
    
    } else if(key->wVKey == VK_ESCAPE || key->wVKey == VK_DELETE) {
      mNameFilter.clear();
      GetDlgItem(IDC_FILTER)->SetWindowText(mNameFilter.c_str());      
    }

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::onQualifierKeyPressed(NMHDR* pNMHDR, LRESULT* pResult)
{
  const static string function = "ScopedNameBrowserRenderer::onQualifierKeyPressed()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    
    LV_KEYDOWN* key = (LV_KEYDOWN*)pNMHDR;
    *pResult = 0; 
    
    ScopedNameBrowserItem *item = getData((char)key->wVKey, getScopeListRenderer().GetNextItem(-1, LVIS_SELECTED), getScopeListRenderer());
    if(item) {
       getController().qualifierSelectionChanged(item);
    }

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

ScopedNameBrowserItem* ScopedNameBrowserRenderer::getData(char key, int index, const CListCtrl &ctrl)
{
  int offset;
  switch(key) {
    case  VK_DOWN : offset = 1; break;
    case  VK_UP : offset = -1; break;

    default: return NULL;
  }

  if(index+offset >= 0 && index+offset < ctrl.GetItemCount()) {       
    return reinterpret_cast<ScopedNameBrowserItem*>(ctrl.GetItemData(index+offset));
  }

  return NULL;
}

bool ScopedNameBrowserRenderer::isSelected(NMHDR *pNMHDR)
{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//  if ((pNMListView->uChanged & LVIF_STATE) != LVIF_STATE)
//	     false; // state has not changed
//
//  // Has the selected state changed?
//  if (!(pNMListView->uNewState & LVIS_SELECTED) || (pNMListView->uOldState & LVIS_SELECTED))
//    return false;

	return true;
}

void ScopedNameBrowserRenderer::append(CListCtrl &ctrl, const smart_ptr<ScopedNameBrowserItem> &scopedItem, bool useScope)
{
  string text = useScope ? scopedItem->getScopeLabel() : scopedItem->getNameLabel();
  int icon = useScope ? scopedItem->getScopeIcon() : scopedItem->getNameIcon();

  LV_ITEM item;
	item.mask= icon > 0 ? LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM : LVIF_PARAM | LVIF_TEXT;
	item.pszText=(char*)text.c_str();
	item.cchTextMax=1000;
	item.iSubItem=0;
  item.iItem = ctrl.GetItemCount();
	item.iImage = icon;
  item.lParam = (LPARAM)scopedItem.get();

  ctrl.InsertItem(&item);  
}

ScopedNameBrowserItem* ScopedNameBrowserRenderer::getData(const CListCtrl &ctrl, NMHDR *pNMHDR)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	
  if(pNMListView->iItem < 0) {
    throw out_of_range("item index < 0");
  }
  return reinterpret_cast<ScopedNameBrowserItem*>(ctrl.GetItemData(pNMListView->iItem));
}

ScopedNameBrowserItem* ScopedNameBrowserRenderer::getSelectedItem()
{
	for( int i = 0; i < getNameListRenderer().GetItemCount(); i++ ) {
		if( getNameListRenderer().GetItemState( i, LVIS_SELECTED )  ) {
			return reinterpret_cast<ScopedNameBrowserItem*>(getNameListRenderer().GetItemData(i));
		}
	}
	return NULL;
}

void ScopedNameBrowserRenderer::onFocusNames(NMHDR* pNMHDR, LRESULT* pResult) 
{
  const static string function = "ScopedNameBrowserRenderer::onFocusNames()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    *pResult = 0;
    getNameListRenderer().SetItemState(getNameListRenderer().GetNextItem(-1, LVIS_SELECTED), LVIS_FOCUSED, LVIS_FOCUSED);
  
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::onFocusQualifiers(NMHDR* pNMHDR, LRESULT* pResult) 
{
  const static string function = "ScopedNameBrowserRenderer::onFocusQualifiers()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    *pResult = 0;
    getScopeListRenderer().SetItemState(getScopeListRenderer().GetNextItem(-1, LVIS_SELECTED), LVIS_FOCUSED, LVIS_FOCUSED);
  
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::setFilter(const std::string &value)
{
  GetDlgItem(IDC_FILTER)->SetWindowText(value.c_str());
}

void ScopedNameBrowserRenderer::onClose() 
{
  const static string function = "ScopedNameBrowserRenderer::onClose()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
  	getController().typeSelected(getSelectedItem());
	  CDialog::OnOK(); 

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::OnOK() 
{ 
  const static string function = "ScopedNameBrowserRenderer::OnOK()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
	  getController().typeSelected(getController().getSelection());  

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void ScopedNameBrowserRenderer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
  const static string function = "ScopedNameBrowserRenderer::OnShowWindow()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
	  super::OnShowWindow(bShow, nStatus);
  	onFilterChanged();

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << e << endlog;
    
  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}