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

#ifndef ESBServerManagerRenderer_H
#define ESBServerManagerRenderer_H

#include <Renderer/MFC/LoggerPanel.h>

#include <Renderer/MFC/3rdParty/OptionPage/OptionSheet.h>
#include <Renderer/MFC/3rdParty/OptionPage/OptionListBox.h>
#include <Renderer/MFC/3rdParty/SystemTray.h>

class ESBServerManagerRenderer : public COptionSheet
{
public:
  ESBServerManagerRenderer();

protected:
  BOOL OnInitDialog();
  void OnCancel();
  LRESULT onSystemTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void onMaximize();
	afx_msg void onMinimize();
  afx_msg void onAbout();
	afx_msg void onPreferences();
	afx_msg void onExit();
  afx_msg void onLogWindow();
  afx_msg LRESULT onCloseWorkspace(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
  COptionListBox mListbox;
  smart_ptr<LoggerPanel> mLoggerPanel;

  CSystemTray mSystemTray;
  CSystemTray& getSystemTray() { return mSystemTray; }

};


#endif