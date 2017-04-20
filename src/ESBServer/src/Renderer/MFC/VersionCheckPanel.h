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

#ifndef VersionCheckPanel_H
#define VersionCheckPanel_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <Renderer/UIProperties.h>
#include <Renderer/MFC/3rdParty/HyperLink.h>

class VersionCheckPanel : public CDialog
{
private:
  typedef CDialog super;

public:
  VersionCheckPanel(std::pair<std::string, std::string> = std::pair<std::string, std::string>());   // standard constructor

  void run();
  std::string fetchVersion();
  std::pair<std::string, std::string> fetchLatestVersion(bool showProgress = false);
  
	//{{AFX_DATA(VersionCheckPanel)
	enum { IDD = IDD_VERSION_CHECKING };
	CProgressCtrl	mProgressbar;
	BOOL	mIsEnabled;
	CHyperLink	mDownloadPage;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(VersionCheckPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
  
	//{{AFX_MSG(VersionCheckPanel)
  afx_msg BOOL OnInitDialog();
	afx_msg void onDisableOnlineCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  std::pair<std::string, std::string> mLatestVersion;
};

//{{AFX_INSERT_LOCATION}}

#endif
