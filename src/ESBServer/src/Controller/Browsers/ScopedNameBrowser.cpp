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

#include "ScopedNameBrowser.h"

#include <Renderer/MFC/Browsers/ScopedNameBrowserRenderer.h>
#include <Renderer/UIProperties.h>

#include "StringTokenizer.h"
#include "RegExpr.h"
#include "stl_ext.h"

using namespace std;

struct NameSorter_
{
  bool operator()(smart_ptr<ScopedNameBrowserItem> lhs, smart_ptr<ScopedNameBrowserItem> rhs)
  {  return lhs->getNameLabel() < rhs->getNameLabel(); }
};

struct ScopeSorter_
{
  bool operator()(smart_ptr<ScopedNameBrowserItem> lhs, smart_ptr<ScopedNameBrowserItem> rhs)
  {  return lhs->getScopeLabel() < rhs->getScopeLabel(); }
};


ScopedNameBrowser::ScopedNameBrowser()
: mSelection(0), mCloseOnSelection(false)
{
	mRenderer = smart_ptr<RendererType>(new RendererType(this));
}

ScopedNameBrowser::ScopedNameBrowser(const std::string &label)
: mSelection(0), mCloseOnSelection(false)
{
  mRenderer = smart_ptr<RendererType>(new RendererType(this));
  mRenderer->setCaption("Select " + label);
  
  mRenderer->setFilterDescription("Choose a " + label + " ( ? = any character, * = any string)");  
}

ScopedNameBrowser::~ScopedNameBrowser()
{
}

bool ScopedNameBrowser::show(bool isModal)
{
	if(isModal) {
    return getRenderer().DoModal() != IDCANCEL;

	} else {
    if(!getRenderer().m_hWnd) {
		  getRenderer().Create(IDD_SCOPEDNAME_BROWSER,CWnd::GetDesktopWindow());
    }
    getRenderer().renderNames(getModel().getDatastore().begin(), getModel().getDatastore().end());
		getRenderer().ShowWindow(SW_SHOW);
		return true;
	}
}

void ScopedNameBrowser::setFilter(const std::string &what)
{
  getRenderer().setFilter(what);
}

void ScopedNameBrowser::filterChanged(const std::string &newFilter)
{
  typedef std::set<smart_ptr<ModelItemType>, NameSorter_> NameSet;

	NameSet result;

  //copy_if(getModel().begin(), getModel().end(), inserter(result, result.end()), RegExpr(newFilter));
	
	RegExpr regExpr(to_lower(newFilter));
  for(ModelType::DatastoreType::const_iterator type = getModel().getDatastore().begin(); type != getModel().getDatastore().end(); type++) {
    if(regExpr.hasMatch(to_lower((*type)->getNameLabel()))) {
      result.insert(*type);
    }
  }

  getRenderer().renderNames(result.begin(), result.end());
}

void ScopedNameBrowser::typeSelected(ModelItemType *selection)
{
	setSelection(selection);
  fireTagSelectionEvent();
  if(mCloseOnSelection || PropertiesManager::instance().getProperty(Browsers::CLOSE_ON_RETURN, "") == "1") {
    getRenderer().close();
  }
}

void ScopedNameBrowser::typeSelectionChanged(ModelItemType *selection)
{
	setSelection(selection);

  typedef std::multiset<smart_ptr<ModelItemType>, ScopeSorter_> ScopeSet;

  ScopeSet scopes;

	//TypeList::iterator t = getModel().find(smart_ptr<CppType>(selection));
	//copy_if(getModel().getDatastore().begin(), getModel().getDatastore().end(), bind());
  for(ModelType::DatastoreType::iterator t = getModel().getDatastore().begin(); t != getModel().getDatastore().end(); t++) {
		if((*t)->getNameLabel() == selection->getNameLabel()) {
			scopes.insert(*t);
		}
	}

	getRenderer().renderScopes(scopes.begin(), scopes.end());
}

void ScopedNameBrowser::qualifierSelectionChanged(ModelItemType *selection)
{
	setSelection(selection);
}


ScopedNameBrowser::RendererType& ScopedNameBrowser::getRenderer() 
{ 
  return *mRenderer; 
}

void ScopedNameBrowser::setRenderer(const smart_ptr<ScopedNameBrowser::RendererType> &r) 
{ 
  mRenderer = r; 
  mRenderer->setController(this); 
}

void ScopedNameBrowser::moveToForeground()
{
  getRenderer().moveToForeground();
}

void ScopedNameBrowser::fireTagSelectionEvent()
{
  typedef std::list<smart_ptr<TagSelectionListener> >::const_iterator Iterator;
  for(Iterator listener = mSelectionListeners.begin(); listener != mSelectionListeners.end(); ++listener) {
    (*listener)->selectionChanged(*getSelection());
  }  
}
