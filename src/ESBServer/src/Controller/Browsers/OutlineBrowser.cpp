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

#include <Utilities/StringTokenizer.h>
#include <Utilities/Properties.h>

#include <Controller/ESBServer.h>
#include <Controller/Browsers/OutlineBrowser.h>

#include <Renderer/MFC/Browsers/OutlineBrowserRenderer.h>
#include <Renderer/MFC/Properties/OutlinePropertyPanel.h>

#include <Utilities/RegExpr.h>

#include <Model/MetaModel/MetaModel.h>

using namespace std;

OutlineBrowser::OutlineBrowser()
: mSelection(NULL), mESBServer(mESBServer), mFilters(new MemberFilter("custom-filter"))
{
	mRenderer = smart_ptr<OutlineBrowserRenderer>(new OutlineBrowserRenderer(this));

  MemberFilter::Filter subFilter; 
  
  //public only
  {
    smart_ptr<MemberFilter> filter(new MemberFilter("public-only-filter"));
    subFilter.access |= eProtected | ePrivate;
    filter->setTypeFilter(subFilter);
    filter->setMethodFilter(subFilter);
    filter->setFieldFilter(subFilter);
    filter->enable(false);
    mFilters->next(filter);
  }

  //no fields
  {
    smart_ptr<MemberFilter> filter(new MemberFilter("no-fields-filter"));
    subFilter = MemberFilter::Filter();
    subFilter.access = UINT_MAX;
    filter->setFieldFilter(subFilter);
    filter->enable(false);
    mFilters->next(filter);
  }

  //no static
  {
    smart_ptr<MemberFilter> filter(new MemberFilter("no-static-filter"));
    subFilter = MemberFilter::Filter();
    subFilter.other |= eStatic;
    filter->setMethodFilter(subFilter);
    filter->setFieldFilter(subFilter);
    filter->enable(false);
    mFilters->next(filter);  
  }

  //custom-filter
  {
    smart_ptr<MemberFilter> filter(new MemberFilter("custom-filter"));
    filter->enable(false);
    mFilters->next(filter); 
  }

  //sorter
  {
    smart_ptr<MemberSorter> filter(new MemberSorter("sorter"));
    filter->enable(false);
    mFilters->next(filter);
  }
}

OutlineBrowser::~OutlineBrowser()
{
}

std::string OutlineBrowser::makeCaption(const std::string &name)
{
  size_t pos;
  string caption;
  if((pos = name.rfind("\\")) != string::npos) {
    caption = "FileOutline: " + name.substr(pos+1, string::npos);
  } else {
    caption = "TypeOutline: " + name;
  }

  return caption;
}

void OutlineBrowser::setModel(const FileOutlineLoader::TagList &model, const std::string &fn)
{ 
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::setModel");
  
  mModel = model; 
  mOutlineName = fn;
  setSelection(NULL);

  getRenderer().render();
  getRenderer().setCaption(makeCaption(mOutlineName));
}

void OutlineBrowser::show()
{
  getRenderer().render();
  getRenderer().setCaption(makeCaption(getModelName()));
  getRenderer().show();
}

void OutlineBrowser::moveToForeground()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::moveToForeground");
  getRenderer().moveToForeground();
}

void OutlineBrowser::setSelection(MetaObject *s) 
{ 
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::setSelection");
  mSelection = s; 
}

void OutlineBrowser::tagSelectionChanged(MetaObject *tag, const MouseEvent &e)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::tagSelectionChanged(single)");

  if(tag) {
    setSelection(tag);
    if(e.getMouseModifiers() == 0 && e.getButton() == MouseEvent::eMiddleButton) {
      getRenderer().showPopupMenu(getSelection(), e.getPoint().first, e.getPoint().second);
    }

    typedef std::list<smart_ptr<SelectionListener> >::iterator Iterator;
    for(Iterator listener = mSelectionListeners.begin(); listener != mSelectionListeners.end(); ++listener) {
      (*listener)->selectionChanged(*tag, e);
    }  
  }
}

void OutlineBrowser::tagSelectionChanged(const std::list<MetaObject*> &selection)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::tagSelectionChanged(multi)");

  typedef std::list<smart_ptr<SelectionListener> >::iterator Iterator;
  for(Iterator listener = mSelectionListeners.begin(); listener != mSelectionListeners.end(); ++listener) {
    (*listener)->selectionChanged(selection);
  }  
}

void OutlineBrowser::sortOrderChanged(bool byName)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::sortOrderChanged");

  getFilter("sorter").enable(byName ? MemberSorter::eByName : MemberSorter::eByAccess);
	getRenderer().render();
}

void OutlineBrowser::publicMemberFilterChanged(bool publicOnly)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::publicMemberFilterChanged");

  getFilter("public-only-filter").enable(publicOnly);
	getRenderer().render();
}

void OutlineBrowser::staticMemberFilterChanged(bool noStatic)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::staticMemberFilterChanged");

  getFilter("no-static-filter").enable(noStatic);
	getRenderer().render();
}

void OutlineBrowser::fieldFilterChanged(bool noFields)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::fieldFilterChanged");

  getFilter("no-fields-filter").enable(noFields);
	getRenderer().render();
}

void OutlineBrowser::customFilterEnabled(bool enable)
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::customFilterChanged");

  getFilter("custom-filter").enable(enable);
  getRenderer().render();
}

#include <Renderer/MFC/Browsers/CustomOutlineFilterRenderer.h>
bool OutlineBrowser::customFilterChanged()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::customFilterChanged");

  MemberFilter f = dynamic_cast<MemberFilter&>(getFilter("custom-filter"));
  MemberFilter::Filter tf = f.getTypeFilter();
  MemberFilter::Filter mf = f.getMethodFilter();
  MemberFilter::Filter ff = f.getFieldFilter();
  
  bool changed = CustomOutlineFilter(tf, mf, ff).DoModal() == IDOK;
  if(changed) {
    dynamic_cast<MemberFilter&>(getFilter("custom-filter")).setTypeFilter(tf);
    dynamic_cast<MemberFilter&>(getFilter("custom-filter")).setMethodFilter(mf);
    dynamic_cast<MemberFilter&>(getFilter("custom-filter")).setFieldFilter(ff);
  }

  return changed;
}

MetaModelFilter& OutlineBrowser::getFilter(const std::string &id)
{
  for(smart_ptr<MetaModelFilter> next = mFilters; !next.isNull(); next = next->next()) {
    if(next->id() == id) {
      return *next;
    }
  }

  throw invalid_argument("no filter (id=" + id + ") found");
}

std::list<smart_ptr<MetaObject> > OutlineBrowser::getChildrenOf(MetaObject *tag)
{  
  return getFilterChain()->apply(tag == NULL ? getModel() : tag->getInnerTags());
}

void OutlineBrowser::gotoHeader()
{
  //gotoFile(getSelection()->getDeclaration());
}

void OutlineBrowser::gotoSource()
{
  //gotoFile(getSelection()->getDefinition());
}

void OutlineBrowser::gotoHierarchy()
{
  getESBServer()->showHierarchy(*dynamic_cast<Inheritable*>(getSelection()));
}

void OutlineBrowser::inspect()
{
  Tracer trc(NO_LABEL, RENDERER, "OutlineBrowser::inspect");

  stringstream ss;
  if(getSelection()) {
    ss << getSelection()->getFQN() << endl
       << getSelection()->getDeclaration().getFile() << ":" << getSelection()->getDeclaration().getLine() << endl
       << getSelection()->getDefinition().getFile() << ":" << getSelection()->getDefinition().getLine();
  
  } else {
    ss << "no tag selected !!";
  }

  message_box(ss.str());
}

/*
struct KindSorter : public std::binary_function<smart_ptr<MetaObject>,smart_ptr<MetaObject>,bool>
{
	bool operator()(const first_argument_type &lhs, const second_argument_type &rhs)
	{ 
    MetaObjectType lhsType = getType(lhs);
    MetaObjectType rhsType = getType(rhs);

    if(isVariable(rhsType)) {
      return false;

    } else if(isFunction(rhsType)) {
      return isVariable(lhsType);

    } else if(isType(rhsType)) {
      return !isType(lhsType);
    }
    
    return false;
  }

private:
  MetaObjectType getType(const first_argument_type &tag)
  {
    MetaObjectTyper typer;
    tag->acceptVisitor(&typer);
    return typer.getType();    
  }

  bool isVariable(MetaObjectType type) 
  { return type == eVariable || type == eField; }

  bool isFunction(MetaObjectType type) 
  { return type == eFunction || type == eMethod; }
  
  bool isType(MetaObjectType type) 
  { return !isFunction(type) && !isVariable(type); }
};
*/



