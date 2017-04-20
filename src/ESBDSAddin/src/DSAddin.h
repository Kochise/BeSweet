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

#ifndef __ESBDSADDIN_H_
#define __ESBDSADDIN_H_

#include <string>

#include "EventHandler.h"
#include "CommandHandler.h"
#include "BeSweetPreferencesPanel.h"
#include "ComHelper.h"

#include "ESBServer.h"
#include "ESBDSAddin.h"
class BeSweetAddin :public IDispatchImpl<IESBDSAddin, &IID_IESBDSAddin, &LIBID_ESBDSADDINLib>
{
private:
    BeSweetPreferencesPanel mProperties;

public:
  virtual ~BeSweetAddin(){}

  bool isEnabled() { return mProperties.enabled(); }
  void enable(bool e);

    // IESBDSAddInImpl
  HResult gotoLine(BSTR filename, long line);
  virtual void gotoLine(const std::string &, long) = 0;
  
  void modifyProperties();
  void loadWorkspace(const std::string &, const std::list<std::string> &);
  void closeWorkspace();
  void reopenWorkspace();
  void documentChanged(const std::string &);
  void documentActivated(const std::string &);

  void showBrowser(const std::string &browsers, const std::string arg);

  BeSweetPreferencesPanel& getProperties() { return mProperties; }
  bool isWorkspaceValid() { return !getProperties().getWorkspace().empty(); }

  CComPtr<IESBServer> getESBServer();
  CComPtr<IESBServerManager> getServerManager(bool start);
  bool handleDisabledState();

private:
  bool retry();

};


#endif //__ESBDSADDIN_H_
