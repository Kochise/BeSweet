#include "stdafx.h"


#include "EventHandlerVC7.h"
#include <ObjectModelVC7/AddinVC7.h>
#include <VSObjectModelHelper.h>

#include <algorithm>
#include <functional>

using namespace std;
using namespace  EnvDTE;

struct ProjectsCollection
{
  typedef EnvDTE::Projects collection_type;
  typedef EnvDTE::Project value_type;
}; 

namespace VisualStudio7
{

void __stdcall EventHandler::documentSaved(EnvDTE::Document* document)
{
  Tracer trc(BE_SWEET, EVENT, "VisualStudio7::EventHandler::documentSaved");
  SANDBOX_BEGIN("VisualStudio7::EventHandler", "documentSaved")
  getAddin()->documentChanged(getFullName(CComPtr<Document>(document)));
  SANDBOX_END()
}

void __stdcall EventHandler::windowActivated(EnvDTE::Window *active ,EnvDTE::Window *old )
{
  Tracer trc(BE_SWEET, EVENT, "VisualStudio7::EventHandler::windowActivated");
  SANDBOX_BEGIN("VisualStudio7::EventHandler", "windowActivated")
 
  CComPtr<Document> document = getDocument<Document>(CComPtr<Window>(active));
  
  if(document) {
     getAddin()->documentActivated(getFullName(document));
  }

  SANDBOX_END()
}

bool isVcprojFile(std::string name)
{
  return name.find(".vcproj") != string::npos;
}

void __stdcall EventHandler::solutionOpened()
{
  Tracer trc(BE_SWEET, EVENT, "VisualStudio7::EventHandler::solutionOpened");
  SANDBOX_BEGIN("VisualStudio7::EventHandler", "solutionOpened")

  CComPtr<_Solution> solution;
  cex_ = getAddin()->getApplication()->get_Solution(&solution);

  string solutionName = getFullName(solution);
  vector<string> projects = getProjectNames<ProjectsCollection>(solution);

  projects.erase(remove_if(projects.begin(), projects.end(), not1(ptr_fun(isVcprojFile))), projects.end());
  getAddin()->loadWorkspace(solutionName, list<string>(projects.begin(), projects.end()));
  
  SANDBOX_END()
} 

void __stdcall EventHandler::solutionClosed()
{
  Tracer trc(BE_SWEET, EVENT, "VisualStudio7::EventHandler::solutionClosed");
  SANDBOX_BEGIN("VisualStudio7::EventHandler", "solutionClosed")
  getAddin()->closeWorkspace();
  SANDBOX_END()
}

} //namespace VisualStudio7