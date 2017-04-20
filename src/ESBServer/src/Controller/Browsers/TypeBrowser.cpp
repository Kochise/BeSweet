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

#include "TypeBrowser.h"

#include <Model/Editor.h>

#include <Renderer/MFC/ESBImageList.h>
#include <Renderer/MFC/Browsers/TypeBrowserRenderer.h>

#include <sstream>
#include <iterator>

using namespace std;

TagbrowserItem::TagbrowserItem(const smart_ptr<MetaObject>& tag)
  : mTag(tag) 
{ 
}

std::string TagbrowserItem::getNameLabel() const 
{ 
  return getTag()->getFQN().name(); 
}  

std::string TagbrowserItem::getScopeLabel() const 
{ 
  string scope = getTag()->getFQN().scope();
  if(scope == GLOBAL_NAMESPACE || scope.empty()) {
    scope = "default";
  }

  string location = getTag()->getDeclaration().getFile();
  if(location.empty()) {
    location = getTag()->getDefinition().getFile();
  }
  return scope + " - " + getTag()->getDeclaration().getFile(); 
}  
  
int TagbrowserItem::getNameIcon() const 
{ 
  return ESBImageList::instance().getIcon(*getTag());
}

int TagbrowserItem::getScopeIcon() const 
{ 
  return ESBImageList::instance().getIcon(*getTag()->getScope(), true);
}

const smart_ptr<MetaObject>& TagbrowserItem::getTag() const
{
  return mTag;
}
  
TypeBrowserModel::TypeBrowserModel(const TypeInfoLoader::TypeList &tl)
{ 
  std::transform(tl.begin(), tl.end(), std::inserter(mDatastore, mDatastore.begin()), makeDataItem);
}

smart_ptr<ScopedNameBrowserItem> TypeBrowserModel::makeDataItem(const smart_ptr<MetaObject> &tag)
{ 
  return smart_ptr<TagbrowserItem>(new TagbrowserItem(tag)); 
}


MethodBrowserItem::MethodBrowserItem(const smart_ptr<Function>& function)
: TagbrowserItem(function)
{
}

int MethodBrowserItem::getScopeIcon() const 
{
  IconDescriptor desc;
  getTag()->getScope()->acceptVisitor(&desc);
  if(!getTag()->getDefinition().getLine()) {
    desc.decorators.isPrototype = true;
  }
  desc.decorators.hasProblem = false;
  desc.decorators.hasQuestion = false;
  return ESBImageList::instance().getIcon(desc);
}
  
std::string MethodBrowserItem::getScopeLabel() const 
{
  Function *f = dynamic_cast<Function*>(getTag().get());
  
  stringstream ss;
  ss << f->getReturnType() << " " << f->getFQN() << "(";
  
  transform(f->getParameters().begin(), f->getParameters().end(), ostream_iterator<string>(ss, ","), mem_fun_ref(&Field::getType));
  ss << ")";
  return ss.str();
}

MethodBrowserModel::MethodBrowserModel(const std::list<smart_ptr<Function> > &fl)
{
  std::transform(fl.begin(), fl.end(), std::inserter(mDatastore, mDatastore.begin()), makeDataItem);
}

smart_ptr<ScopedNameBrowserItem> MethodBrowserModel::makeDataItem(const smart_ptr<Function> &function)
{
  return smart_ptr<TagbrowserItem>(new MethodBrowserItem(function)); 
}
