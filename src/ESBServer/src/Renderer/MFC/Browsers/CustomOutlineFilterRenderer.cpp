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
#include "CustomOutlineFilterRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable:4800)

/////////////////////////////////////////////////////////////////////////////
// CustomOutlineFilter dialog


CustomOutlineFilter::CustomOutlineFilter(MemberFilter::Filter &tf, MemberFilter::Filter &mf, MemberFilter::Filter &ff, CWnd* pParent)
	: CDialog(CustomOutlineFilter::IDD, pParent), mType(tf), mMethod(mf), mField(ff)
{
	//{{AFX_DATA_INIT(CustomOutlineFilter)
	//}}AFX_DATA_INIT
}


void CustomOutlineFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CustomOutlineFilter)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CustomOutlineFilter, CDialog)
	//{{AFX_MSG_MAP(CustomOutlineFilter)
  ON_BN_CLICKED(IDC_TYPE_PUBLIC, onType)
  ON_BN_CLICKED(IDC_METHOD_PUBLIC, onMethod)
  ON_BN_CLICKED(IDC_FIELD_PUBLIC, onField)
  ON_BN_CLICKED(IDC_TYPE_PROTECTED, onType)
  ON_BN_CLICKED(IDC_TYPE_PRIVATE, onType)
  ON_BN_CLICKED(IDC_TYPE_CLASS, onType)
  ON_BN_CLICKED(IDC_TYPE_STRUCT, onType)
  ON_BN_CLICKED(IDC_TYPE_UNION, onType)
  ON_BN_CLICKED(IDC_TYPE_ENUM, onType)
  ON_BN_CLICKED(IDC_TYPE_TYPEDEF, onType)
  ON_BN_CLICKED(IDC_METHOD_PROTECTED, onMethod)
  ON_BN_CLICKED(IDC_METHOD_PRIVATE, onMethod)
  ON_BN_CLICKED(IDC_METHOD_CONST, onMethod)
  ON_BN_CLICKED(IDC_METHOD_STATIC, onMethod)
  ON_BN_CLICKED(IDC_METHOD_ABSTRACT, onMethod)
  ON_BN_CLICKED(IDC_METHOD_VIRTUAL, onMethod)
  ON_BN_CLICKED(IDC_FIELD_PROTECTED, onField)
  ON_BN_CLICKED(IDC_FIELD_PRIVATE, onField)
  ON_BN_CLICKED(IDC_FIELD_CONST, onField)
  ON_BN_CLICKED(IDC_FIELD_STATIC, onField)
	ON_EN_CHANGE(IDC_TYPES, onTypesChanged)
	ON_EN_CHANGE(IDC_METHODS, onMethodsChanged)
	ON_EN_CHANGE(IDC_FIELDS, onFieldsChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CustomOutlineFilter::OnInitDialog()
{
  CDialog::OnInitDialog();

  check(mType.access & ePublic, IDC_TYPE_PUBLIC);
  check(mType.access & eProtected, IDC_TYPE_PROTECTED);
  check(mType.access & ePrivate, IDC_TYPE_PRIVATE); 
  check(mType.other & eClass, IDC_TYPE_CLASS);
  check(mType.other & eStruct, IDC_TYPE_STRUCT);
  check(mType.other & eUnion, IDC_TYPE_UNION);
  check(mType.other & eEnum, IDC_TYPE_ENUM);
  check(mType.other & eTypedef, IDC_TYPE_TYPEDEF);
  SetDlgItemText(IDC_TYPES, mType.regex.c_str());

  check(mMethod.access & ePublic, IDC_METHOD_PUBLIC);
  check(mMethod.access & eProtected, IDC_METHOD_PROTECTED);
  check(mMethod.access & ePrivate, IDC_METHOD_PRIVATE); 
  check(mMethod.other & eConst, IDC_METHOD_CONST);
  check(mMethod.other & eStatic, IDC_METHOD_STATIC);
  check(mMethod.other & eVirtual, IDC_METHOD_VIRTUAL);
  check(mMethod.other & eAbstract, IDC_METHOD_ABSTRACT);
  SetDlgItemText(IDC_METHODS, mMethod.regex.c_str());

  check(mField.access & ePublic, IDC_FIELD_PUBLIC);
  check(mField.access & eProtected, IDC_FIELD_PROTECTED);
  check(mField.access & ePrivate, IDC_FIELD_PRIVATE); 
  check(mField.other & eConst, IDC_FIELD_CONST);
  check(mField.other & eStatic, IDC_FIELD_STATIC);
  SetDlgItemText(IDC_FIELDS, mField.regex.c_str());

  HICON icon = AfxGetApp()->LoadIcon(IDI_FILTER);
  SetIcon(icon, FALSE);
  SetIcon(icon, TRUE);
  
  SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
  return FALSE;
}

void CustomOutlineFilter::check(bool doCheck, UINT id) 
{
  static_cast<CButton*>(GetDlgItem(id))->SetCheck(!doCheck);
}

bool CustomOutlineFilter::isChecked(UINT id) 
{
  return static_cast<CButton*>(GetDlgItem(id))->GetCheck() != TRUE;
}

void CustomOutlineFilter::mark(size_t &dest, bool set, size_t flag)
{
  if(set) {
    dest |= flag;

  } else if(dest & flag) {
    dest ^= flag;
  }
}

void CustomOutlineFilter::onType()
{
  markTypeAccess(isChecked(IDC_TYPE_PUBLIC), ePublic);
  markTypeAccess(isChecked(IDC_TYPE_PROTECTED), eProtected);
  markTypeAccess(isChecked(IDC_TYPE_PRIVATE), ePrivate);

  markTypeKind(isChecked(IDC_TYPE_CLASS), eClass);
  markTypeKind(isChecked(IDC_TYPE_STRUCT), eStruct);
  markTypeKind(isChecked(IDC_TYPE_UNION), eUnion);
  markTypeKind(isChecked(IDC_TYPE_ENUM), eEnum);
  markTypeKind(isChecked(IDC_TYPE_TYPEDEF), eTypedef);
}

void CustomOutlineFilter::markTypeAccess(bool set, size_t flag)
{
  mark(mType.access, set, flag);
}

void CustomOutlineFilter::markTypeKind(bool set, size_t flag)
{
  mark(mType.other, set, flag);
}

void CustomOutlineFilter::onMethod()
{
  markMethodAccess(isChecked(IDC_METHOD_PUBLIC), ePublic);
  markMethodAccess(isChecked(IDC_METHOD_PROTECTED), eProtected);
  markMethodAccess(isChecked(IDC_METHOD_PRIVATE), ePrivate);
  
  markMethodImplementation(isChecked(IDC_METHOD_CONST), eConst);
  markMethodImplementation(isChecked(IDC_METHOD_STATIC), eStatic);
  markMethodImplementation(isChecked(IDC_METHOD_VIRTUAL), eVirtual);
  markMethodImplementation(isChecked(IDC_METHOD_ABSTRACT), eAbstract);
}

void CustomOutlineFilter::markMethodAccess(bool set, size_t flag)
{
  mark(mMethod.access, set, flag);
}

void CustomOutlineFilter::markMethodImplementation(bool set, size_t flag)
{
  mark(mMethod.other, set, flag);
}

void CustomOutlineFilter::onField() 
{
  markFieldAccess(isChecked(IDC_FIELD_PUBLIC), ePublic);
  markFieldAccess(isChecked(IDC_FIELD_PROTECTED), eProtected);
  markFieldAccess(isChecked(IDC_FIELD_PRIVATE), ePrivate);

  markFieldImplementation(isChecked(IDC_FIELD_CONST), eConst);
  markFieldImplementation(isChecked(IDC_FIELD_STATIC), eStatic);
}

void CustomOutlineFilter::markFieldAccess(bool set, size_t flag)
{
  mark(mField.access, set, flag);
}

void CustomOutlineFilter::markFieldImplementation(bool set, size_t flag)
{
  mark(mField.other, set, flag);
}


void CustomOutlineFilter::onTypesChanged() 
{
	mType.regex = freeTextFilterChanged(IDC_TYPES);
}

void CustomOutlineFilter::onMethodsChanged() 
{
  mMethod.regex = freeTextFilterChanged(IDC_METHODS);	
}

void CustomOutlineFilter::onFieldsChanged() 
{
	mField.regex = freeTextFilterChanged(IDC_FIELDS);	
}

std::string CustomOutlineFilter::freeTextFilterChanged(UINT id)
{
  CString s;
  GetDlgItemText(id, s);
  return s.GetBuffer(0);
}
