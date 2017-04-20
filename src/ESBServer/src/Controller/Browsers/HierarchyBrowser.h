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

#ifndef HierarchyBrowser_H
#define HierarchyBrowser_H

#include <Model/HierarchyLoader.h>
#include <Model/MetaModel/MetaModelUtilities.h>

#include <Controller/Browsers/OutlineBrowser.h>
#include <Controller/Browsers/SelectionListener.h>

#include <Renderer/MFC/Browsers/HierarchyBrowserRenderer.h>

#include <smart_ptr.h>

class HierarchyBrowserRenderer;
class HierarchyBrowser;
class MouseEvent;

class HierarchyBrowser
{
public:
  typedef std::list<smart_ptr<Inheritable> > History;
  
private:
  class OutlineListener : public MultiSelectionListener
  {
  private:
    HierarchyBrowser *mBrowser;
  
  public:
    OutlineListener(HierarchyBrowser *b);
    void selectionChanged(const std::list<MetaObject*> &selection) const;
  };
  friend class OutlineListener;
  
  class HierarchyListener : public DefaultSingleSelectionListener
  {
  private:
    typedef DefaultSingleSelectionListener super;
    HierarchyBrowser *mBrowser;

  public:
    HierarchyListener(HierarchyBrowser *b);
    void selectionChanged(const MetaObject&, const MouseEvent&) const;
  };
  friend class HierarchyListener;

  class EventForwarder : public SingleSelectionListener
  {
  private:
    HierarchyBrowser *mBrowser;
    
  public:
    EventForwarder(HierarchyBrowser *b);
    void selectionChanged(const MetaObject&, const MouseEvent&) const;
  };
  friend class EventForwarder;
  

private:
  smart_ptr<Inheritable> mModel;
  History mHistory;
  mutable smart_ptr<Inheritable> mRoot; //lazy initialized
  
  smart_ptr<HierarchyBrowserRenderer> mRenderer;

  OutlineBrowser mMemberOutline;
  std::list<MetaObject*> mMembers;

  Inheritable *mTypeSelection;
  HierarchyViewType  mHierarchyType;

  bool mShowInheritedMembers;
  bool mIsHierarchyLocked;

  smart_ptr<EventForwarder> mForwarder;
  smart_ptr<MultiSelectionListener> mOutlineListener;
  
  smart_ptr<SelectionListener> mHierarchySelectionStrategy;

  std::list<smart_ptr<SelectionListener> > mSelectionListeners;
  
public:
  HierarchyBrowser();

  bool show(bool isModal = false);
  
  void setModel(const smart_ptr<Inheritable> &);

  void addSelectionListener(const smart_ptr<SingleSelectionListener>& listener)
  { mSelectionListeners.push_back(listener); }
  
  void removeSelectionListener(const smart_ptr<SingleSelectionListener>& listener)
  { mSelectionListeners.remove_if(std::bind2nd(std::equal_to<smart_ptr<SelectionListener> >(), listener)); }

  Inheritable* getSelection() { return mTypeSelection; }

  void hierarchyFilterChanged(HierarchyViewType hierarchyType);
  void hierarchySelectionChanged(MetaObject*, const MouseEvent&);

  void hierarchyLockChanged(bool);
  void memberFilterChanged(bool);
  void historyEntrySelectionChanged(size_t id);
  void focusOn(Inheritable*);

  std::list<smart_ptr<MetaObject> > getHierarchyOf(MetaObject*) const;
  std::list<smart_ptr<MetaObject> > getHierarchyOf(const Inheritable&) const;
  int getSelectionState(const MetaObject&) const;
  const OutlineRenderer& getOutlineRenderer();

  HierarchyBrowserRenderer& getRenderer();

private:
  void updateMemberOutline(const Inheritable&);
  void updateHierarchyView();

  std::list<std::list<smart_ptr<MetaObject> > > getMembersOf(const CppType&);

  Inheritable& getModel() { return *mModel; }
  const Inheritable& getModel() const { return *mModel; }

  OutlineBrowser& getMemberOutline() { return mMemberOutline; }

  void swapMemberOutlineListeners(bool);

  smart_ptr<Inheritable> getRoot() const;

  void fireSelectionChanged(const MetaObject& tag, const MouseEvent &e) const;

  //void setSelection(Inheritable *s) { mTypeSelection = s; }
  
};

#endif