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

#include "CTagsHierarchyLoader.h"

#include <Model/LoaderBroker.h>
#include <Model/CTags/CTagsDatabase.h>
#include <Model/CTags/CTagsMetaObjectFactory.h>

#include <Utilities/StringTokenizer.h>

#include <list>
#include <sstream>
#include <iterator>

using namespace std;

namespace CTags
{
  

Inheritable& HierarchyLoader::loadHierarchy(Inheritable &type, bool recursive)
{
  type.setExtendors(loadSubtypeHierarchy(type, recursive));
  type.setParents(loadSupertypeHierarchy(type, recursive));
  return type;
}

std::list<smart_ptr<Inheritable> > HierarchyLoader::loadSubtypeHierarchy(const Inheritable &type, bool recursive)
{
  return getSubHierarchy(type.getFQN(), recursive);
}

std::list<smart_ptr<Inheritable> > HierarchyLoader::loadSupertypeHierarchy(const Inheritable &type, bool recursive)
{
  return getSuperHierarchy(type.getFQN(), recursive);
}

std::list<smart_ptr<Inheritable> > HierarchyLoader::getSuperHierarchy(const std::string &fqn, bool recursive)
{
  Tag tag;
  try {
    tag = getTag(fqn);
    
  } catch(UndefinedSymbolException &) {  
    //message_box("hello");
  }

  std::list<smart_ptr<Inheritable> > parents;
  for(list<string>::const_iterator parent = tag.getInheritance().begin(); parent != tag.getInheritance().end(); ++parent) {
    smart_ptr<Inheritable> super;
    try {
      //ctags 5.5 doesn't give a full qualified name for parent class if the are defined
      //in different files...
      //f1.h namespace Foo { class Bar{}: }
      //f2.h namespace Foo { class X : public Bar {}; }
      //will result in class:X inherits: Bar
      //instead of class:X inherits:Foo::Bar
      //so we call recover to try different strategies to find the parent class nevertheless
      try {
        super = makeRootNode(getTag(*parent));
        
      } catch(UndefinedSymbolException&) {
        super = recoverSuperHierarchy(fqn, *parent);
      }

      if(recursive) {
        super->setParents(getSuperHierarchy(*parent, recursive));
      }
      
    
    } catch(UndefinedSymbolException &) {
      super = makeUnknown(*parent);
    }

    if(!super.isNull()) {
      parents.push_back(super);
    }
    
  }

  return parents;
}

struct InheritsIs : public std::binary_function<Tag, string, bool>
{
  result_type operator()(const first_argument_type &tag, const second_argument_type &name) const
  {
    return find(tag.getInheritance().begin(), tag.getInheritance().end(), name) != tag.getInheritance().end();
  }
};

std::list<smart_ptr<Inheritable> > HierarchyLoader::getSubHierarchy(const std::string &fqn, bool recursive)
{
  list<Tag> children = getDatabase().selectTagsWhere(make_query(InheritsIs(), fqn));
  
  //see HierarchyLoader::getSuperHierarchy() why we try again with an unqualified name !
  children.splice(children.end(), recoverSubHierarchy(fqn));
  children.sort(TagSorter());
  children.unique(TagComperator());

  std::list<smart_ptr<Inheritable> > extendors;
  
  for(list<Tag>::iterator child = children.begin(); child != children.end(); ++child) {
    smart_ptr<Inheritable> extendor = makeRootNode(*child);
    if(recursive) {
      extendor->setExtendors(getSubHierarchy(child->getFQN(), recursive));
    }
    extendors.push_back(extendor);
  }
  
  return extendors;
}


smart_ptr<Inheritable> HierarchyLoader::makeRootNode(const Tag &tag)
{
  smart_ptr<CppType> inheritable = TypeFactory().createTag(tag);

  smart_ptr<Inheritable> root = dynamic__cast<Inheritable>(inheritable);   
  if(root.isNull()) {
    log(GENERAL, CTAGS, eWarning) << "HierarchyLoader::makeRootNode(): node creation failed" << endl << tag << endlog;
    root = smart_ptr<Class>(new Class(tag.getFQN(), tag.getAccessQualifier(), TagLocation(tag.getFile(), tag.getLine())));
    root->setScope(inheritable->getScope());
  }

  root->setBroker(LoaderBroker::instance(mWorkspace));
  return root;
}

smart_ptr<Inheritable> HierarchyLoader::makeUnknown(const std::string &fqn)
{
  smart_ptr<Inheritable> unknown = make_smart_ptr(new Class(fqn, ePublic, TagLocation()));
  unknown->setBroker(LoaderBroker::instance(mWorkspace));
  return unknown;
}

Database& HierarchyLoader::getDatabase()
{
  return *DatabaseManager::instance().getDatabase(mWorkspace);
}

struct TypeNameIs : public binary_function<CTags::Tag, std::string, bool>
{
  result_type operator()(const first_argument_type &tag, const second_argument_type &name) const
  {
    switch(tag.getType()) {
      case eTypedef:
      case eClass :
      case eStruct :
      case eUnion : return tag.getFQN().name() == name;
    }

    return false;
  }
};

smart_ptr<Inheritable> HierarchyLoader::recoverSuperHierarchy(const std::string &child, const std::string &parent)
{
  Tracer trc(NO_LABEL, CTAGS, "HierarchyLoader::recoverSuperHierarchy(" + child + "," + parent + ")");
  
  if(child == parent) {
    throw logic_error("child has equal fqn as parent ?!?! " + child);
  }
//  FullQualifiedName fqnChild(child);
//  FullQualifiedName fqnParent(parent);
//  
//  log(NO_LABEL, CTAGS, eDebug) << " searching for parent " << fqnChild.scope() + "::" + fqnParent.toString() << endlog;
//  smart_ptr<Inheritable> p = makeRootNode(getTag(fqnChild.scope() + "::" + fqnParent.toString()));
//  
//  log(NO_LABEL, CTAGS, eDebug) << "found " << p->getFQN() << " as parent of " << child << endlog;
//  
//  return p;
  

	
  list<string> path = StringListTokenizer(FullQualifiedName(child).scope(), "::").tokens();

  while(!path.empty()) {
    try {        
      stringstream scope;
      copy(path.begin(), (++path.rbegin()).base(), ostream_iterator<string>(scope, "::"));
      scope << path.back();
      path.pop_back();

      //message_box(scope.str() + "::" + parent);
      log(NO_LABEL, CTAGS, eDebug) << " searching for parent " << scope.str() + "::" + parent << endlog;
      Tag tag = getTag(scope.str() + "::" + parent);
      if(tag.getFQN().toString() != child) {
        return makeRootNode(tag);
      }
      
    } catch(UndefinedSymbolException &) { }
  }

  return makeRootNode(getTag(parent, TypeNameIs()));   


}

struct InheritsIsX : public std::binary_function<Tag, string, bool>
{
  result_type operator()(const first_argument_type &tag, const second_argument_type &name) const
  {
    typedef list<string>::const_iterator LII;
    for(LII baseClass = tag.getInheritance().begin(); baseClass != tag.getInheritance().end(); ++baseClass) {
      if(hasPartialScopeMatch(*baseClass, name)) {
        return true;
      }
    }
    
    return false;
  }

  bool hasPartialScopeMatch(const std::string &baseClassFQN, const std::string &name) const
  {
    size_t pos = baseClassFQN.rfind(name);
    //baseClassFQN is equal to name
    if(pos == 0 && name.size() == baseClassFQN.size()) {
      return true;
    
    //no match
    } else if(pos == std::string::npos || baseClassFQN[pos-1] != ':') {
      return false;
    }
    
    return baseClassFQN.size() - name.size() == pos;
  }
};

struct FQNIs : public std::binary_function<std::string, FullQualifiedName, bool>
{
  bool operator()(const std::string & parent, const FullQualifiedName &fqn)const
  { return FullQualifiedName(parent) == fqn; }
};

std::list<CTags::Tag>  HierarchyLoader::recoverSubHierarchy(const std::string &parent)
{
  Tracer trc(NO_LABEL, CTAGS, "HierarchyLoader::recoverSubHierarchy(" + parent + ")");

  //get all tags with "name" only
  std::list<CTags::Tag> children = getDatabase().selectTagsWhere(make_query(InheritsIsX(), FullQualifiedName(parent).name()));
  
  //now start searching for qualified tags in descending order...
  for(list<Tag>::iterator child = children.begin(); child != children.end(); ++child) {
   list<string> parentPath = StringListTokenizer(parent, "::").tokens();
   list<string> childPath = StringListTokenizer(child->getFQN(), "::").tokens();
   
   //strip common path parent=Foo::Bar::Parent, child=Foo::Bar::XXX::Child
   while(!parentPath.empty() && !childPath.empty() && parentPath.front() == childPath.front()) {
     parentPath.pop_front();
     childPath.pop_front();
   }
   stringstream name;
   copy(parentPath.begin(), (++parentPath.rbegin()).base(), ostream_iterator<string>(name, "::"));
   if(!parentPath.empty()) { name << parentPath.back(); }
 
   if(find_if(child->getInheritance().begin(), child->getInheritance().end(), bind2nd(FQNIs(), FullQualifiedName(name.str()))) == child->getInheritance().end()){
     stringstream ss;
     copy(child->getInheritance().begin(), child->getInheritance().end(), ostream_iterator<string>(ss, ","));
     log(NO_LABEL, CTAGS, eDebug) << name.str() << " not found in parent list of " << child->getFQN() << "(" << ss.str() << ")" << endlog;
     child = children.erase(child);
   }
  }


  log(NO_LABEL, CTAGS, eDebug) << parent << " is probably extended by " << endl;
  for(list<Tag>::iterator child = children.begin(); child != children.end(); ++child) {
   _log_stream << child->getFQN() << endl;
  }
  _log_stream << endlog;
  
  return children;
}


Tag HierarchyLoader::resolveAmbiguity(const list<Tag> &tags)
{
  stringstream ss;
  ss << "tag-count " << tags.size() << "!=1 for tag " << tags.front().getFQN() << endl << endl << "found declarations at" << endl;
  for(list<Tag>::const_iterator i = tags.begin(); i != tags.end(); i++) {
    ss << i->getFQN() << " " << i->getFile() << ":"<< i->getLine() << endl;
  }

  if(mAmbiguityResolver.isNull()) {
    throw AmbiguousSymbolException(ss.str() + "\n\n(no ambiguity resolver set)");
  }
  
  message_box(ss.str());

  //create MetaObject list
  std::list<smart_ptr<MetaObject> > symbols;
  transform(tags.begin(), tags.end(), back_inserter(symbols), TypeFactory());

  //resolve
  smart_ptr<MetaObject> symbol = mAmbiguityResolver->resolve(symbols);
  if(symbol.isNull()) {
    throw AmbiguousSymbolException("resolve failed");
  }

  //find corresponding entry in ctags-list
  std::list<smart_ptr<MetaObject> >::iterator tmp = find(symbols.begin(), symbols.end(), symbol);
  if(tmp == symbols.end()) {
    throw AmbiguousSymbolException("resolve failed");
  }


  size_t dist = distance(symbols.begin(), tmp);
  list<Tag>::const_iterator tag = tags.begin();
  advance(tag, dist);
  
  //message_box(tag->getFile());

  return *tag;
}

}
