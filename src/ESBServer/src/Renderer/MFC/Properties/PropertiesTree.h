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

#ifndef PropertiesTree_H
#define PropertiesTree_H

#include <Renderer/MFC/3rdParty/OptionPage/OptionSheet.h>
#include <map>
class PropertiesTree : public CTreeCtrl, public COptionSheet::COptionList
{

public:
  virtual BOOL  Create (COptionSheet *pParentWnd, UINT nID);
  virtual BOOL  AddGroup (COptionPage *pPage);
  virtual BOOL  AddPage (COptionPage *pPage, COptionPage *pGroupPage);   
  virtual BOOL  SetSelection (COptionPage *page);
  virtual COptionPage *  GetSelection ();
  virtual CWnd *  GetControlWnd ();
  BOOL RemovePage(int index);

protected:
  COptionPage * mSelection;
  std::map<COptionPage*, HTREEITEM> mGroupMap;

  //{{AFX_MSG(PropertiesTree)
	afx_msg void onSelectionChange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

#endif
