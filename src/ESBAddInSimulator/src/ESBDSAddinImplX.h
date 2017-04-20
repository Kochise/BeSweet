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

// ESBDSAddinImpl.h : Declaration of the ESBDSAddinImpl

#ifndef __AddinSimulator_H_
#define __AddinSimulator_H_

#include "resource.h"       // main symbols
//#import "C:\Program Files\Development\Microsoft Visual Studio\Common\IDE\IDE98\DTE.TLB" raw_interfaces_only, raw_native_types, no_namespace, named_guids 

#include <stubs/ESBDSAddin.h>
#include <ComHelper.h>

/////////////////////////////////////////////////////////////////////////////
// ESBDSAddinImpl
class ATL_NO_VTABLE AddinSimulator : 
public CComObjectRootEx<CComSingleThreadModel>,
public CComCoClass<AddinSimulator, &CLSID_ESBDSAddin>,
public IDispatchImpl<IESBDSAddin, &IID_IESBDSAddin, &LIBID_ESBDSADDINLib>
{
private:
  static DWORD mClassRegistrationCookie;
  DWORD mCookie;
  
  
public:
  AddinSimulator();
  
public:  
  // IESBDSAddInImpl
  HResult gotoLine(BSTR filename, long line);
   
  static void setupComStuff();
  static void tearDownComStuff();
  
public:
  //the atl crap goes here
  DECLARE_REGISTRY_RESOURCEID(IDR_ESBDSADDIN)
    
  DECLARE_CLASSFACTORY_SINGLETON(AddinSimulator);
  //DECLARE_PROTECT_FINAL_CONSTRUCT()
    
  BEGIN_COM_MAP(AddinSimulator)
    COM_INTERFACE_ENTRY(IESBDSAddin)
    COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()    
};


#endif //__ESBDSADDIN_H_
