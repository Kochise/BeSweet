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
#include "resource.h"

#include "HierarchyBrowser.h"

#include <Model/Editor.h>
#include <Model/MetaModel/MetaModelUtilities.h>

#include <Controller/Browsers/TypeBrowser.h>

#include <Renderer/MFC/Browsers/HierarchyBrowserRenderer.h>
#include <Renderer/UIProperties.h>

using std::max;
using namespace std;

HierarchyBrowser::HierarchyBrowser()
: mTypeSelection(NULL), mHierarchyType(eFullHierarchy), 
  mShowInheritedMembers(false), mIsHierarchyLocked(false)
{

  mOutlineListener = make_smart_ptr(new OutlineListener(this));
  mHierarchySelectionStrategy = make_smart_ptr(new HierarchyListener(this));
  
  mForwarder = make_smart_ptr(new EventForwarder(this));
  mMemberOutline.addSelectionListener(mForwarder);
  
  //mRenderer = smart_ptr<HierarchyBrowserRenderer>(new HierarchyBrowserRenderer(this));
  getRenderer();
}

bool HierarchyBrowser::show(bool isModal)
{    
  if(isModal) {
    return getRenderer().DoModal() != 0;

  } else {
    getRenderer().show(); 
    return true;
  }
}

const OutlineRenderer& HierarchyBrowser::getOutlineRenderer()
{
  return getMemberOutline().getRenderer();
}

void HierarchyBrowser::setModel(const smart_ptr<Inheritable>& model)
{
  mModel = model;
  if(model.isNull()) {
    return;
  }

  if(find_if(mHistory.begin(), mHistory.end(), bind2nd(FQNComparator<Inheritable>(), mModel)) == mHistory.end()) {
    mHistory.push_front(mModel);
    if(mHistory.size() > 10) {
      mHistory.pop_back();
    }
  }
  
  mRoot = smart_ptr<Inheritable>();
  mTypeSelection = model.get();

  updateHierarchyView();
  updateMemberOutline(getModel());
}

void HierarchyBrowser::historyEntrySelectionChanged(size_t id)
{
  History::iterator entry = mHistory.begin();
  advance(entry, id);
  setModel(*entry);
}

void HierarchyBrowser::hierarchyFilterChanged(HierarchyViewType hierarchyType)
{
  mHierarchyType = hierarchyType;
  updateHierarchyView();
}

struct extractor
{
  const smart_ptr<MetaObject>& operator()(const std::pair<long, std::list< smart_ptr<MetaObject> > > &x) const
  { 
    return x.second.front(); 
  }
};

void HierarchyBrowser::hierarchySelectionChanged(MetaObject *tag, const MouseEvent &e)
{
  if(!tag) {
    return;
  }
  
  Inheritable *inheritable = dynamic_cast<Inheritable*>(tag);
  if(inheritable) {
    mTypeSelection = inheritable;
  }

  mHierarchySelectionStrategy->selectionChanged(*tag, e);
}

void HierarchyBrowser::updateHierarchyView()
{
  getRenderer().setCaption("Hierachy: " + getModel().getFQN().name());
  getRenderer().setHistory(mHistory);
  getRenderer().render();
}

void HierarchyBrowser::updateMemberOutline(const Inheritable &inheritable)
{
  map<long, list<smart_ptr<MetaObject> > > members = inheritable.getMembers(mShowInheritedMembers);

  list<smart_ptr<MetaObject> > result;
  transform(members.begin(), members.end(), back_inserter(result), extractor());

  getMemberOutline().setModel(result);
  getRenderer().setOutlineDescriptor(inheritable);
}


void HierarchyBrowser::hierarchyLockChanged(bool isLocked)
{
  mIsHierarchyLocked = isLocked;
  swapMemberOutlineListeners(isLocked);
  
  if(isLocked) {
    mHierarchySelectionStrategy = mForwarder;
  } else {
    mMembers.clear();
    mHierarchySelectionStrategy = make_smart_ptr(new HierarchyListener(this));
  }
  updateHierarchyView();
}

void HierarchyBrowser::focusOn(Inheritable *inheritable)
{
  if(inheritable != NULL) {
    MetaObjectCloner cloner;
    inheritable->acceptVisitor(&cloner);
    
    smart_ptr<Inheritable> clone = dynamic__cast<Inheritable>(cloner.getClone());
    clone->setInnerTags(list<smart_ptr<MetaObject> >());
    clone->setParents(list<smart_ptr<Inheritable> >());
    clone->setExtendors(list<smart_ptr<Inheritable> >());
    setModel(clone);

  } else {
    TagBrowser typeBrowser("Type");
    typeBrowser.show(true);
  }
}
  
void HierarchyBrowser::swapMemberOutlineListeners(bool isLocked)
{
  if(isLocked) {
    mMemberOutline.removeSelectionListener(mForwarder);
    mMemberOutline.addSelectionListener(mOutlineListener);
    
  } else {
    mMemberOutline.removeSelectionListener(mOutlineListener);
    mMemberOutline.addSelectionListener(mForwarder);
  }
}

void HierarchyBrowser::memberFilterChanged(bool show)
{
  mShowInheritedMembers = show;
  updateMemberOutline(*mTypeSelection);
}

HierarchyBrowserRenderer& HierarchyBrowser::getRenderer()
{ 
  if(mRenderer.isNull()) {
    mRenderer = make_smart_ptr(new HierarchyBrowserRenderer(this));
    if(!getRenderer().m_hWnd) {
      getRenderer().Create(IDD_HIERARCHY_BROWSER_PANEL,CWnd::GetDesktopWindow());
    }    
  }
 
  return *mRenderer; 
}

smart_ptr<Inheritable> HierarchyBrowser::getRoot() const
{
  if(mRoot.isNull()) {
    RootFinder::RootMap result;
    result.swap(RootFinder().findRootOf(mModel));

    /* debug code
    stringstream ss;
    ss << "map size = " << result.size() << endl
       << "root-dist = " << result.rbegin()->first << endl
       << "root list size" << result.rbegin()->second.size() << endl << endl;

    for(RootFinder::RootMap::iterator x = result.begin(); x != result.end(); ++x) {
      ss << "dist = " << x->first << endl;
      ss << "list size = " << x->second.size() << endl;
      for(list<smart_ptr<Inheritable> >::const_iterator y = x->second.begin(); y != x->second.end(); ++y) {
        ss << (*y)->getFQN().toString() << endl;
      }
      ss << endl << endl;
    }
    message_box(ss.str().c_str());*/
  
    if(result.size() > 1 && PropertiesManager::instance().getProperty(Browsers::Hierarchy::WARN_MULTIPLE_PARENTS, "1") == "1") {
      message_box(getModel().getFQN().toString() + " has multiple parents...\nonly the longest path is displayed");
    }  

    mRoot = result.rbegin()->second.front();
  }
  return mRoot;
}

std::list<smart_ptr<MetaObject> > HierarchyBrowser::getHierarchyOf(MetaObject *element) const
{
  std::list<smart_ptr<MetaObject> > elements;
  Inheritable *inheritable = dynamic_cast<Inheritable*>(element);

  if(element == NULL && mHierarchyType == eFullHierarchy) {
    elements.push_front(getRoot());
    
  } else if(element == NULL) {
    elements.push_front(mModel);

  } else if(inheritable != NULL) {
    elements.swap(getHierarchyOf(*inheritable));
  }
  
  return elements;
}

template<class T1, class T2>
struct fqnIsEqual : public std::binary_function<smart_ptr<T1>, T2,bool>
{
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getFQN().toString() == rhs->getFQN().toString(); }
};


std::list<smart_ptr<MetaObject> > HierarchyBrowser::getHierarchyOf(const Inheritable &inheritable) const
{
  std::list<smart_ptr<MetaObject> > elements;

  switch(mHierarchyType) {
    case eFullHierarchy :
      {
        copy_if(inheritable.getExtendors().begin(), inheritable.getExtendors().end(), back_inserter(elements), bind2nd(IsRelativeOf(), mModel)); 
        //HACK:
        elements.remove_if(bind2nd(fqnIsEqual<MetaObject, const Inheritable*>(), &inheritable));
        log(NO_LABEL, CONTROLLER, eDebug) << inheritable.getFQN() << " has " << elements.size() << " extendors: " << endl;
        for(std::list<smart_ptr<MetaObject> >::iterator i = elements.begin(); i != elements.end(); ++i){
          _log_stream << (*i)->getFQN() << endl;        
        }
        _log_stream << endlog;
      }
      break;
    
    case eSubtypesOnly:      
      elements.assign(inheritable.getExtendors().begin(), inheritable.getExtendors().end());
      break;
    
    case eSupertypesOnly:
      elements.assign(inheritable.getParents().begin(), inheritable.getParents().end());
      break;
  }
  
  for(list<MetaObject*>::const_iterator member = mMembers.begin(); member != mMembers.end(); ++member) {
    list<smart_ptr<MetaObject> >::const_iterator x;
    typedef hash_comparator<true, const MetaObject*, smart_ptr<MetaObject> > weak_hash_is_equal;
    x = find_if(inheritable.getInnerTags().begin(), inheritable.getInnerTags().end(), bind1st(weak_hash_is_equal(), *member));
    if(x != inheritable.getInnerTags().end()) {
      elements.push_front(*x);
    }
  } 

  
  return elements;
}

int HierarchyBrowser::getSelectionState(const MetaObject &tag) const
{
  Inheritable const *inheritable = dynamic_cast<Inheritable const *>(&tag);
  
  /* 0 = nothing, 1 = select, 2 = icon*/
  int select = 0;

  if(inheritable == NULL) {
    return select;
  }

  if(inheritable->equals(*mTypeSelection)) {
    select |= 0x01;
  }
  
  if(inheritable->equals(getModel())) {
    select |= 0x02;
  }

  return select;
}

void HierarchyBrowser::fireSelectionChanged(const MetaObject& tag, const MouseEvent &e) const
{
  typedef std::list<smart_ptr<SelectionListener> >::const_iterator Iterator;
  for(Iterator listener = mSelectionListeners.begin(); listener != mSelectionListeners.end(); ++listener) {
    (*listener)->selectionChanged(tag, e);
  }  
}


HierarchyBrowser::OutlineListener::OutlineListener(HierarchyBrowser *b) 
: mBrowser(b) 
{
}

void HierarchyBrowser::OutlineListener::selectionChanged(const std::list<MetaObject*> &selection) const
{
  mBrowser->mMembers = selection;
  mBrowser->updateHierarchyView();
}

HierarchyBrowser::HierarchyListener::HierarchyListener(HierarchyBrowser *b)
: mBrowser(b)
{
}

void HierarchyBrowser::HierarchyListener::selectionChanged(const MetaObject &tag, const MouseEvent &e) const
{
  const Inheritable *inheritable = dynamic_cast<const Inheritable*>(&tag);
  if(inheritable && e.getButton() == MouseEvent::eLeftButton) {
    mBrowser->updateMemberOutline(*inheritable);
    
  } else if(e.getButton() == MouseEvent::eRightButton) {
    mBrowser->fireSelectionChanged(tag, e);
  }
}

HierarchyBrowser::EventForwarder::EventForwarder(HierarchyBrowser *b)
: mBrowser(b)
{
}

void HierarchyBrowser::EventForwarder::selectionChanged(const MetaObject &tag, const MouseEvent &e) const
{
  mBrowser->fireSelectionChanged(tag, e);
}
