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

#if !defined(AFX_TypeBrowserRenderer_H__B7D8D931_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_)
#define AFX_TypeBrowserRenderer_H__B7D8D931_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeBrowserRenderer.h : header file
//


#include <set>
#include <map>
#include <string>

//#include "TypeInfoLoader.h"
#include <Renderer/MFC/ESBImageList.h>
#include <Renderer/MFC/3rdParty/xySplitterWnd.h>

#include "CustomDialog.h"

/////////////////////////////////////////////////////////////////////////////
// TypeBrowserRenderer dialog

class ScopedNameBrowser;
class ScopedNameBrowserItem;


class ScopedNameBrowserRenderer : public RaisableDialog
{
private:
  typedef RaisableDialog super;

  ScopedNameBrowser *mController;
  CImageList *mImages;

  CySplitterWnd mSplitter;

  std::string mFilterDescription;
  std::string mNameFilter;
  
public:
  template<class InputIterType>
	void renderNames(const InputIterType &begin, const InputIterType &end)
  { 
    showIn(begin, end, getNameListRenderer(), false); 
    if(begin != end) {
      getController().typeSelectionChanged((*begin).get()); 
    }

    SetDlgItemText(IDC_MATCHES, (lexical_cast<string>(getNameListRenderer().GetItemCount()) + " Matches:").c_str());
  }

  template<class InputIterType>
  void renderScopes(const InputIterType &begin, const InputIterType &end)
  { showIn(begin, end, getScopeListRenderer(), true); }

  void setImages(CImageList &images) 
  { mImages = &images; }

  void setController(ScopedNameBrowser *c) 
  { mController = c; };
  
  void setFilterDescription(const std::string &description) 
  { mFilterDescription = description; }

  void close()
  { super::OnOK(); }

  void setFilter(const std::string &);

protected:
  void filterChanged(const std::string&);

  void OnOK();


private:
	ScopedNameBrowser& getController() { return *mController; };
  CImageList* getImages() { return mImages; }
	CListCtrl& getNameListRenderer() { return mNameListRenderer; }
	CListCtrl& getScopeListRenderer() { return mScopeListRenderer; }

	ScopedNameBrowserItem* getSelectedItem();

	bool isSelected(NMHDR*);
	ScopedNameBrowserItem* getData(const CListCtrl&, NMHDR*);

  void append(CListCtrl&, const smart_ptr<ScopedNameBrowserItem>&, bool);

	template<class InputIterType>
	void showIn(const InputIterType &begin, const InputIterType &end, CListCtrl &ctrl, bool useScopeIcon)
	{
		ctrl.SetRedraw(false);
		ctrl.DeleteAllItems();

		for(InputIterType type = begin; type != end; type++) {
			append(ctrl, *type, useScopeIcon);
		}

    ctrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
    ctrl.SetColumnWidth( 0, LVSCW_AUTOSIZE );
    ctrl.SetRedraw(true);
	}

  ScopedNameBrowserItem* getData(char key, int index, const CListCtrl &ctrl);

public:
	ScopedNameBrowserRenderer(ScopedNameBrowser*, CWnd* pParent = NULL);   // standard constructor

  CEdit mFilter;
	CListCtrl	mNameListRenderer;
	CListCtrl	mScopeListRenderer;

	//{{AFX_DATA(TypeBrowserRenderer)
	enum { IDD = IDD_SCOPEDNAME_BROWSER };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(TypeBrowserRenderer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(TypeBrowserRenderer)
	virtual BOOL OnInitDialog();
	afx_msg void onNameSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void onScopeSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void onFilterChanged();
  afx_msg void onClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void onFocusNames(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void onFocusQualifiers(NMHDR* pNMHDR, LRESULT* pResult);
  //}}AFX_MSG
  afx_msg void onNameKeyPressed(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void onQualifierKeyPressed(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#endif
