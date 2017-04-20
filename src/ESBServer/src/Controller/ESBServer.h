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

#ifndef ESBServerXX_H
#define ESBServerXX_H

#include "smart_ptr.h"

#include "ESBServerCtrlPanel.h"

#include <Controller/Browsers/OutlineBrowser.h>
#include <Controller/Browsers/HierarchyBrowser.h>
#include <Controller/Browsers/TypeBrowser.h>
#include <Controller/Browsers/WorkspaceBrowser.h>

#include "Workspace.h"

#include "CTagsDatabase.h"
#include "CTagsTypeInfoLoader.h"
#include "CTagsMethodLoader.h"
#include "CTagsFileOutlineLoader.h"

class ESBServer
{
private:
  class EditorHandler : public DefaultSingleSelectionListener, public TagSelectionListener
  {
  private:
    ESBServer *mOuter;
  public:
    EditorHandler(ESBServer *outer) : mOuter(outer){};
    void selectionChanged(const ScopedNameBrowserItem&);
    void gotoFile(const TagLocation &) const;
  };
  friend class EditorHandler;

  class Resolver : public AmbiguityResolver
  {
  private:
    mutable ESBServer *mOuter;
  public:
    Resolver(ESBServer *outer) : mOuter(outer){};
    smart_ptr<MetaObject> resolve(const std::list<smart_ptr<MetaObject> > &) const;
  };
  friend class Resolver;
  
private:
  std::string mName;
  
  ESBServerControlPanel mRenderer;
  OutlineBrowser  mOutlineBrowser[2];
  HierarchyBrowser  mHierarchyBrowser;
  TagBrowser     mTypeBrowser;
  TagBrowser     mMethodBrowser;
  WorkspaceBrowser mWorkspaceBrowser;
  
  smart_ptr<Workspace> mWorkspace;
  smart_ptr<EditorBroker> mEditorBroker;


public:
  ESBServer(const std::string &wsname);
  ~ESBServer();

  void showFileOutline(const std::string&, bool enforceModelReload = false);
  void showHierarchy(const Inheritable&);
  void showTypes();
  void showMethods();
  void showWorkspace();
  void showBrowser(const std::string&, const std::string&);

  void reparseFile(const std::string&);
  void reparseWorkspace();
  void openWorkspace(const Workspace&);

  CWnd* getRenderer() { return &mRenderer; };
  void run();

  const std::string& getName() const { return mName; };
  std::string getShortName() const;
  
  void setEditorBroker(smart_ptr<EditorBroker> eb) { mEditorBroker = eb; };
  EditorBroker&  getEditorBroker() { return *mEditorBroker; }
  
private:
  void showTypeOutline(const std::string&);
  void showHierarchy(const std::string&);
  smart_ptr<CppType> getType(const std::string&);
  
  OutlineBrowser& getFileOutlineBrowser() { return mOutlineBrowser[0]; }
  OutlineBrowser& getTypeOutlineBrowser() { return mOutlineBrowser[1]; }
  HierarchyBrowser& getHierarchyBrowser() { return mHierarchyBrowser; }
  TagBrowser& getTypeBrowser() { return mTypeBrowser; }
  TagBrowser& getMethodBrowser() { return mMethodBrowser; }
  WorkspaceBrowser& getWorkspaceBrowser() { return mWorkspaceBrowser; }

  void setWorkspace(const smart_ptr<Workspace> &w) { mWorkspace = w; }
  smart_ptr<Workspace> getWorkspace() { return mWorkspace; }

  
  smart_ptr<TypeInfoLoader> getTypeInfoLoader();
  smart_ptr<MethodLoader> getMethodLoader();
  smart_ptr<FileOutlineLoader> getFileOutlineLoader();

};

#endif
