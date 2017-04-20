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

#ifndef AddinVC7_H
#define AddinVC7_H

#include <stdafx.h>


#include "AddinVC7.h"
#include <ComHelper.h>
#include <ObjectModelVC7/LoggerConsumerVC7.h>
#include <ObjectModelVC7/CommandHandlerVC7.h>

#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>


using namespace std;

namespace VisualStudio7
{

const wchar_t * const TOOLBAR_NAME      = L"BeSweet";

Addin::Addin()
//: mDTE(NULL), mAddin(NULL)
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::Addin");
  getEventHandler()->setAddin(this);

  //CComObject<VisualStudio7::CommandHandler>::CreateInstance(&mCommandHandler);
  //mCommandHandler->setAddIn(this);
  mCommandHandler = this;
  mCommandHandler->setAddIn(this);
}

STDMETHODIMP Addin::OnConnection(IDispatch *pApplication, AddInDesignerObjects::ext_ConnectMode mode, IDispatch *pAddIn, SAFEARRAY ** /*custom*/ )
{
  //pApplication->QueryInterface(__uuidof(EnvDTE::_DTE), (LPVOID*)&mDTE);
  //pAddIn->QueryInterface(__uuidof(EnvDTE::AddIn), (LPVOID*)&mAddin);
  mDTE = pApplication;
  mAddin = pAddIn;

  if(!_logger) {
    _logger = new VisualStudio7::LoggerConsumer(mDTE);
  }

  Tracer trc(BE_SWEET, ADDIN, "Addin::OnConnection");
  SANDBOX_BEGIN("Addin", "OnConnection");
  log(BE_SWEET, ADDIN, eDebug) << "connect mode is " << mode << endlog;

  switch(mode) {
    case AddInDesignerObjects::ext_cm_AfterStartup:
    case AddInDesignerObjects::ext_cm_Startup : registerEvents(); break;

    case 5 /*AddInDesignerObjects::ext_cm_UISetup*/: registerCommands(); break;

    default: 
      {
        stringstream ss;
        ss << "unknown connection mode " << mode << endl;
        ::MessageBox(NULL, ss.str().c_str(), "", MB_OK);
      }
  }

  return S_OK;

  SANDBOX_END()

  return E_FAIL;
}

STDMETHODIMP Addin::OnDisconnection(AddInDesignerObjects::ext_DisconnectMode mode, SAFEARRAY ** /*custom*/ )
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::OnDisconnection");
  log(BE_SWEET, ADDIN, eDebug) << "disconnect mode is " << mode << endlog;

  SANDBOX_BEGIN("Addin", "OnBeginShutdown")
    if(mode != 2) { //2 == AddInDesignerObjects::ext_dm_UISetupComplete)
      deregisterEvents();
      mDTE = CComQIPtr<EnvDTE::_DTE>();
      mAddin = CComQIPtr<EnvDTE::AddIn>();
    }
    
  SANDBOX_END()
 
  return S_OK;
}

STDMETHODIMP Addin::OnAddInsUpdate (SAFEARRAY ** /*custom*/ )
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::OnAddInsUpdate");
  return S_OK;
}

STDMETHODIMP Addin::OnStartupComplete (SAFEARRAY ** /*custom*/ )
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::OnStartupComplete");
  return S_OK;
}

STDMETHODIMP Addin::OnBeginShutdown (SAFEARRAY ** /*custom*/ )
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::OnBeginShutdown");
  SANDBOX_BEGIN("Addin", "OnBeginShutdown")
    delete _logger;
   _logger = 0;
  SANDBOX_END()
  return  S_OK;;
}


CComPtr<EnvDTE::Command> Addin::newCommand(const Cmd &cmd, CComPtr<EnvDTE::Commands> &commands)
{
  Tracer trc(BE_SWEET, ADDIN, "VisualStudio7::Addin::newCommand");
  
  CComPtr<EnvDTE::Command> command;
  HRESULT hr = commands->Item(CComVariant(cmd.full_name.c_str()), 0, &command);

  // remove command if it's already there
  if (SUCCEEDED(hr)  &&  command){
    command->Delete();
    command = NULL;
  }

  // add our command
  command = NULL;
  command.p = NULL;

  log(BE_SWEET, ADDIN, eDebug) << "creating command " << string(bstr_t(cmd.full_name.c_str())) << endlog;
  cex_ = commands->AddNamedCommand(
    mAddin, 
    CComBSTR(cmd.name.c_str()), 
    CComBSTR(cmd.text.c_str()), 
    CComBSTR(cmd.tooltip.c_str()), 
    VARIANT_FALSE, 
    cmd.bitmap,  // MSO: 127
    NULL, 
    EnvDTE::vsCommandStatusSupported+EnvDTE::vsCommandStatusEnabled, 
    &command);

  log(BE_SWEET, ADDIN, eDebug) << "..command creation succeeded " << endlog;

  return command;
}

CComPtr<Office::CommandBar> Addin::getToolbar()
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::getToolbar");

  CComPtr<EnvDTE::Commands> pCommands;
  cex_ = mDTE->get_Commands(&pCommands);

  CComPtr<Office::_CommandBars> pCommandBars;
  cex_ = mDTE->get_CommandBars(&pCommandBars);

  // Get our toolbar
  CComQIPtr<Office::CommandBar> toolbar;
  HRESULT hr = pCommandBars->get_Item(CComVariant(TOOLBAR_NAME), &toolbar);
  if (!SUCCEEDED(hr)) {
    // haven't created the toolbar yet
    CComPtr<IDispatch> pDisp;
    cex_ = pCommands->AddCommandBar(bstr_t(TOOLBAR_NAME), EnvDTE::vsCommandBarTypeToolbar, NULL, 0, &pDisp);        
    toolbar = pDisp;	
  }
  return toolbar;
}

void Addin::registerCommands()
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::registerCommands");

  CComPtr<EnvDTE::Commands> commands;
  cex_ = mDTE->get_Commands(&commands);

  // Get our toolbar
  CComPtr<Office::CommandBar> toolbar = getToolbar();
  
  vector<Cmd> command = getCommandHandler()->getCommands();
  for(size_t i = 0; i < command.size(); ++i){
    log(BE_SWEET, ADDIN, eDebug) << "registering command " << i + 1 << "/" << command.size() /*<< " (" << command[0].full_name << ")"*/ << endlog;
    CComPtr<EnvDTE::Command> cmd = newCommand(command[i], commands);

    // this causes a memory leak -- don't know why
    CComPtr<Office::CommandBarControl> pCommandBarControl;
    cex_ = cmd->AddControl(toolbar, (long)i+1, &pCommandBarControl);

    // make sure it's image only
    CComQIPtr<Office::_CommandBarButton> pButton(pCommandBarControl);
    if (pButton) {
      pButton->put_Style(Office::msoButtonIcon);
    }
  }

  log(BE_SWEET, ADDIN, eDebug) << "commands successfuly registered" << endlog;
  toolbar->put_Visible(VARIANT_TRUE);
}

//CComPtr<EnvDTE::_WindowEvents> windowEvents;
//CComPtr<EnvDTE::_SolutionEvents> solutionEvents;
//CComPtr<EnvDTE::_DocumentEvents> documentEvents;

void Addin::registerEvents()
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::registerEvents");

  CComPtr<EnvDTE::Events> pEvents;
  cex_ = mDTE->get_Events(&pEvents);

  EnvDTE::_WindowEvents *windowEvents;
  cex_ = pEvents->get_WindowEvents(NULL, &windowEvents);
  dynamic_cast<EventHandler::WindowEventHandler*>(getEventHandler())->DispEventAdvise(windowEvents);

  EnvDTE::_SolutionEvents *solutionEvents;
  cex_ = pEvents->get_SolutionEvents(&solutionEvents);
  dynamic_cast<EventHandler::SolutionEventHandler*>(getEventHandler())->DispEventAdvise(solutionEvents);

  EnvDTE::_DocumentEvents *documentEvents;
  cex_ = pEvents->get_DocumentEvents(NULL, &documentEvents);
  dynamic_cast<EventHandler::DocumentEventHandler*>(getEventHandler())->DispEventAdvise(documentEvents);

  log(BE_SWEET, ADDIN, eDebug) << "registerEvents() succeeded" << endlog;
}

void Addin::deregisterEvents()
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::deregisterEvents");
  
  CComPtr<EnvDTE::Events> pEvents;
  cex_ = mDTE->get_Events(&pEvents);

  EnvDTE::_SolutionEvents *solutionEvents;
  cex_ = pEvents->get_SolutionEvents(&solutionEvents);
  dynamic_cast<EventHandler::SolutionEventHandler*>(getEventHandler())->DispEventUnadvise(solutionEvents);

  EnvDTE::_DocumentEvents *documentEvents;
  cex_ = pEvents->get_DocumentEvents(NULL, &documentEvents);
  dynamic_cast<EventHandler::DocumentEventHandler*>(getEventHandler())->DispEventUnadvise(documentEvents);

  EnvDTE::_WindowEvents *windowEvents;
  cex_ = pEvents->get_WindowEvents(NULL, &windowEvents);
  dynamic_cast<EventHandler::WindowEventHandler*>(getEventHandler())->DispEventUnadvise(windowEvents);
}

void Addin::gotoLine(const std::string &filename, long line)
{
  Tracer trc(BE_SWEET, ADDIN, "Addin::gotoLine");
  log(BE_SWEET, ADDIN, eInfo) << "open file \"" << filename << "\" at line " << line << endlog;

    //CComPtr<EnvDTE::Documents> documents;
    //cex_ = mDTE->get_Documents(&documents);

    //CComPtr<EnvDTE::Document> document;
    //cex_ = documents->Open(filename, CComBSTR(EnvDTE::vsViewKindTextView), VARIANT_FALSE, &document);

    CComPtr<EnvDTE::Window> window;
    cex_ = mDTE->OpenFile(CComBSTR(EnvDTE::vsViewKindTextView), CComBSTR(filename.c_str()), &window);
    cex_ = window->Activate();

    CComPtr<EnvDTE::Document> document;
    cex_ = mDTE->get_ActiveDocument(&document);
    
    CComPtr<IDispatch> tmp;
    cex_ = document->get_Selection(&tmp);

    CComQIPtr<EnvDTE::TextSelection> selection(tmp);
    cex_ = selection->GotoLine(line, VARIANT_TRUE);

}


} //namespace VisualStudio7
