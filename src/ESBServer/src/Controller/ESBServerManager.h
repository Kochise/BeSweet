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

// ESBServer.h : main header file for the ESBSERVER application
//

#if !defined(AFX_ESBSERVER_H__B7D8D927_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_)
#define AFX_ESBSERVER_H__B7D8D927_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include <Controller/ESBServer.h>
#include <Renderer/MFC//ESBServerManagerRenderer.h>

#include <map>
#include <string>

/////////////////////////////////////////////////////////////////////////////
// ESBServer:
// See ESBServer.cpp for the implementation of this class
//

class ESBServerManager : public CWinApp
{
private:
  typedef std::list<std::pair<std::string, smart_ptr<ESBServer> > > ServerContainerType;
  ServerContainerType mWorkspaceServers;
  smart_ptr<ESBServerManagerRenderer> mRenderer;
  
public:
  static ESBServerManager& instance();
  ESBServer& getWorkspaceServer(const std::string&);
  ESBServer& newWorkspaceServer(const std::string&);
  bool removeWorkspaceServer(std::string);

public:
	//{{AFX_VIRTUAL(ESBServer)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(ESBServer)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    ESBServerManager();
    ServerContainerType::iterator find(const std::string &);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESBSERVER_H__B7D8D927_E2DD_11D6_A215_0080C72FE4DC__INCLUDED_)
