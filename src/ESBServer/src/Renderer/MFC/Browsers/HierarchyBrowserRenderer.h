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

#ifndef HierarchyBrowserRenderer_H
#define HierarchyBrowserRenderer_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <Renderer/MFC/3rdParty/BtnST.h>
#include <Renderer/MFC/3rdParty/xySplitterWnd.h>
#include <Renderer/MFC/Base/CustomDialog.h>
#include <Renderer/MFC/Progressbar.h>
#include <Utilities/smart_ptr.h>


class BCMenu;
class MetaObject;
class Inheritable;
class HierarchyBrowser;

enum HierarchyViewType { eFullHierarchy, eSubtypesOnly, eSupertypesOnly };

class HierarchyBrowserRenderer : public RaisableDialog
{
private:
  CButtonST mShowTypeHierarchy;
  CButtonST mShowSubtypeHierarchy;
  CButtonST mShowSupertypeHierarchy;
  CButtonST mPreviousHierarchies;

  CButtonST mLockHierarchy;
  CButtonST mShowInheritedMembers;

  CySplitterWnd mSplitter;

  HierarchyBrowser *mController;
  Progressbar mProgress;

public:
  void show();
  void render();

  void setHistory(const std::list<smart_ptr<Inheritable> > &);
  void setOutlineDescriptor(const Inheritable &);

protected:
  void mouseClicked(const MouseEvent&);

private:
  CTreeCtrl& getHierarchyPanel() { return mHierarchyRenderer; }
  HierarchyBrowser& getController() { return *mController; };
  
  void render(const std::list<smart_ptr<MetaObject> >&, HTREEITEM);

  bool emulateRadioButton(CButtonST& check, CButtonST &uc1, CButtonST &cu2);
  HierarchyViewType getSelectedHierachyType();
  
  void onShowTypeHierarchy();
  void onShowSubtypeHierarchy();
  void onShowSupertypeHierarchy();
  void onLockHierarchy();
  void onShowInheritedMembers();
  
  void setupHierarchyView();
  void setupMemberView();

  
  size_t getStyle();
  int getSelectedIcon(const MetaObject&);

  BCMenu* getSubMenu(size_t pos);
  MouseEvent makeMouseEvent(CWnd*, NMHDR*);

  afx_msg void onHistory(UINT);

public:
	HierarchyBrowserRenderer(HierarchyBrowser *controller, CWnd* pParent = NULL);

	//{{AFX_DATA(HierarchyBrowserRenderer)
	enum { IDD = IDD_HIERARCHY_BROWSER_PANEL };
	CStatic	mOutlineIcon;
	CTreeCtrl	mHierarchyRenderer;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(HierarchyBrowserRenderer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(HierarchyBrowserRenderer)
	virtual BOOL OnInitDialog();
	afx_msg void onExpandHierarchy(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void onFocusOn();
  afx_msg void onFocusOnX();
  afx_msg void onMouseClickedHierarchy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void onGetToolInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif
