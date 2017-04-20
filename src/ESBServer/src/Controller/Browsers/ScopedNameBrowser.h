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

#ifndef ScopedNameBrowser_H
#define ScopedNameBrowser_H

#include <algorithm>

#include "TypeInfoLoader.h"
#include "smart_ptr.h"

class ScopedNameBrowserRenderer;


class ScopedNameBrowserItem
{
public:
  virtual std::string getNameLabel() const = 0;
  virtual int getNameIcon() const = 0;

  virtual std::string getScopeLabel() const = 0;
  virtual int getScopeIcon() const = 0;
};


class ScopedNameBrowserModel
{
public:
  typedef std::list<smart_ptr<ScopedNameBrowserItem> > DatastoreType;

protected:
  DatastoreType mDatastore;

public:
  ScopedNameBrowserModel() {}
  virtual ~ScopedNameBrowserModel() {};

  const DatastoreType& getDatastore() const
  { return mDatastore; }
  
  DatastoreType& getDatastore() 
  { return mDatastore; }  

};

class TagSelectionListener
{
public:
  virtual ~TagSelectionListener(){}
  virtual void selectionChanged(const ScopedNameBrowserItem&) = 0;
};

class ScopedNameBrowser
{
private:
  //typedef TypeBrowserModel::DatastoreType::value_type;
  typedef ScopedNameBrowserRenderer RendererType;
  typedef ScopedNameBrowserModel ModelType;
  typedef ScopedNameBrowserItem ModelItemType;

	smart_ptr<RendererType> mRenderer;
	smart_ptr<ModelType> mModel;
	ModelItemType *mSelection;

  std::list<smart_ptr<TagSelectionListener> > mSelectionListeners;
  bool mCloseOnSelection;

public:
	ScopedNameBrowser();
  ScopedNameBrowser(const std::string &);
	~ScopedNameBrowser();

  void addSelectionListener(const smart_ptr<TagSelectionListener>& listener)
  { mSelectionListeners.push_back(listener); }
  
  void removeSelectionListener(const smart_ptr<TagSelectionListener>& listener)
  { mSelectionListeners.remove_if(std::bind2nd(std::equal_to<smart_ptr<TagSelectionListener> >(), listener)); }
  
  bool show(bool modal = false);

  void setFilter(const std::string&);
  void closeOnSelection(bool b) { mCloseOnSelection = b; };

	virtual void filterChanged(const std::string&);

	virtual void typeSelected(ModelItemType*);
	virtual void typeSelectionChanged(ModelItemType*);
	virtual void qualifierSelectionChanged(ModelItemType*);

	ModelType& getModel() { return *mModel; }
	void setModel(const smart_ptr<ModelType> &model) { setSelection(0); mModel = model; }

	RendererType& getRenderer();
	void setRenderer(const smart_ptr<RendererType> &r);
  
  ModelItemType* getSelection() { return mSelection; };
  void setSelection(ModelItemType *s) { mSelection = s; };

  void moveToForeground();

protected:
  void fireTagSelectionEvent();

};

#endif
