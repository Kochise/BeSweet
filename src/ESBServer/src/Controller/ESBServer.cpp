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

#include "stdafx.h"
#include "Resource.h"

#include "ESBServer.h"
#include "VSWorkspaceLoader.h"

#include <Model/Simulators/HierarchyLoaderSimulator.h>
#include <Model/CTags/CTagsHierarchyLoader.h>
#include <Model/CTags/CTagsOutlineLoader.h>
#include <Renderer/MFC/Browsers/OutlineBrowserRenderer.h>

#include <algorithm>
#include <functional>

using namespace std;

void ESBServer::EditorHandler::selectionChanged(const ScopedNameBrowserItem &item)
{
  
  if(dynamic_cast<const TagbrowserItem*>(&item)) {
    const TagbrowserItem *metaObject = dynamic_cast<const TagbrowserItem*>(&item);
    if(!metaObject->getTag()->getDeclaration().getFile().empty()) {
      gotoFile(metaObject->getTag()->getDeclaration());

    } else if(!metaObject->getTag()->getDefinition().getFile().empty()) {
      gotoFile(metaObject->getTag()->getDefinition());
    }

  } else if(dynamic_cast<const WorkspaceBrowserItem*>(&item)) {    
    const WorkspaceBrowserItem *file = dynamic_cast<const WorkspaceBrowserItem*>(&item);
    gotoFile(TagLocation(file->getFile()));
  }
}

void ESBServer::EditorHandler::gotoFile(const TagLocation &location) const
{
  mOuter->getEditorBroker().getEditor(location.getFile())->goToLine(location.getLine());
}

smart_ptr<TagbrowserItem> asTagBrowserItem(smart_ptr<MetaObject> object)
{
  return smart_ptr<TagbrowserItem>(new TagbrowserItem(object));
}

smart_ptr<MetaObject> ESBServer::Resolver::resolve(const std::list<smart_ptr<MetaObject> >&symbols) const
{
  TagBrowser browser("Tags");

  smart_ptr<ScopedNameBrowserModel> model(new ScopedNameBrowserModel());
  transform(symbols.begin(), symbols.end(), back_inserter(model->getDatastore()), asTagBrowserItem);
  browser.setModel(model);
  browser.closeOnSelection(true);

  if(!browser.show(true) || browser.getSelection() == NULL) {
    return smart_ptr<MetaObject>();
  }

  return dynamic_cast<TagbrowserItem*>(browser.getSelection())->getTag();
}

ESBServer::ESBServer(const std::string &name)
: mName(name), mRenderer(this), mMethodBrowser("Method"), mTypeBrowser("Type") 
{  
  LoggerManager::instance().map(getShortName(), NO_LABEL);
 	Tracer trc(getShortName(), "Controller", "ESBServer::ESBServer()");

  getFileOutlineBrowser().setRenderer(make_smart_ptr(new OutlineBrowserDialog()));
  getFileOutlineBrowser().setESBServer(this);
  getTypeOutlineBrowser().setRenderer(make_smart_ptr(new OutlineBrowserDialog()));
  getTypeOutlineBrowser().setESBServer(this);
  
  smart_ptr<EditorHandler> listener = make_smart_ptr(new EditorHandler(this));
  getFileOutlineBrowser().addSelectionListener(listener);
  getTypeOutlineBrowser().addSelectionListener(listener);
  getHierarchyBrowser().addSelectionListener(listener);

  getTypeBrowser().addSelectionListener(listener);
  getMethodBrowser().addSelectionListener(listener);
  getWorkspaceBrowser().addSelectionListener(listener);
}

ESBServer::~ESBServer()
{
  try {
    CTags::DatabaseManager::instance().removeDatabase(getWorkspace());

  } catch(std::exception &e) {
    message_box(e.what());
  }
}

void ESBServer::run() 
{ 
  mRenderer.DoModal(); 
}

void ESBServer::showTypes() 
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showTypes()");

  smart_ptr<TypeBrowserModel> model(new TypeBrowserModel(getTypeInfoLoader()->load()));

  getTypeBrowser().setModel(model);
	getTypeBrowser().show();	
}

void ESBServer::showMethods()
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showMethods()");

 	smart_ptr<MethodBrowserModel> model(new MethodBrowserModel(getMethodLoader()->load()));

  getMethodBrowser().setModel(model);
	getMethodBrowser().show();	
}


void ESBServer::showWorkspace()
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showWorkspace()");

  smart_ptr<ScopedNameBrowserModel> model(new WorkspaceBrowserModel(*getWorkspace()));

  getWorkspaceBrowser().setModel(model);
  getWorkspaceBrowser().show();
}

void ESBServer::showFileOutline(const std::string &file, bool enforceModelReload)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showFileOutline()");

  if(!file.empty()) {
    string x = file_path_is::pathof(file) + file_name_is::nameof(file);
    string y = file_path_is::pathof(getFileOutlineBrowser().getModelName()) + file_name_is::nameof(getFileOutlineBrowser().getModelName());
    
    bool reload = enforceModelReload || x != y;

    if(reload) {
      getFileOutlineBrowser().setModel(getFileOutlineLoader()->load(file), file);
      getFileOutlineBrowser().show();
    }  
  }
}

void ESBServer::showHierarchy(const Inheritable &type)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showHierarchy()");

  MetaObjectCloner cloner;
  const_cast<Inheritable&>(type).acceptVisitor(&cloner);

  smart_ptr<Inheritable> clone = dynamic__cast<Inheritable>(cloner.getClone());
  
  getHierarchyBrowser().setModel(clone);
  getHierarchyBrowser().show();
}

void ESBServer::reparseFile(const std::string &file)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::reparseFile()");

  CTags::DatabaseManager::instance().getDatabase(getWorkspace())->refreshTagsReferencingFile(file);
  showFileOutline(file, true);

}

void ESBServer::openWorkspace(const Workspace& w)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::openWorkspace()");

  VisualStudioWorkspaceLoader workspaceLoader(w.getName());
  setWorkspace(workspaceLoader.load(w.getPackages()));
  
  LoaderBroker::instance(getWorkspace())->setOutlineLoader(make_smart_ptr(new CTags::OutlineLoader(getWorkspace())));
  
  smart_ptr<HierarchyLoader> hierarchyLoader(new CTags::HierarchyLoader(getWorkspace()));
  hierarchyLoader->setAmbiguityResolver(smart_ptr<AmbiguityResolver>(new Resolver(this)));
  LoaderBroker::instance(getWorkspace())->setHierarchyLoader(hierarchyLoader);  
}

void ESBServer::reparseWorkspace()
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::reparseWorkspace()");

  openWorkspace(*getWorkspace());
}

smart_ptr<CppType> ESBServer::getType(const std::string &type_name)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::getType()");

  TypeInfoLoader::TypeList tl = CTags::TypeInfoLoader(getWorkspace()).load(type_name);
  smart_ptr<MetaObject> item;
  
  smart_ptr<TypeBrowserModel> model;
  if(tl.size() != 1) {
    if(tl.empty()) {
 	    model = smart_ptr<TypeBrowserModel>(new TypeBrowserModel(getTypeInfoLoader()->load()));

    } else if(tl.size() > 1) {
      model = smart_ptr<TypeBrowserModel>(new TypeBrowserModel(tl));
    }
  
    TagBrowser typeBrowser("Type");
    typeBrowser.closeOnSelection(true);
    typeBrowser.setModel(model);
    //typeBrowser.setFilter("*" + type_name + "*");

    if(!typeBrowser.show(true)) {
      return smart_ptr<CppType>();
    }
    item = dynamic_cast<TagbrowserItem*>(typeBrowser.getSelection())->getTag();

  } else {
    item = *tl.begin();
  }

  return dynamic__cast<CppType>(item);
}

void ESBServer::showTypeOutline(const std::string &type_name)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showOutline()");

//  if(type_name.empty()) {
//    getFileOutlineBrowser().show();
//    getFileOutlineBrowser().moveToForeground();

//  } else {
    smart_ptr<MetaObject> type = getType(type_name);
    if(!type.isNull()) {
      list<smart_ptr<MetaObject> > outline;
      outline.push_back(type);
    
      getTypeOutlineBrowser().setModel(outline, type->getFQN().name());     
      getTypeOutlineBrowser().show();
      getTypeOutlineBrowser().moveToForeground();
    }
//  }
}

void ESBServer::showHierarchy(const std::string &type_name)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showHierarchy()");

  //if(getHierarchyBrowser().getSelection() == NULL || !type_name.empty()) {
    smart_ptr<Inheritable> type = dynamic__cast<Inheritable>(getType(type_name.empty() ? string("*") : type_name));
    if(!type.isNull()) {
      showHierarchy(*type);
    }
   
  //} else {
  //  getHierarchyBrowser().show();
  //  getHierarchyBrowser().getRenderer().moveToForeground();
  //}
}

template<class T>
void manageVisibility(T & browser, list<CWnd*> &visibles)
{
  CWnd *wnd = dynamic_cast<CWnd*>(&browser.getRenderer());
  if(wnd && wnd->IsWindowVisible()) {
    visibles.push_back(wnd);
  }
}

void ESBServer::showBrowser(const std::string &browser, const std::string &what)
{
 	Tracer trc(getShortName(), "Controller", "ESBServer::showBrowser()");

  static list<CWnd*> visibles;

  if(browser == "outline") {
    showTypeOutline(what);

  } else if(browser == "hierarchy") {
    showHierarchy(what);

  } else if(browser == "types") {
    showTypes();
    if(!what.empty()) {
      getTypeBrowser().setFilter(what);
    }
    getTypeBrowser().moveToForeground();

  } else if(browser == "methods") {
    showMethods();
    if(!what.empty()) {
      getMethodBrowser().setFilter(what);
    }
    getMethodBrowser().moveToForeground();

  } else if(browser == "workspace") {
    showWorkspace();
    getWorkspaceBrowser().moveToForeground();

  } else if(browser == "minimize") {
    
    visibles.clear();
    manageVisibility(getFileOutlineBrowser(), visibles);
    manageVisibility(getTypeOutlineBrowser(), visibles);
    manageVisibility(getHierarchyBrowser(), visibles);
    manageVisibility(getTypeBrowser(), visibles);
    manageVisibility(getMethodBrowser(), visibles);
    manageVisibility(getWorkspaceBrowser(), visibles);

    for_each(visibles.begin(), visibles.end(), bind2nd(mem_fun(&CWnd::ShowWindow), SW_HIDE));

  } else if(browser == "restore") {  
    for_each(visibles.begin(), visibles.end(), bind2nd(mem_fun(&CWnd::ShowWindow), SW_SHOW));
    visibles.clear();

  } else {
    log(getShortName(), CONTROLLER, eError) << "ESBServer::showBrowser():  unknown browser type " << browser << endlog;
  }
}

#include "Model/Simulators/TypeInfoLoaderSimulator.h"
smart_ptr<::TypeInfoLoader> ESBServer::getTypeInfoLoader()
{
  //return smart_ptr<TypeInfoLoaderSimulator>(new TypeInfoLoaderSimulator());
  return smart_ptr<::TypeInfoLoader>(new CTags::TypeInfoLoader(getWorkspace())); 
}

smart_ptr<::MethodLoader> ESBServer::getMethodLoader()
{
  return smart_ptr<::MethodLoader>(new CTags::MethodLoader(getWorkspace()));
}

smart_ptr<::FileOutlineLoader> ESBServer::getFileOutlineLoader()
{
  //return smart_ptr<FileOutlineLoaderSimulator>(new FileOutlineLoaderSimulator());
  return smart_ptr<::FileOutlineLoader>(new CTags::FileOutlineLoader(getWorkspace()));
}

std::string ESBServer::getShortName() const
{
  return file_name_is::nameof(getName());
}
