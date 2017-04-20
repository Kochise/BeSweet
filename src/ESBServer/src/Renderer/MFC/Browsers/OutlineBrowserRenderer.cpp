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


#pragma warning( disable:4800 )

#include "stdafx.h"
#include "resource.h"

#include <Controller/Browsers/OutlineBrowser.h>

#include <Renderer/LabelBroker.h>
#include <Renderer/TooltipBroker.h>
#include <Renderer/MFC/ESBImageList.h>
#include <Renderer/MFC/Properties/OutlinePropertyPanel.h>
#include <Renderer/MFC/Browsers/OutlineBrowserRenderer.h>
#include <Renderer/MFC/TreeCtrlHelper.h>

#include <Utilities/StringTokenizer.h>

#include <string>

//#include <boost/bind.hpp>

using namespace std;
using namespace boost;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// OutlineBrowserRenderer dialog

BEGIN_MESSAGE_MAP(OutlineBrowserRenderer, super )
  //{{AFX_MSG_MAP(OutlineBrowserRenderer)
  ON_COMMAND(ID_OUTLINEBROWSER_HEADER, onGotoHeader)
  ON_COMMAND(ID_OUTLINEBROWSER_SOURCE, onGotoSource)
  ON_COMMAND(ID_OUTLINEBROWSER_INSPECT, onInspectTag)
  ON_COMMAND(ID_OUTLINEBROWSER_HIERARCHY, onShowHierarchy)
  ON_WM_SIZE()
  ON_WM_CREATE()
  //}}AFX_MSG_MAP

  ON_NOTIFY(NM_CLICK, IDC_OUTLINE, onMouseClicked)
  ON_NOTIFY(NM_DBLCLK, IDC_OUTLINE, onMouseClicked)
  ON_NOTIFY(NM_RCLICK, IDC_OUTLINE, onMouseClicked)
  ON_NOTIFY(NM_RDBLCLK, IDC_OUTLINE, onMouseClicked)
	ON_NOTIFY(TVN_GETINFOTIP, IDC_OUTLINE, onGetToolTip)
  //ON_NOTIFY_EX( TTN_NEEDTEXT, 0, onToolTipNotify )

  ON_BN_CLICKED(IDC_OUTLINE_SORT, onSort)
  ON_BN_CLICKED(IDC_OUTLINE_NO_FIELDS, onNoFields)
  ON_BN_CLICKED(IDC_OUTLINE_NO_STATIC, onNoStatic)
  ON_BN_CLICKED(IDC_OUTLINE_PUBLIC_ONLY, onPublicOnly)
  ON_BN_CLICKED(IDC_CUSTOM_FILTER, onCustomFilter)

END_MESSAGE_MAP()


OutlineBrowserRenderer::OutlineBrowserRenderer(OutlineBrowser *c, CWnd* pParent /*=NULL*/)
	: super(), mController(c), mOld(0,0,0,0)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::OutlineBrowserRenderer()");
  
  size_t style = WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  Create("", style, CRect(0,0, 300, 450), pParent ? pParent : GetDesktopWindow(), IDC_OUTLANE_PANEL); //super=CStatic
  //Create(NULL, "", style, CRect(0,0, 300, 450), pParent ? pParent : GetDesktopWindow(), IDC_OUTLANE_PANEL); //super=cwnd
  
  initialize();
  super::registerMouseListener(this);
}



void OutlineBrowserRenderer::initialize() 
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::initialize()");

  CSize size(22,23);
  CRect rect;

  GetClientRect(rect);
  size_t treeStyle = WS_VISIBLE | TVS_INFOTIP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_CHILD | WS_BORDER | WS_TABSTOP;
  size_t border = 2;
  mOutline.EnableToolTips();
  mOutline.CreateEx(WS_EX_CLIENTEDGE, "SysTreeView32", "", treeStyle, CRect(CPoint(0, size.cy), CPoint(rect.Width(), rect.Height())), this, IDC_OUTLINE);
  mOutline.ModifyStyleEx(TVS_NOTOOLTIPS, TVS_INFOTIP);
  mOutline.ModifyStyle(TVS_NOTOOLTIPS, TVS_INFOTIP);
  mOutline.GetToolTips()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
  mOutline.GetToolTips()->AddTool(&mOutline, LPSTR_TEXTCALLBACK, NULL, 0);
  mOutline.GetToolTips()->SetDelayTime(0, 1000);
  
#if _MSC_VER > 1200  // VC7
  mOutline.CreateEx(WS_EX_CLIENTEDGE, treeStyle, CRect(CPoint(0, size.cy), CPoint(rect.Width(), rect.Height())), this, IDC_OUTLINE);
#else
  getOutlinePanel().SetImageList(&ESBImageList::instance(), TVSIL_NORMAL );
#endif
  mOutline.SetWindowText("ffosfdgoosfg");

  GetWindowRect(rect);
  ScreenToClient(rect);

  rect.SetRect(CPoint(rect.right-size.cx, 0), CPoint(rect.right, size.cy));

  rect.OffsetRect(-border, 0);
  mSorted.Create(_T(""), getStyle(),rect, this, IDC_OUTLINE_SORT);
  mSorted.SetBitmaps(IDB_SORT_A, RGB(0,0,0), IDB_SORT_F);
  mSorted.SetTooltipText("Sort");
  mSorted.SetWindowPos(&wndTopMost,0,0,0,0,SWP_SHOWWINDOW|
                                    SWP_NOSIZE |SWP_NOMOVE);
  rect.OffsetRect(-size.cx, 0);
  mNoFields.Create(_T(""), getStyle(), rect, this, IDC_OUTLINE_NO_FIELDS);
  mNoFields.SetBitmaps(IDB_NO_FIELDS_A, RGB(0,0,0), IDB_NO_FIELDS_F);
  mNoFields.SetTooltipText("Hide Fields");
  
  rect.OffsetRect(-size.cx, 0);
  mNoStatic.Create(_T(""), getStyle(),rect, this, IDC_OUTLINE_NO_STATIC);
  mNoStatic.SetBitmaps(IDB_NO_STATIC_A, RGB(0,0,0), IDB_NO_STATIC_F);
  mNoStatic.SetTooltipText("Hide Static Members");

  rect.OffsetRect(-size.cx, 0);
  mPublicOnly.Create(_T(""), getStyle(), rect, this, IDC_OUTLINE_PUBLIC_ONLY);
  mPublicOnly.SetBitmaps(IDB_PUBLIC_ONLY_A, RGB(0,0,0), IDB_PUBLIC_ONLY_F, RGB(255,0,0));
  mPublicOnly.SetTooltipText("Hide Non-Public Members");
 
  size = CSize(29, size.cy);
  rect.OffsetRect(-size.cx, 0);
  rect.SetRect(rect.TopLeft(), CPoint(rect.TopLeft().x + size.cx , size.cy));
  mCustomFilter.Create(_T(""), getStyle(), rect, this, IDC_CUSTOM_FILTER);
  mCustomFilter.SetBitmaps(IDB_FILTER_A, RGB(0,0,0), IDB_FILTER_F, RGB(255,0,0));
  mCustomFilter.SetTooltipText("Custom Filter");

  mCustomFilter.SetMenu(IDR_CUSTOM_OUTLINE_FILTER, *this, true);
  mCustomFilter.GetMenu().RemoveMenu(0, MF_BYPOSITION);
  BCMenu *popup = mCustomFilter.GetMenu().AppendODPopupMenuA("Custom Filter");  

  getOutlinePanel().ModifyStyle(0, TVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
  
}

size_t OutlineBrowserRenderer::getStyle()
{
  return WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_BITMAP | BS_AUTOCHECKBOX | BS_NOTIFY;
}


/////////////////////////////////////////////////////////////////////////////
// OutlineBrowserRenderer message handlers

void OutlineBrowserRenderer::render()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::render()");

  getOutlinePanel().SetRedraw(false);
  getOutlinePanel().DeleteAllItems();
  
  render(getController().getChildrenOf(NULL), TVI_ROOT);
  
  getOutlinePanel().SetRedraw(true);
}

void OutlineBrowserRenderer::render(const TagList &elements, HTREEITEM parent)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::render(const TagList&, HTREEITEM)");

  LabelBroker broker;
  //for_each(outline.begin(), outline.end(), bind(&MetaObject::acceptVisitor, _1, &outliner));
  for(TagList::const_iterator element = elements.begin(); element != elements.end(); element++) {

    HTREEITEM x = getOutlinePanel().InsertItem(broker.getLabel(*(*element)).c_str(), parent);
    int icon = ESBImageList::instance().getIcon(*(*element));
	  getOutlinePanel().SetItemImage(x, icon, icon);
    getOutlinePanel().SetItemData(x, (long)element->get());  

    render(getController().getChildrenOf(element->get()), x);
    if(parent == TVI_ROOT) {
      getOutlinePanel().Expand(x, TVE_EXPAND);
    }
	}
}

void OutlineBrowserRenderer::showPopupMenu(MetaObject *tag, short x, short y)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::render(MetaObject*, short, short)");

  if(tag) { 
    CMenu menu;
	  menu.LoadMenu(IDR_OUTLINE_BROWSER_POPUPMENU);
	  CMenu* pPopup = menu.GetSubMenu(0);

    if(tag->getDeclaration().getFile().empty()) {
      pPopup->EnableMenuItem(ID_OUTLINEBROWSER_HEADER, MF_GRAYED);
    } 
    if(tag->getDefinition().getFile().empty()) {
      pPopup->EnableMenuItem(ID_OUTLINEBROWSER_SOURCE, MF_GRAYED);
    }
    MetaObjectTyper typer;
    tag->acceptVisitor(&typer);
    if(!(typer.getType() & (eClass | eStruct | eUnion))) {
      pPopup->EnableMenuItem(ID_OUTLINEBROWSER_HIERARCHY, MF_GRAYED);
    }
 	  pPopup->TrackPopupMenu(TPM_LEFTALIGN, x, y, this);
  }
}

void OutlineBrowserRenderer::onSort()
{
  const static string function("OutlineBrowserRenderer::onSort()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
    getOutlinePanel().SetFocus();
    getController().sortOrderChanged(mSorted.GetCheck() == 1);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onPublicOnly()
{
  const static string function("OutlineBrowserRenderer::onPublicOnly()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
	  getOutlinePanel().SetFocus();
	  getController().publicMemberFilterChanged(mPublicOnly.GetCheck() == 1);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onNoStatic()
{
  const static string function("OutlineBrowserRenderer::onNoStatic()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
	  getOutlinePanel().SetFocus();
	  getController().staticMemberFilterChanged(mNoStatic.GetCheck() == 1);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onNoFields()
{
  const static string function("OutlineBrowserRenderer::onNoFields()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
	  getOutlinePanel().SetFocus();
	  getController().fieldFilterChanged(mNoFields.GetCheck() == 1);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onCustomFilter()
{
  const static string function("OutlineBrowserRenderer::onCustomFilter()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
	  getOutlinePanel().SetFocus();
    bool isChecked = mCustomFilter.GetCheck() == 1;
    
    CPoint pos = GetMessagePos();
    CRect rect;
    mCustomFilter.GetWindowRect(&rect);
    
    if(pos.x < rect.left + 10 ) {
      if(!isChecked) {
        mCustomFilter.SetCheck(TRUE);
      }
      if(!getController().customFilterChanged() && isChecked) {
        mCustomFilter.SetCheck(FALSE);
        return;
      }
    }

	  getController().customFilterEnabled(isChecked);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onGotoHeader() 
{
  const static string function("OutlineBrowserRenderer::onGotoHeader()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
	  getController().gotoHeader();

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onGotoSource() 
{
  const static string function("OutlineBrowserRenderer::onGotoSource()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
	  getController().gotoSource();

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onInspectTag() 
{
  const static string function("OutlineBrowserRenderer::onInspectTag()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
	  getController().inspect();

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onShowHierarchy() 
{
  static const string function = "OutlineBrowserRenderer::onShowHierarchy()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    getController().gotoHierarchy();

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::onMouseClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
  static const string function = "OutlineBrowserRenderer::onMouseClicked(NMHDR*, LRESULT*) ";
  Tracer trc(NO_LABEL, RENDERER, function);

  *pResult = 0;

  MSG msg;
  if(!PeekMessage(&msg, *this, WM_MOUSEFIRST , WM_MOUSELAST , PM_NOREMOVE)){
    msg.wParam = 0;
  }

  size_t clickCount = 0;
  MouseEvent::MouseButton button;
  CWnd *wnd = &getOutlinePanel();
  switch(pNMHDR->code) {
    case NM_CLICK:   button = MouseEvent::eLeftButton;  clickCount = 1; break;
    case NM_DBLCLK:  button = MouseEvent::eLeftButton;  clickCount = 2; break;
    case NM_RCLICK:  button = MouseEvent::eRightButton; clickCount = 1; break;
    case NM_RDBLCLK: button = MouseEvent::eRightButton; clickCount = 2; break;
  }

  mouseClicked(MouseEvent(MouseEvent::eClicked, button | msg.wParam, wnd, GetMessagePos(), clickCount));
}

void OutlineBrowserRenderer::mouseClicked(const MouseEvent &e)
{
  static const string function = "OutlineBrowserRenderer::mouseClicked(MouseEvent)";
  Tracer trc(NO_LABEL, RENDERER, function);
  
  TagRetriever retriever(&getOutlinePanel());

  list<MetaObject*> selection;
  HTREEITEM item = getOutlinePanel().GetFirstSelectedItem();
  while(item != NULL) {
    selection.push_back(retriever.getTag(item));
    item = getOutlinePanel().GetNextSelectedItem(item);
  }

  getController().tagSelectionChanged(selection);
  getController().tagSelectionChanged(retriever.getTagAt(e.getPoint(), true), e);
}

void OutlineBrowserRenderer::setCaption(const std::string &text)
{
  //SetWindowText(text.c_str());
}

std::string OutlineBrowserRenderer::getCaption()
{
  CString text;
  GetWindowText(text);
  return text.GetBuffer(0);
}

std::pair<short, short> OutlineBrowserRenderer::getPosition()
{
  CRect rect;
  GetWindowRect(&rect);
  return make_pair(rect.TopLeft().x , rect.TopLeft().y);
}

void OutlineBrowserRenderer::setPosition(short x, short y)
{
  SetWindowPos( NULL, x, y, 0 , 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOOWNERZORDER);
}

void OutlineBrowserRenderer::show()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::show()");
  ShowWindow(SW_SHOW);
}

void OutlineBrowserRenderer::hide()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::hide()");
  ShowWindow(SW_HIDE);
}

void OutlineBrowserRenderer::OnSize(UINT nType, int cx, int cy)
{
  static const string function = "OutlineBrowserRenderer::OnSize()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try{
    super::OnSize(nType, cx, cy);
  
    if(!m_hWnd || !mOutline.m_hWnd) return;
    
    // start deferring window pos
    HDWP hdwp=BeginDeferWindowPos(20);

    CRect buttonBar;
    mSorted.GetWindowRect(buttonBar);
    DeferWindowPos(hdwp,mOutline,NULL, 0, 0, cx, cy-buttonBar.Height(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOOWNERZORDER);

    int distance = cx - mOld.Width();
    if(distance) {
      moveChild(mSorted, distance, 0, hdwp);
      moveChild(mNoFields, distance, 0, hdwp);
      moveChild(mNoStatic, distance, 0, hdwp);
      moveChild(mPublicOnly, distance, 0, hdwp); 
      moveChild(mCustomFilter, distance, 0, hdwp); 
    }

    // do the deferred window position change
    EndDeferWindowPos(hdwp);

    //store current rect
    GetWindowRect(&mOld);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void OutlineBrowserRenderer::moveChild(const CWnd &wnd, int x, int y, HDWP hdwp)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::moveChild()");

  CRect rect;
  wnd.GetWindowRect(&rect);
  ScreenToClient(rect);
  rect.OffsetRect(x, y);
  DeferWindowPos(hdwp,wnd,NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

int OutlineBrowserRenderer::OnCreate(LPCREATESTRUCT create)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserRenderer::OnCreate()");

  if(mOld == CRect(0,0,0,0)) {
    mOld.SetRect(create->x, create->y, create->cx, create->cy);
  }

  return super::OnCreate(create);
}

int OutlineBrowserRenderer::onToolTipNotify(UINT, NMHDR *pNMHDR, LRESULT *pResult)
{
  static const string function = "OutlineBrowserRenderer::onToolTipNotify()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try{

    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT*)pNMHDR; 
    getOutlinePanel().GetToolTips()->SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);

    if((pTTT->uFlags & TTF_IDISHWND) && ::GetDlgCtrlID((HWND)pNMHDR->idFrom)) 
    { 
      MetaObject *object = TagRetriever(&getOutlinePanel()).getTagAt(GetMessagePos(), false);
      if(object && PropertiesManager::instance().getProperty(Browsers::SHOW_TOOLTIPS, "1") == "1") { 
        pTTT->lpszText = "item xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
      }
    
      pTTT->hinst = NULL; 
      return TRUE; 
    } 

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }

  return FALSE; 

}

void OutlineBrowserRenderer::onGetToolTip(NMHDR* pNMHDR, LRESULT* pResult) 
{
  static const string function = "OutlineBrowserRenderer::onGetToolTip()";
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
  
    NMTVGETINFOTIP *pInfoTip = (NMTVGETINFOTIP*)pNMHDR;
    *pResult = 1;

    MetaObject *object = TagRetriever(&getOutlinePanel()).getTag(pInfoTip->hItem);
    if(object && PropertiesManager::instance().getProperty(Browsers::SHOW_TOOLTIPS, "1") == "1") {
      static string hack; //.c_str() is tmp only and the InfoTip doesnt copy it imediatly !
      hack = TooltipBroker().getTooltip(*object);
      pInfoTip->pszText = (char*)hack.c_str();
      getOutlinePanel().GetToolTips()->SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);
      *pResult = 0;
    }
    
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}


///////////////////////////////////////////////
// 
///////////////////////////////////////////////
OutlineBrowserDialog::OutlineBrowserDialog()
: RaisableDialog(IDD_EMPTY_DIALOG), mOutline(NULL)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserDialog::OutlineBrowserDialog()");

  setHoverTimeKey(Browsers::Outline::HOVERTIME);
  setRaiseCornerKey(Browsers::Outline::RAISECORNER);
  setPositionKey(Browsers::Outline::POSITION);
  setIcon(IDI_OUTLINE);
}

void OutlineBrowserDialog::show()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserDialog::show()");

	bool modal = false;

//  getRenderer().renderOutline(getModel(), getFilter());
//  setCaption(mOutlineName);
  
//  if(modal) {
//    DoModal();
    
//  } else {
    if(!m_hWnd) {
		  Create(IDD_EMPTY_DIALOG,CWnd::GetDesktopWindow());
    }
    ShowWindow(SW_SHOW);
//  }
}

void OutlineBrowserDialog::hide()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserDialog::hide()");
  super::hide();
}

void OutlineBrowserDialog::close()
{
}

void OutlineBrowserDialog::moveToForeground() 
{ 
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserDialog::moveToForeground()");
  super::moveToForeground(); 
}

BOOL OutlineBrowserDialog::OnInitDialog()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowserDialog::OnInitDialog()");

  RaisableDialog::OnInitDialog();
  
  CWnd *outline = &dynamic_cast<CWnd&>(mOutline);
  CRect rect;
  outline->GetWindowRect(&rect);
  SetWindowPos(NULL, 0, 0, rect.Width()+7, rect.Height()+26, SWP_NOZORDER | SWP_NOMOVE);
  outline->SetParent(this);
  outline->ShowWindow(SW_SHOW);

  AddControl(IDC_OUTLANE_PANEL,sizeResize,sizeResize);

  setFocusableWindow(dynamic_cast<CWnd&>(mOutline).GetDlgItem(IDC_OUTLINE));

  return TRUE;
}
