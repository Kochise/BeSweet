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

#include <stdafx.h>


#include "CommandHandlerVC7.h"
#include <ObjectModelVC7/AddinVC7.h>

#include "ComHelper.h"

#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>


using namespace std;


namespace VisualStudio7
{

Cmd cmds_[] = { 
    Cmd(L"BeSweet.Addin.vc7.properties", L"properties", L"Properties",L"Properties", IDB_PROPERTIES),
    Cmd(L"BeSweet.Addin.vc7.outline", L"outline", L"Outline",L"Show Outline", IDB_OUTLINE),
    Cmd(L"BeSweet.Addin.vc7.hierarchy", L"hierarchy", L"Hierarchy",L"Show Hierarchy", IDB_HIERARCHY),
    Cmd(L"BeSweet.Addin.vc7.types", L"types", L"Types",L"Show Types", IDB_TYPES),
    Cmd(L"BeSweet.Addin.vc7.methods", L"methods", L"Methods",L"Show Methods", IDB_METHODS),
    Cmd(L"BeSweet.Addin.vc7.workspace", L"workspace", L"Workspace",L"Show Workspace", IDB_WORKSPACE)
};

struct CommandFinder : public binary_function<Cmd, wstring, bool>
{
  result_type operator()(const first_argument_type &cmd, const second_argument_type &fullname) const
  { return cmd.full_name == fullname; }
};

CommandHandler::CommandHandler()
: mCommands(cmds_, cmds_ + (sizeof(cmds_)/sizeof(Cmd)))
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio7::CommandHandler::CommandHandler");
}


STDMETHODIMP CommandHandler::QueryStatus(BSTR cmdName, EnvDTE::vsCommandStatusTextWanted NeededText, EnvDTE::vsCommandStatus *pStatusOption, VARIANT *pvarCommandText)
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio7::CommandHandler::QueryStatus");
  SANDBOX_BEGIN("VisualStudio7::CommandHandler", "QueryStatus")
  log(BE_SWEET, COMMAND, eDebug) << "quering for " << string(bstr_t(cmdName)) << endlog;

  if(NeededText == EnvDTE::vsCommandStatusTextWantedNone)
  {
    vector<Cmd>::const_iterator cmd = findCommand(cmdName);
    if( cmd != getCommands().end()) {
      *pStatusOption = (EnvDTE::vsCommandStatus)(EnvDTE::vsCommandStatusEnabled | EnvDTE::vsCommandStatusSupported);
      if(!getAddIn()->isEnabled() && distance(getCommands().begin(), cmd) > 0) {
        *pStatusOption = (EnvDTE::vsCommandStatus)0;
      }

    } else {
      log(BE_SWEET, COMMAND, eError) << "unknown command " << string(bstr_t(cmdName)) << endlog;
    }
  }

  SANDBOX_END()
  return S_OK;
}


STDMETHODIMP CommandHandler::Exec(BSTR cmdName, EnvDTE::vsCommandExecOption option, VARIANT * /*pvarVariantIn*/, VARIANT * /*pvarVariantOut*/, VARIANT_BOOL *isHandled)
{
  Tracer trc(BE_SWEET, COMMAND, "VisualStudio7::CommandHandler::Exec");
  SANDBOX_BEGIN("VisualStudio7::CommandHandler", "Exec")
  
  log(BE_SWEET, COMMAND, eDebug) << "executing command " << string(bstr_t(cmdName)) << endlog;
   
  *isHandled = VARIANT_FALSE;

  if(option == EnvDTE::vsCommandExecOptionDoDefault)
  {
    vector<Cmd>::const_iterator cmd = findCommand(cmdName);
    if( cmd != getCommands().end()) {
      *isHandled = VARIANT_TRUE;
      if(distance(getCommands().begin(),cmd) > 0) {
        showBrowser(string(bstr_t(cmd->name.c_str())));
      } else {
        properties();
      }
    } else {
      log(BE_SWEET, COMMAND, eError) << "unknown command " << string(bstr_t(cmdName)) << endlog;
    }
  }

  SANDBOX_END()
  return S_OK;
}

std::vector<Cmd>::const_iterator CommandHandler::findCommand(const BSTR &name)
{
  wstring wname = bstr_t(name);
  return find_if(getCommands().begin(), getCommands().end(), bind2nd(CommandFinder(), wname));
}

std::string CommandHandler::getSelectedText()
{
  SANDBOX_BEGIN("VisualStudio7::CommandHandler", "getTextSelection")

    CComPtr<EnvDTE::Document> document;
    cex_ = dynamic_cast<VisualStudio7::Addin*>(getAddIn())->getApplication()->get_ActiveDocument(&document);

    CComPtr<IDispatch> sel;
    cex_ = document->get_Selection(&sel);
    CComQIPtr<EnvDTE::TextSelection> selection(sel);
    
    BSTR text;
    cex_ = selection->get_Text(&text);

    return string(bstr_t(text));

  SANDBOX_END()

  return string();
}

} //namespace VisualStudio7
