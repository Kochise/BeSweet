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

#ifndef TagFactory_H
#define TagFactory_H

#include <Model/ctags/CTagsDatabase.h>
#include <Model/ctags/CTagsTag.h>
#include <Utilities/smart_ptr.h>

#include <Model/MetaModel/MetaModel.h>

#include <functional>
#include <string>


namespace CTags {


struct TypeCreator
{
  typedef CppType result_type;
  smart_ptr<result_type> create(const CTags::Tag&, const smart_ptr<LoaderBroker>&);
  smart_ptr<result_type> create(MetaObjectType type, const FullQualifiedName &fqn, AccessQualifier access = ePublic, const TagLocation &location = TagLocation());
};

struct MethodInfoCreator
{
  typedef Function result_type;
  smart_ptr<result_type> create(const CTags::Tag&, const smart_ptr<LoaderBroker>&);
};

struct FieldCreator
{
  typedef Variable result_type;
  smart_ptr<result_type> create(const CTags::Tag&, const smart_ptr<LoaderBroker>&);
};

template<class CreationPolicy>
class TagFactory : public std::unary_function<CTags::Tag, smart_ptr<typename CreationPolicy::result_type> >, protected CreationPolicy
{
private:
  smart_ptr<LoaderBroker> mBroker;

public:
  typedef std::unary_function<CTags::Tag, smart_ptr<typename CreationPolicy::result_type> > Base;
  typedef typename Base::result_type result_type;
  
  result_type createTag(const CTags::Tag &ctag)
  { return create(ctag, mBroker); }
  
  result_type operator()(const CTags::Tag &tag)
  { return createTag(tag); }

  void setBroker(const smart_ptr<LoaderBroker> &broker)
  { mBroker = broker; }
    
};


template<class T>
class CreatorPolicy
{
public:
  typedef T result_type;

protected:
  CreatorPolicy(){}
  virtual ~CreatorPolicy(){};
    
  void setDeclAndDefn(T &t, CTags::Tag const *decl, CTags::Tag const *defn) const
  {
    setDeclaration(t, decl);
    setDefinition(t, defn);
  }
  
  void setDeclaration(T &function, CTags::Tag const* declaration) const
  {
    if(declaration && declaration->isDeclaration()) {
		  function.setDeclaration(TagLocation(declaration->getFile(), declaration->getLine()));
	  }
  }

  void setDefinition(T &function, CTags::Tag const* definition) const
  {
    if(definition && !definition->isDeclaration()) {
		  function.setDefinition(TagLocation(definition->getFile(), definition->getLine()));
	  }
  }

};

class MethodCreatorPolicy : protected CreatorPolicy<Method>
{
public:  
  Method create(CTags::Tag const *, CTags::Tag const*) const;
};

class FunctionCreatorPolicy : protected CreatorPolicy<Function>
{
public:
  typedef smart_ptr<Function> result_type;
  smart_ptr<Function> create(CTags::Tag const *, CTags::Tag const*) const;
};

template<class CreationPolicy>
class MethodFunctionFactory : protected CreationPolicy
{
private:
  CTags::Database::TagTable *mTagTable;


public:
  MethodFunctionFactory(CTags::Database::TagTable *table)
  : CreationPolicy(), mTagTable(table) {};

  typename CreationPolicy::result_type create(const CTags::Tag &tag, CTags::Database::TagTable::iterator &buddy) const
  {
    Tag const *buddyTag = 0;
  
    try {
      buddy = findBuddyTagOf(tag);
      buddyTag = &(*buddy);
    
    } catch(...) {
      buddy = mTagTable->end();
    }

    return CreationPolicy::create(getDeclaration(&tag, buddyTag), getDefinition(&tag, buddyTag));
  }

  typename CreationPolicy::result_type operator()(const CTags::Tag &tag, CTags::Database::TagTable::iterator &buddy) const
  { return create(tag, buddy); }
  
protected:
  
private:
  CTags::Database::TagTable::iterator findBuddyTagOf(const CTags::Tag &tag) const
  {
    for(CTags::Database::TagTable::iterator buddy = mTagTable->begin(); buddy != mTagTable->end(); buddy++) {
		  if(buddy->isBuddyOf(tag)) {
			  return buddy;
		  }
	  }

	  throw invalid_argument("no buddy tag found for " + tag.getFQN().toString());
  }
  
  CTags::Tag const* getDeclaration(CTags::Tag const *tag1, CTags::Tag const *tag2) const
  {
    return tag1 && tag1->isDeclaration() ? tag1 : tag2;
  }

  CTags::Tag const* getDefinition(CTags::Tag const *tag1, CTags::Tag const *tag2) const
  {
    return tag1 && !tag1->isDeclaration() ? tag1 : tag2;
  }
};

typedef TagFactory<TypeCreator> TypeFactory;
typedef TagFactory<FieldCreator> FieldFactory;
typedef TagFactory<FieldCreator> VariableFactory;

typedef TagFactory<MethodInfoCreator> FunctionInfoFactory;
typedef MethodFunctionFactory<FunctionCreatorPolicy> FunctionFactory;

typedef TagFactory<MethodInfoCreator> MethodInfoFactory;
typedef MethodFunctionFactory<MethodCreatorPolicy> MethodFactory;

}

#endif
