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

#ifndef CTagsHierarchyLoader_H
#define CTagsHierarchyLoader_H

#include "Model/HierarchyLoader.h"
#include <Model/CTags/CTagsDatabase.h>

class Workspace;
 
namespace CTags
{

class Database;
class Tag;

struct AmbiguousSymbolException : public std::runtime_error
{
  AmbiguousSymbolException(const std::string &what) 
    : runtime_error(what) {}
}; 

struct UndefinedSymbolException : public std::runtime_error
{
  UndefinedSymbolException(const std::string &what) 
    : runtime_error(what) {}
};

class HierarchyLoader : public ::HierarchyLoader
{
private:
  smart_ptr<Workspace> mWorkspace;
  smart_ptr<AmbiguityResolver> mAmbiguityResolver;
  
public:
  HierarchyLoader(const smart_ptr<Workspace> &ws) : mWorkspace(ws) {};
  
  virtual Inheritable& loadHierarchy(Inheritable&, bool recursive = false);
  virtual std::list<smart_ptr<Inheritable> > loadSubtypeHierarchy(const Inheritable&, bool recursive = false);
  virtual std::list<smart_ptr<Inheritable> > loadSupertypeHierarchy(const Inheritable&, bool recursive = false);

  virtual void setAmbiguityResolver(const smart_ptr<AmbiguityResolver> &resolver)
  { mAmbiguityResolver = resolver; }
  
private:
  std::list<smart_ptr<Inheritable> > getSuperHierarchy(const std::string &fqn, bool recursive);
  std::list<smart_ptr<Inheritable> > getSubHierarchy(const std::string &fqn, bool recursive);
  smart_ptr<Inheritable> makeRootNode(const Tag&);
  smart_ptr<Inheritable> makeUnknown(const std::string&);
  Database& getDatabase();

  smart_ptr<Inheritable> recoverSuperHierarchy(const std::string &child, const std::string &parent);
  std::list<CTags::Tag>  recoverSubHierarchy(const std::string &parent);
  
  Tag resolveAmbiguity(const list<Tag>& );

  //Tag getTag(const std::string &fqn);
  Tag getTag(const std::string &fqn) { return getTag(fqn, FullQualifiedNameIs()); }
  
  template<class BinaryFunctor>
  Tag getTag(const std::string &fqn, BinaryFunctor functor)
  {
    list<Tag> ctags = getDatabase().selectTagsWhere(make_query(functor, fqn));
    
    if(ctags.empty()) {
      throw UndefinedSymbolException(fqn);
      
    } else if (ctags.size() > 1) {
      return resolveAmbiguity(ctags);
    }
    
    return ctags.front();
  }
};


}
#endif