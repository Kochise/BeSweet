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

#if !defined(AFX_OutlineBrowserRenderer_H__8E19DBE0_E434_11D6_A215_0080C72FE4DC__INCLUDED_)
#define AFX_OutlineBrowserRenderer_H__8E19DBE0_E434_11D6_A215_0080C72FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutlineBrowserRenderer.h : header file
//

#include <Renderer/MFC/Base/CustomDialog.h>
#include <Renderer/MFC/3rdParty/BtnST.h>
#include <Renderer/MFC/3rdParty/MltiTree.h>

#include <Renderer/OutlineRenderer.h>
#include <Renderer/MFC/Base/MouseEventDispatcher.h>

#include <map>


/////////////////////////////////////////////////////////////////////////////
// OutlineBrowserRenderer dialog

class OutlineBrowserRenderer : public MouseEventDispatcher<CStatic>, public OutlineRenderer, public MouseListener
{
private:
  typedef MouseEventDispatcher<CStatic> super;
  typedef std::list<smart_ptr<MetaObject> > TagList;

  OutlineBrowser *mController;
  
  CRect mOld;
  CMultiTree mOutline;
  CButtonST mSorted;
  CButtonST mNoFields;
  CButtonST mNoStatic;
  CButtonST mPublicOnly;
  CButtonST mCustomFilter;

  CToolTipCtrl mTooltip;
    
public:
	void render();
  void registerListener(OutlineBrowser *listener) { mController = listener; }
  

  void show();
  void hide();
  void moveToForeground() { /*RaisableDialog::moveToForeground();*/ };

  void setCaption(const std::string &text);
  std::string getCaption();
  
  std::pair<short, short> getPosition();
  void setPosition(short x, short y);

  void showPopupMenu(MetaObject*, short x, short y);

protected:
  virtual void render(const TagList &elements, HTREEITEM parent);

private:
	OutlineBrowser& getController() { return *mController; }
	CMultiTree& getOutlinePanel() { return mOutline; }

  void onSort();
  void onPublicOnly();
  void onNoStatic();
  void onNoFields();
  void onCustomFilter();

  void mouseClicked(const MouseEvent&);

  size_t getStyle();
  

  void initialize();
  void moveChild(const CWnd&, int x, int y, HDWP);

// mfc crap follows here
public:
	OutlineBrowserRenderer(OutlineBrowser*, CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(OutlineBrowserRenderer)
	enum { IDD = IDD_OUTLINE_BROWSER  };
	//}}AFX_DATA

protected:
	//{{AFX_MSG(OutlineBrowserRenderer)
	afx_msg void onGotoHeader();
	afx_msg void onGotoSource();
	afx_msg void onInspectTag();
  afx_msg void onShowHierarchy();
  afx_msg void OnSize (UINT nType, int cx, int cy); 
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct ); 
//}}AFX_MSG

  afx_msg void  onMouseClicked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void onGetToolTip(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg int onToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
//OutlineBrowserDialog
///////////////////////////////// 
class OutlineBrowserDialog : public OutlineRenderer, public RaisableDialog
{
private:
  typedef RaisableDialog super;
  OutlineBrowserRenderer mOutline;

public:
  OutlineBrowserDialog();

  // OutlineRenderer implementation 
  void registerListener(OutlineBrowser *listener) 
  { mOutline.registerListener(listener); }

  virtual void render() 
  { mOutline.render(); }

  virtual void show();
  virtual void hide();
  virtual void close();
  virtual void moveToForeground();

  virtual void setCaption(const std::string &c)  { super::setCaption(c); };
  virtual std::string getCaption()  { return super::getCaption(); };
  
  virtual std::pair<short, short> getPosition() { return super::getPosition(); };
  virtual void setPosition(short x, short y) { super::setPosition(x,y); };

  virtual void showPopupMenu(MetaObject *tag, short x, short y) { mOutline.showPopupMenu(tag, x, y); };

private:
  BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OutlineBrowserRenderer_H__8E19DBE0_E434_11D6_A215_0080C72FE4DC__INCLUDED_)
