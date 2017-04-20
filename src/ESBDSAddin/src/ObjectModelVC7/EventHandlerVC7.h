#ifndef BeSweetVC7EventHandler_H
#define BeSweetVC7EventHandler_H

//#pragma once
//#pragma warning( disable : 4278 )
//The following #import imports DTE based on it's LIBID
//#import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" version("7.0") lcid("0") raw_interfaces_only named_guids
//#pragma warning( default : 4278 )

#include <string>
#include <sstream>

#include "ComHelper.h"

namespace VisualStudio7
{

class Addin;

class EventHandler : 
            public IDispEventImpl<1, VisualStudio7::EventHandler, &__uuidof(EnvDTE::_dispDocumentEvents), &EnvDTE::LIBID_EnvDTE, 7, 0>,
            public IDispEventImpl<1, VisualStudio7::EventHandler, &__uuidof(EnvDTE::_dispWindowEvents), &EnvDTE::LIBID_EnvDTE, 7, 0>,
            public IDispEventImpl<1, VisualStudio7::EventHandler, &__uuidof(EnvDTE::_dispSolutionEvents), &EnvDTE::LIBID_EnvDTE, 7, 0>
{
public:
  typedef IDispEventImpl<1, VisualStudio7::EventHandler, &__uuidof(EnvDTE::_dispDocumentEvents), &EnvDTE::LIBID_EnvDTE, 7, 0> DocumentEventHandler;
  typedef IDispEventImpl<1, VisualStudio7::EventHandler, &__uuidof(EnvDTE::_dispWindowEvents), &EnvDTE::LIBID_EnvDTE, 7, 0> WindowEventHandler;
  typedef IDispEventImpl<1, VisualStudio7::EventHandler, &__uuidof(EnvDTE::_dispSolutionEvents), &EnvDTE::LIBID_EnvDTE, 7, 0> SolutionEventHandler;

public:
  EventHandler() : mAddin(NULL){}
  virtual ~EventHandler() { mAddin = NULL; }

  void setAddin(Addin *h) { mAddin = h; }
  Addin* getAddin() { return mAddin; }

  void __stdcall windowActivated(EnvDTE::Window *active, EnvDTE::Window *old ); 
  void __stdcall documentSaved(EnvDTE::Document* document);
  void __stdcall solutionOpened();
  void __stdcall solutionClosed();

  BEGIN_SINK_MAP(EventHandler)
    //document events
    SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispDocumentEvents), 1, documentSaved)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispDocumentEvents), 2, DocumentClosing)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispDocumentEvents), 3, DocumentOpening)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispDocumentEvents), 4, DocumentOpened)
    
    //window events
    SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispWindowEvents), 3, windowActivated)
    
    //solution events
    SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 1, solutionOpened)
    SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 2, solutionClosed)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 3, AfterClosing)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 4, QueryCloseSolution)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 5, Renamed)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 6, ProjectAdded)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 7, ProjectRemoved)
    //SINK_ENTRY_EX(1, __uuidof(EnvDTE::_dispSolutionEvents), 8, ProjectRenamed)

   END_SINK_MAP()

private:
  Addin *mAddin;
};


} //namespace VisualStudio7

#endif