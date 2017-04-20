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

#ifndef OutlineBrowser_H
#define OutlineBrowser_H

#include <Utilities/smart_ptr.h>
#include <Model/FileOutlineLoader.h>
#include <Model/MetaModel/MetaModelFilter.h>
#include <Controller/Browsers/SelectionListener.h>

#include <Renderer/OutlineRenderer.h>

#include "Editor.h"

class ESBServer;
class MouseEvent;


class OutlineBrowser
{
protected:
  typedef smart_ptr<MetaModelFilter> FilterChain;
	smart_ptr<OutlineRenderer> mRenderer;
	FileOutlineLoader::TagList mModel;
  FilterChain mFilters;
  std::string mOutlineName;
  MetaObject *mSelection;
  ESBServer *mESBServer;

  std::list<smart_ptr<SelectionListener> > mSelectionListeners;

public:
	OutlineBrowser();
	~OutlineBrowser();
  
  void addSelectionListener(const smart_ptr<SelectionListener>& listener)
  { mSelectionListeners.push_back(listener); }

  void removeSelectionListener(const smart_ptr<SelectionListener>& listener)
  { mSelectionListeners.remove_if(std::bind2nd(std::equal_to<smart_ptr<SelectionListener> >(), listener)); }

  const std::string& getModelName() { return mOutlineName; }
  
	FileOutlineLoader::TagList& getModel() { return mModel; }
  void setModel(const FileOutlineLoader::TagList &model, const std::string &fn = "");
  
	OutlineRenderer& getRenderer() { return *mRenderer; }
  void setRenderer(const smart_ptr<OutlineRenderer> &r) { mRenderer = r; mRenderer->registerListener(this); };

  ESBServer* getESBServer() { if(!mESBServer) {throw NullPointerException("getESBServer"); }; return mESBServer; }
  void setESBServer(ESBServer *srv) { mESBServer = srv; }

  void show();
  void moveToForeground();

  void gotoHeader();
  void gotoSource();
  void gotoHierarchy();
  void inspect();

	void tagSelectionChanged(MetaObject*,  const MouseEvent &);
  void tagSelectionChanged(const std::list<MetaObject*>&);
	
  void sortOrderChanged(bool);
	void publicMemberFilterChanged(bool);
	void staticMemberFilterChanged(bool);
	void fieldFilterChanged(bool);
	void customFilterEnabled(bool);
  bool customFilterChanged();
  
  std::list<smart_ptr<MetaObject> > getChildrenOf(MetaObject*);


protected:
	FilterChain& getFilterChain() { return mFilters; }
  MetaModelFilter& getFilter(const std::string &);

  //void gotoFile(const TagLocation&);

  void setSelection(MetaObject *s);
  MetaObject* getSelection() { return mSelection; }

  std::string makeCaption(const std::string &);

};


#endif
