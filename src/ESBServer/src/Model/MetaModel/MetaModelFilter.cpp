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

#include <stdafx.h>
#include "MetaModelFilter.h"

#include <Model/MetaModel/MetaModel.h>
#include <Utilities/RegExpr.h>

using namespace std;

MetaModelFilter::~MetaModelFilter()
{
}

void MetaModelFilter::next(const smart_ptr<MetaModelFilter> &n) 
{
  if(next().isNull()) {
    mNext = n;
  } else {
    mNext->next(n);
  }
}
  
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
MemberSorter::~MemberSorter()
{
}

std::list<smart_ptr<MetaObject> > MemberSorter::apply(const std::list<smart_ptr<MetaObject> > &elements)
{
  if(!isEnabled()) {
    return elements;
  }
  
  std::list<smart_ptr<MetaObject> > fields = selectTags(elements, eField | eVariable);
  std::list<smart_ptr<MetaObject> > methods =  selectTags(elements, eMethod | eFunction);;
  std::list<smart_ptr<MetaObject> > types = selectTags(elements, eNamespace | eClass | eStruct | eUnion | eEnum | eTypedef);

  switch(getOrder()) {
    case eByName :
      {
        NameSorter<MetaObject> sorter;
        fields.sort(sorter);
        methods.sort(sorter);
        types.sort(sorter);
        break;
      }
    case eByAccess:
      {
        AccessSorter<MetaObject> sorter;
        fields.sort(sorter);
        methods.sort(sorter);
        types.sort(sorter);
      }
  }

  std::list<smart_ptr<MetaObject> > result;
  result.assign(fields.begin(), fields.end());
  result.splice(result.end(), methods);
  result.splice(result.end(), types);

  return result;
}

std::list<smart_ptr<MetaObject> > MemberSorter::selectTags(const std::list<smart_ptr<MetaObject> >& tags, size_t type) const
{
  MetaObjectTyper tagTyper;
  std::list<smart_ptr<MetaObject> > result;
  typedef std::list<smart_ptr<MetaObject> > TagList;
  for(TagList::const_iterator tag = tags.begin(); tag != tags.end(); tag++) {
    (*tag)->acceptVisitor(&tagTyper);
    if(tagTyper.getType() & type) {
      result.push_back(*tag);
    }
  }

  return result;
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
MemberFilter::~MemberFilter()
{
}

bool MemberFilter::isFiltered(const CppType &object)
{
	MetaObjectTyper typer;
  const_cast<CppType&>(object).acceptVisitor(&typer);

  return object.getAccessQualifier() & mTypeFilter.access ||
         typer.getType() & mTypeFilter.other ||
         !RegExpr(mTypeFilter.regex).hasMatch(object.getFQN().name());
}

bool MemberFilter::isFiltered(const Field &field)
{
	return (field.getAccessQualifier() & mFieldFilter.access) ||
         ((mFieldFilter.other & eStatic) && field.isStatic()) ||
         ((mFieldFilter.other & eConst) && field.isConst()) ||
         !RegExpr(mFieldFilter.regex).hasMatch(field.getFQN().name());
}

bool MemberFilter::isFiltered(const Method &method)
{
	return ((mMethodFilter.other & eStatic) && method.isStatic()) ||
         ((mMethodFilter.other & eConst) && method.isConst()) ||
         (mMethodFilter.other & method.getImplementationQualifier()) ||
				 (method.getAccessQualifier() & mMethodFilter.access) ||
         !RegExpr(mMethodFilter.regex).hasMatch(method.getFQN().name());
}


std::list<smart_ptr<MetaObject> > MemberFilter::apply(const std::list<smart_ptr<MetaObject> > &elements)
{
  std::list<smart_ptr<MetaObject> > result;
  if(isEnabled()) {
    for(std::list<smart_ptr<MetaObject> >::const_iterator element = elements.begin(); element != elements.end(); ++element) {
      mChildrenVisible = false;
      (*element)->acceptVisitor(this);
      if(!isFiltered()) {
        result.push_back(*element);
      } else if(mChildrenVisible) {
        result.splice(result.end(), apply((*element)->getInnerTags()));
      }
    }
  } else {
    result = elements;
  }
  
  if(!next().isNull()) {
    result = next()->apply(result);
  }
    
  return result;
}


void MemberFilter::visitType(CppType *tag)
{
  mIsFiltered = isFiltered(*tag);
}

namespace Browsers {  namespace Outline { extern const std::string SHOW_NAMESPACES; } }
void MemberFilter::visitNamespace(Namespace *nspace)
{
//  mIsFiltered = PropertiesManager::instance().getProperty(Browsers::Outline::SHOW_NAMESPACES, "") != "1";
  mIsFiltered = true;
  mChildrenVisible = true;
}

void MemberFilter::visitField(Field *field)
{
  mIsFiltered = isFiltered(*field);
}


void MemberFilter::visitMethod(Method *method)
{
  mIsFiltered = isFiltered(*method);
}