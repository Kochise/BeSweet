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

#ifndef CustomOutlineFilter_H
#define CustomOutlineFilter_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <Controller/Browsers/OutlineBrowser.h>

class CustomOutlineFilter : public CDialog
{
public:
  CustomOutlineFilter(MemberFilter::Filter&, MemberFilter::Filter&, MemberFilter::Filter&, CWnd* pParent = NULL);

	//{{AFX_DATA(CustomOutlineFilter)
	enum { IDD = IDD_CUSTOM_OUTLINE_FILTER };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CustomOutlineFilter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CustomOutlineFilter)
  afx_msg BOOL OnInitDialog();
  afx_msg void onType();
  afx_msg void onMethod();
  afx_msg void onField();
	afx_msg void onTypesChanged();
  afx_msg void onMethodsChanged();
  afx_msg void onFieldsChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  bool isChecked(UINT);
  void check(bool, UINT);

  void mark(size_t&, bool set, size_t flag);

  void markTypeAccess(bool set, size_t flag);
  void markTypeKind(bool set, size_t flag);

  void markMethodAccess(bool set, size_t flag);
  void markMethodImplementation(bool set, size_t flag);

  void markFieldAccess(bool set, size_t flag);
  void markFieldImplementation(bool set, size_t flag);

  std::string freeTextFilterChanged(UINT);

private:
  MemberFilter::Filter &mType;
  MemberFilter::Filter &mMethod;
  MemberFilter::Filter &mField;
  
};

//{{AFX_INSERT_LOCATION}}

#endif
