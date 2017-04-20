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

// CommandHandler.cpp : Implementation of CommandHandler
#include "stdafx.h"
#include <stubs/ESBDSAddin.h>

#include "CommandHandlerVC6.h"
#include "AddinVC6.h"
#include "ComHelper.h"

using namespace std;

namespace VisualStudio6 {

CommandHandler::CommandHandler() 
{ 
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::CommandHandler");
}

CommandHandler::~CommandHandler() 
{ 
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::~CommandHandler");
}

std::string CommandHandler::getSelectedText()
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::getSelectedText()");
  
  string what;
  try {
    CComPtr< IDispatch> tmp = 0;
    cex_ = dynamic_cast<VisualStudio6::Addin*>(getAddIn())->getApplication()->get_ActiveDocument(&tmp);
    CComQIPtr<ITextDocument> active(tmp);
  
    tmp = 0;
    cex_ = active->get_Selection(&tmp);
    CComQIPtr<ITextSelection> selection = tmp;
  
    BSTR bstr;
    cex_ = selection->get_Text(&bstr);
    what = bstr_t(bstr);

  } catch(...) {
    log(BE_SWEET, COMMAND, eInfo) << "VisualStudio6::CommandHandler::showBrowser(): no text-editor active" << endlog;
  }

  return what;
}


HRESULT CommandHandler::setProperties()
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::setProperties");
  SANDBOX_BEGIN("VisualStudio6::CommandHandler" , "setProperties")
  
  super::properties();

  SANDBOX_END()

  return S_OK;
}

HRESULT CommandHandler::showOutlineBrowser()
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::showOutlineBrowser");
  SANDBOX_BEGIN("VisualStudio6::CommandHandler", "showOutlineBrowser")
 
  showBrowser("outline");

  SANDBOX_END()

  return S_OK;
}

HRESULT CommandHandler::showHierarchyBrowser()
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::showHierarchyBrowser");
  SANDBOX_BEGIN("VisualStudio6::CommandHandler", "showHierarchyBrowser")
  
  showBrowser("hierarchy");

  SANDBOX_END()
    
  return S_OK;
}

HRESULT CommandHandler::showTypeBrowser()
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::showTypeBrowser");
  SANDBOX_BEGIN("VisualStudio6::CommandHandler", "showTypeBrowser")
    
  showBrowser("types");

  SANDBOX_END()
    
  return S_OK;
}

HRESULT CommandHandler::showMethodBrowser()
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::showMethodBrowser");
  SANDBOX_BEGIN("VisualStudio6::CommandHandler", "showMethodBrowser")

  showBrowser("methods");

  SANDBOX_END()
    
  return S_OK;
}

HRESULT CommandHandler::showWorkspaceBrowser()
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio6::CommandHandler::showWorkspaceBrowser");
  SANDBOX_BEGIN("VisualStudio6::CommandHandler", "showWorkspaceBrowser")

  showBrowser("workspace");
  
  SANDBOX_END()
    
  return S_OK;
}

} // namespace VisualStudio6