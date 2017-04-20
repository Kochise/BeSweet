#ifndef VSObjectModelHelper_H
#define VSObjectModelHelper_H

#include "ComHelper.h"

#include <string>
#include <vector>

template<class PtrType>
std::string getFullName(PtrType &t)
{
  BSTR name;
  cex_ = t->get_FullName(&name);
  return string(bstr_t(name));
}

inline CComPtr<IGenericDocument> _getDocument(CComQIPtr<IGenericWindow> &window)
{
  CComPtr<IDispatch> document;
  cex_ = window->get_Parent(&document);
  return CComQIPtr<IGenericDocument>(document);
}

inline CComPtr<EnvDTE::Document> _getDocument(CComPtr<EnvDTE::Window> &window)
{
  CComPtr<EnvDTE::Document> document;
  cex_ = window->get_Document(&document);
  return document;    
}

template<class DocumentType, class WindowType> 
CComPtr<DocumentType> getDocument(WindowType &window)
{
  return _getDocument(window);
}

template<class ProjectCollectionType, class SolutionProvider>
std::vector<std::string> getProjectNames(SolutionProvider &solution)
{
  CComPtr<ProjectCollectionType::collection_type> projects;
  cex_ = solution->get_Projects(&projects);

  long cnt;
  cex_ = projects->get_Count(&cnt);
  vector<string> projectNames(cnt);
  
  for(long i =  1; i <= cnt; ++i) {
    CComPtr<ProjectCollectionType::value_type> project;
    cex_ = projects->Item(CComVariant(i), &project);
    projectNames.push_back(getFullName(project));
  }

  return projectNames;
}

  template<class WindowType>
  std::string getCaption(WindowType &window)
  {  
    BSTR bstr;
    cex_ = window->get_Caption(&bstr);
    return bstr_t(bstr);
  }

#endif