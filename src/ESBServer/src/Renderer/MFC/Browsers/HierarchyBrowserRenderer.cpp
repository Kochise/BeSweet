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
#include "HierarchyBrowserRenderer.h"

#include <Model/MetaModel/MetaModel.h>

#include <Controller/Browsers/HierarchyBrowser.h>
#include <Renderer/MFC/ESBImageList.h>
#include <Renderer/MFC/TreeCtrlHelper.h>
#include <Renderer/LabelBroker.h>

#include <list>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// HierarchyBrowserRenderer dialog
const size_t FIRST_HISTORY_ENTRY = 10000;
const size_t MAX_HISTORY_SIZE = 10;

BEGIN_MESSAGE_MAP(HierarchyBrowserRenderer, RaisableDialog)
	//{{AFX_MSG_MAP(HierarchyBrowserRenderer)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_HIERARCHY, onExpandHierarchy)
  ON_COMMAND(ID_HIERARCHY_FOCUSON, onFocusOn)
  ON_COMMAND(ID_HIERARCHY_FOCUSONX, onFocusOnX)
  ON_NOTIFY(NM_CLICK, IDC_HIERARCHY, onMouseClickedHierarchy)
  ON_NOTIFY(NM_DBLCLK, IDC_HIERARCHY, onMouseClickedHierarchy)
  ON_NOTIFY(NM_RCLICK, IDC_HIERARCHY, onMouseClickedHierarchy)
  ON_NOTIFY(TVN_GETINFOTIP, IDC_HIERARCHY, onGetToolInfo)
	//}}AFX_MSG_MAP


  ON_BN_CLICKED(IDC_TYPE_HIERARCHY, onShowTypeHierarchy)
  ON_BN_CLICKED(IDC_SUBTYPE_HIERARCHY, onShowSubtypeHierarchy)
  ON_BN_CLICKED(IDC_SUPERTYPE_HIERARCHY, onShowSupertypeHierarchy)
  ON_BN_CLICKED(IDC_LOCK_HIERARCHY, onLockHierarchy)
  ON_BN_CLICKED(IDC_INHERITED_MEMBERS, onShowInheritedMembers)
  
  ON_COMMAND_RANGE(FIRST_HISTORY_ENTRY, FIRST_HISTORY_ENTRY+MAX_HISTORY_SIZE, onHistory)
  
END_MESSAGE_MAP()

HierarchyBrowserRenderer::HierarchyBrowserRenderer(HierarchyBrowser *controller, CWnd* pParent /*=NULL*/)
	: RaisableDialog(HierarchyBrowserRenderer::IDD, pParent), mController(controller)
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::HierarchyBrowserRenderer()");

  //{{AFX_DATA_INIT(HierarchyBrowserRenderer)
	//}}AFX_DATA_INIT
  setIcon(IDI_HIERARCHY);
  setPositionKey(Browsers::Hierarchy::POSITION);
  setRaiseCornerKey(Browsers::Hierarchy::RAISECORNER);

  mProgress.Create(Progressbar::IDD);
}


void HierarchyBrowserRenderer::DoDataExchange(CDataExchange* pDX)
{
	RaisableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HierarchyBrowserRenderer)
	DDX_Control(pDX, IDC_OUTLINE_ICON, mOutlineIcon);
	DDX_Control(pDX, IDC_HIERARCHY, mHierarchyRenderer);
	//}}AFX_DATA_MAP
}

size_t HierarchyBrowserRenderer::getStyle()
{
  return WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_BITMAP | BS_AUTOCHECKBOX;
}

BCMenu* HierarchyBrowserRenderer::getSubMenu(size_t pos)
{
  return dynamic_cast<BCMenu*>(mPreviousHierarchies.GetMenu().GetSubMenu(pos));
}

void HierarchyBrowserRenderer::setupHierarchyView()
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::setupHierarchyView()");

  CSize size(22,23);
  CRect rect;

  GetWindowRect(rect);
  ScreenToClient(rect);
  rect.SetRect(CPoint(rect.right-size.cx, 0), CPoint(rect.right, size.cy));

  rect.OffsetRect(-5, 0);
  mShowSupertypeHierarchy.Create(_T(""), getStyle(), rect, this, IDC_SUPERTYPE_HIERARCHY);
  mShowSupertypeHierarchy.SetBitmaps(IDB_SUPERTYPE_HIERARCHY_A, RGB(0,0,0), IDB_SUPERTYPE_HIERARCHY_F);
  mShowSupertypeHierarchy.SetTooltipText("Show the Supertype Hierarchy");
	AddControl(IDC_SUPERTYPE_HIERARCHY,sizeRepos,sizeNone);


  rect.OffsetRect(-size.cx, 0);
  mShowSubtypeHierarchy.Create(_T(""),getStyle(), rect, this, IDC_SUBTYPE_HIERARCHY);
  mShowSubtypeHierarchy.SetBitmaps(IDB_SUBTYPE_HIERARCHY_A, RGB(0,0,0), IDB_SUBTYPE_HIERARCHY_F);
  mShowSubtypeHierarchy.SetTooltipText("Show the Subtype Hierarchy");
	AddControl(IDC_SUBTYPE_HIERARCHY,sizeRepos,sizeNone);

  rect.OffsetRect(-size.cx, 0);
  mShowTypeHierarchy.Create(_T(""),getStyle(), rect, this, IDC_TYPE_HIERARCHY);
  mShowTypeHierarchy.SetBitmaps(IDB_TYPE_HIERARCHY_A, RGB(0,0,0), IDB_TYPE_HIERARCHY_F);
  mShowTypeHierarchy.SetTooltipText("Show the Type Hierarchy");
	AddControl(IDC_TYPE_HIERARCHY,sizeRepos,sizeNone);

  size = CSize(40, size.cy);
  rect.OffsetRect(-size.cx, 0);
  rect.SetRect(rect.TopLeft(), CPoint(rect.TopLeft().x + size.cx , size.cy));
  mPreviousHierarchies.Create(_T(""),WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_BITMAP, rect, this, IDC_PREVIOUS_HIERARCHIES);
  mPreviousHierarchies.SetBitmaps(IDB_PREVIOUS_HIERARCHIES_A, RGB(0,0,0), IDB_PREVIOUS_HIERARCHIES_F);
  mPreviousHierarchies.SetTooltipText("Previous Type Hierarchies");
	AddControl(IDC_PREVIOUS_HIERARCHIES,sizeRepos,sizeNone);
  
  mPreviousHierarchies.SetMenu(IDR_PREVIOUS_HIERARCHIES_POPUPMENU, *this, true);
  mPreviousHierarchies.GetMenu().RemoveMenu(0, MF_BYPOSITION);
  BCMenu *popup = mPreviousHierarchies.GetMenu().AppendODPopupMenuA("Previous Type Hierarchies");  

	AddControl(IDC_HIERARCHY,sizeResize,sizeResize);

  mShowTypeHierarchy.SetCheck(true);

  getHierarchyPanel().SetImageList(&ESBImageList::instance(), TVSIL_NORMAL);
  getHierarchyPanel().ModifyStyle(0, TVS_SHOWSELALWAYS, 0);
}
  
void HierarchyBrowserRenderer::setupMemberView()
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::setupMemberView()");

  int border = 2;
  CSize size(22,23);

  CRect hierarchyRect;
  getHierarchyPanel().GetWindowRect(&hierarchyRect);
  ScreenToClient(hierarchyRect);
  
  CRect rect(CPoint(hierarchyRect.right-size.cx, 0), CPoint(hierarchyRect.right, size.cy));
  rect.OffsetRect(-border - (4/*buttons*/*size.cx + 29/*custom-filter*/), hierarchyRect.bottom+border);
  
  mShowInheritedMembers.Create(_T(""), getStyle(), rect, this, IDC_INHERITED_MEMBERS);
  mShowInheritedMembers.SetBitmaps(IDB_INHERITED_MEMBERS_A, RGB(0,0,0), IDB_INHERITED_MEMBERS_F);
  mShowInheritedMembers.SetTooltipText("Show All Inherited Members");
	AddControl(IDC_INHERITED_MEMBERS,sizeRepos,sizeRepos);

  rect.OffsetRect(-size.cx, 0);
  mLockHierarchy.Create(_T(""), getStyle(), rect, this, IDC_LOCK_HIERARCHY);
  mLockHierarchy.SetBitmaps(IDB_LOCK_HIERARCHY_A, RGB(0,0,0), IDB_LOCK_HIERARCHY_F);
  mLockHierarchy.SetTooltipText("Lock View and Show Members in Hierarchy");
	AddControl(IDC_LOCK_HIERARCHY,sizeRepos,sizeRepos);
  mLockHierarchy.SetWindowPos(&mShowInheritedMembers, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);

  ///////member outline descriptor (icon & label)
  rect.OffsetRect(-size.cx, 0);
  GetDlgItem(IDC_OUTLINE_LABEL)->SetWindowPos(&mShowInheritedMembers, 25, rect.top, rect.right-25, rect.Height(), 0);
  AddControl(IDC_OUTLINE_LABEL, sizeResize, sizeRepos);

  int iconSize = 16;
  GetDlgItem(IDC_OUTLINE_ICON)->SetWindowPos(&mShowInheritedMembers, border, rect.top+((rect.Height()-iconSize)/2), border+iconSize, iconSize, 0);
  AddControl(IDC_OUTLINE_ICON, sizeNone, sizeRepos);

  ///////member outline view 
  CWnd *outline = const_cast<CWnd*>(dynamic_cast<CWnd const*>(&getController().getOutlineRenderer()));
  outline->SetParent(this);
  
  CRect thisWnd;
  GetClientRect(&thisWnd);    
  outline->SetWindowPos(&mLockHierarchy, 0, hierarchyRect.bottom+2, thisWnd.Width(), thisWnd.Height()-hierarchyRect.bottom-2, 0);
  
  AddControl(outline->GetDlgCtrlID(),sizeResize,sizeRepos);
  outline->ShowWindow(SW_SHOW);  
}

void HierarchyBrowserRenderer::setOutlineDescriptor(const Inheritable &i)
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::setOutlineDescriptor()");

  LabelBroker broker;
  GetDlgItem(IDC_OUTLINE_LABEL)->SetWindowText(FullQualifiedName(broker.getLabel(i)).name().c_str());

  int icon = ESBImageList::instance().getIcon(i);
  mOutlineIcon.SetIcon(ESBImageList::instance().ExtractIcon(icon));
}

BOOL HierarchyBrowserRenderer::OnInitDialog() 
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::OnInitDialog()");

  RaisableDialog::OnInitDialog();

  AllowSizing(sizeResize,sizeResize);
  HideSizeIcon();

  setupHierarchyView();
  setupMemberView();  

  setFocusableWindow(&getHierarchyPanel());

	return TRUE;
}

void HierarchyBrowserRenderer::render()
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::render()");
  
  getHierarchyPanel().SetRedraw(false);
  getHierarchyPanel().DeleteAllItems();
   
  mProgress.ShowWindow(SW_SHOW);

  render(getController().getHierarchyOf(NULL), TVI_ROOT);
  
  mProgress.ShowWindow(SW_HIDE);
  
  getHierarchyPanel().SetFocus();
  getHierarchyPanel().SetRedraw(true);
}

void HierarchyBrowserRenderer::render(const std::list<smart_ptr<MetaObject> > &elements, HTREEITEM parent)
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::render(MetaObject, HTREEITEM)");

  mProgress.mProgressbar.StepIt();
  int pos = mProgress.mProgressbar.GetPos();
  int lower, upper;
  mProgress.mProgressbar.GetRange(lower, upper);
  mProgress.mProgressbar.SetRange(lower, upper+elements.size());
  mProgress.mProgressbar.SetStep(1);

  LabelBroker broker;
  typedef std::list<smart_ptr<MetaObject> > TagList;

  for(TagList::const_iterator element = elements.begin(); element != elements.end(); element++) {
    
    mProgress.SetWindowText(("Loading " + (*element)->getFQN().toString() + " Hierarchy").c_str());
    
    HTREEITEM x = getHierarchyPanel().InsertItem(broker.getLabel(*(*element)).c_str(), parent);
    int icon = ESBImageList::instance().getIcon(**element);
    getHierarchyPanel().SetItemImage(x, icon, icon);
    getHierarchyPanel().SetItemData(x, (long)element->get());  
    
    int select = getController().getSelectionState(**element);
    if(select & 0x02) {
      icon = getSelectedIcon(**element);
      getHierarchyPanel().SetItemImage(x, icon, icon);
    }
    if(select & 0x01) {
      //setCaption(("Hierachy " + broker.getLabel(*(*element))).c_str());
      getHierarchyPanel().SelectItem(x);
    }

    render(getController().getHierarchyOf(element->get()), x);
    getHierarchyPanel().Expand(x, TVE_EXPAND);

    mProgress.mProgressbar.StepIt();
  }
}

void HierarchyBrowserRenderer::onShowTypeHierarchy()
{
  const static string function("HierarchyBrowserRenderer::onShowTypeHierarchy()");
  Tracer trc(NO_LABEL, RENDERER, function);
  
  try {
    if(emulateRadioButton(mShowTypeHierarchy, mShowSubtypeHierarchy, mShowSupertypeHierarchy)) {
      getController().hierarchyFilterChanged(eFullHierarchy);
    }

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void HierarchyBrowserRenderer::onShowSubtypeHierarchy()
{
  const static string function("HierarchyBrowserRenderer::onShowSubtypeHierarchy()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    if(emulateRadioButton(mShowSubtypeHierarchy, mShowTypeHierarchy, mShowSupertypeHierarchy)) {
      getController().hierarchyFilterChanged(eSubtypesOnly);
    }

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void HierarchyBrowserRenderer::onShowSupertypeHierarchy()
{
  const static string function("HierarchyBrowserRenderer::onShowSupertypeHierarchy()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    if(emulateRadioButton(mShowSupertypeHierarchy, mShowTypeHierarchy, mShowSubtypeHierarchy)) {
      getController().hierarchyFilterChanged(eSupertypesOnly);
    }
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void HierarchyBrowserRenderer::onFocusOn()
{
  const static string function("HierarchyBrowserRenderer::onFocusOn()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    getController().focusOn(getController().getSelection());
    
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void HierarchyBrowserRenderer::onFocusOnX()
{
  const static string function("HierarchyBrowserRenderer::onFocusOnX()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    getController().focusOn(NULL);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

int HierarchyBrowserRenderer::getSelectedIcon(const MetaObject &tag)
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::getSelectedIcon()");

  IconDescriptor descriptor;
  const_cast<MetaObject&>(tag).acceptVisitor(&descriptor);
  descriptor.decorators.isSelected = true;
  
  return ESBImageList::instance().getIcon(descriptor);
}

void HierarchyBrowserRenderer::onLockHierarchy()
{
  const static string function("HierarchyBrowserRenderer::onLockHierarchy()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    bool isLocked = mLockHierarchy.GetCheck() == 1;
    HTREEITEM item = getHierarchyPanel().GetSelectedItem(); 
    
    if(isLocked && item) {
      MetaObject *tag = TagRetriever(&getHierarchyPanel()).getTag(item);
      int icon = getSelectedIcon(*tag);
      getHierarchyPanel().SetItemImage(item, icon, icon);
    }

    getController().hierarchyLockChanged(isLocked);
    
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}
 
void HierarchyBrowserRenderer::onShowInheritedMembers()
{
  const static string function("HierarchyBrowserRenderer::onShowInheritedMembers()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    getController().memberFilterChanged(mShowInheritedMembers.GetCheck() == 1);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

bool HierarchyBrowserRenderer::emulateRadioButton(CButtonST& check, CButtonST &uncheck1, CButtonST &uncheck2)
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::emulateRadioButton()");

  uncheck1.SetCheck(false);
  uncheck2.SetCheck(false);

  if(check.GetCheck()) {
    return true;

  } else {
    check.SetCheck(true);
    return false;
  }
}

HierarchyViewType HierarchyBrowserRenderer::getSelectedHierachyType()
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::getSelectedHierachyType()");

  if(mShowTypeHierarchy.GetCheck()) {
    return eFullHierarchy;

  }else if(mShowSupertypeHierarchy.GetCheck()) {
    return eSupertypesOnly;
    
  }else if(mShowSubtypeHierarchy.GetCheck()) {
    return eSubtypesOnly;
  }
  //this should never happen !
  throw runtime_error("no hierarchy type selected ?!?!");
}

void HierarchyBrowserRenderer::show()
{
  ShowWindow(SW_SHOW);
	//getController().hierarchyFilterChanged(getSelectedHierachyType());
}


MouseEvent HierarchyBrowserRenderer::makeMouseEvent(CWnd *wnd, NMHDR* pNMHDR)
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::makeMouseEvent()");

  MSG msg;
  if(!PeekMessage(&msg, *this, WM_MOUSEFIRST , WM_MOUSELAST , PM_NOREMOVE)){
    msg.wParam = 0;
  }

  MouseEvent::MouseButton button = MouseEvent::eLeftButton;
  size_t clickCount = 0;
  switch(pNMHDR->code) {
    case NM_CLICK:   button = MouseEvent::eLeftButton;  clickCount = 1; break;
    case NM_DBLCLK:  button = MouseEvent::eLeftButton;  clickCount = 2; break;
    case NM_RCLICK:  button = MouseEvent::eRightButton; clickCount = 1; break;
    case NM_RDBLCLK: button = MouseEvent::eRightButton; clickCount = 2; break;
  }
  return MouseEvent(MouseEvent::eClicked, button | msg.wParam, wnd, GetMessagePos(), clickCount);
}

void HierarchyBrowserRenderer::onMouseClickedHierarchy(NMHDR* pNMHDR, LRESULT* pResult) 
{
  const static string function("HierarchyBrowserRenderer::onMouseClickedHierarchy()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    *pResult = 0;
    mouseClicked(makeMouseEvent(&getHierarchyPanel(), pNMHDR));

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void HierarchyBrowserRenderer::mouseClicked(const MouseEvent &e) 
{
  Tracer trc(NO_LABEL, RENDERER, "HierarchyBrowserRenderer::mouseClicked(MouseEvent)");

  TagRetriever retriever(&getHierarchyPanel());
  MetaObject *tag = retriever.getTagAt(e.getPoint(), true);
  
  getController().hierarchySelectionChanged(tag, e);

  if(e.getButton() == MouseEvent::eMiddleButton) {
    CMenu menu;
    menu.LoadMenu(IDR_HIERARCHY_BROWSER_POPUPMENU);
    CMenu* pPopup = menu.GetSubMenu(0);
    
    if(tag == NULL) {
      HTREEITEM ti = getHierarchyPanel().GetSelectedItem();
      tag = reinterpret_cast<MetaObject*>(getHierarchyPanel().GetItemData(ti));
    }

    LabelBroker broker;
    string label = "Focus on '" + broker.getLabel(*tag) + "'";
    pPopup->ModifyMenu(ID_HIERARCHY_FOCUSON, 0, ID_HIERARCHY_FOCUSON, label.c_str());
    pPopup->EnableMenuItem(ID_HIERARCHY_FOCUSONX, MF_GRAYED);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN, e.getPoint().first, e.getPoint().second, this);    
  }
}

void HierarchyBrowserRenderer::onExpandHierarchy(NMHDR* pNMHDR, LRESULT* pResult) 
{
  const static string function("HierarchyBrowserRenderer::onExpandHierarchy()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    NM_TREEVIEW* tree = (NM_TREEVIEW*)pNMHDR;
	
    CPoint pos = /*tree->ptDrag;//*/GetMessagePos();
    getHierarchyPanel().ScreenToClient(&pos);

    UINT flags;
	  HTREEITEM htItem = getHierarchyPanel().HitTest(pos, &flags);

    //*pResult = flags & TVHT_ONITEMBUTTON ? 0 : 1/*no expand*/;
    *pResult = flags & TVHT_ONITEMICON || flags & TVHT_ONITEMLABEL ? 1 : 0;
    
  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void HierarchyBrowserRenderer::onHistory(UINT id)
{
  const static string function("HierarchyBrowserRenderer::onHistory()");
  Tracer trc(NO_LABEL, RENDERER, function);

  try {
    getController().historyEntrySelectionChanged(id-FIRST_HISTORY_ENTRY);

  } catch(std::exception &e) {
    log(NO_LABEL, RENDERER, eError) << function << endl << e << endlog;

  } catch(...) {
    log(NO_LABEL, RENDERER, eError) << function << ": unknown exception occurred" << endlog;
  }
}

void HierarchyBrowserRenderer::setHistory(const std::list<smart_ptr<Inheritable> > &history)
{  
  const static string function("HierarchyBrowserRenderer::setHistory()");
  Tracer trc(NO_LABEL, RENDERER, function);

  BCMenu *popup = dynamic_cast<BCMenu*>(mPreviousHierarchies.GetMenu().GetSubMenu(0));
  for(size_t cnt = popup->GetMenuItemCount(); cnt > 0; --cnt) {
    popup->RemoveMenu(0, MF_BYPOSITION);
  }

  LabelBroker broker;

  typedef std::list<smart_ptr<Inheritable> > History;
  int i = 0;
  for(History::const_iterator x = history.begin(); x != history.end(); ++x, ++i) {
    int selected = getController().getSelectionState(**x);
    int icon = selected ? getSelectedIcon(**x) : ESBImageList::instance().getIcon(**x);
    popup->AppendMenu(MF_OWNERDRAW, FIRST_HISTORY_ENTRY+i, broker.getLabel(**x).c_str(), &ESBImageList::instance(), icon);
  } 
}

void HierarchyBrowserRenderer::onGetToolInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
  const static string function("HierarchyBrowserRenderer::onGetToolInfo()");
  Tracer trc(NO_LABEL, RENDERER, function);

  NMTVGETINFOTIP *pInfoTip = (NMTVGETINFOTIP*)pNMHDR;
  *pResult = 1;

  MetaObject *object = TagRetriever(&getHierarchyPanel()).getTag(pInfoTip->hItem);
  if(object && PropertiesManager::instance().getProperty(Browsers::SHOW_TOOLTIPS, "1") == "1") {
    static string hack;
    hack = object->getFQN().toString();
    pInfoTip->pszText = (char*)hack.c_str();
    *pResult = 0;
  }
}
