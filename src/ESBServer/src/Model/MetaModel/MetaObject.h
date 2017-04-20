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

#ifndef CppMetaObject_H
#define CppMetaObject_H

#include <Model/MetaModel/MetaModelDefines.h>
#include <Model/MetaModel/MetaModelPolicies.h>

#include <Model/TagLocation.h>
#include <Model/FullQualifiedName.h>
#include <Model/LoaderBroker.h>

#include <Utilities/smart_ptr.h>

#include <string>
#include <list>

class MetaObjectVisitor;
class LoaderBroker;


class MetaObject : public AccessPolicy
{
public:
  class MetaObjectImpl
  {
  public:
    TagLocation declaration;
    TagLocation definition;
    
    FullQualifiedName fqn;
    smart_ptr<MetaObject> scope;
    
    mutable std::list<smart_ptr<MetaObject> > innerTags;
    AccessQualifier accessQualifier;
    
    mutable long mHash;

   public:    
    MetaObjectImpl();
    virtual ~MetaObjectImpl();
    MetaObjectImpl(const std::string &name, AccessQualifier access = ePublic, const TagLocation& = TagLocation()); 

    long hash(bool weak = false) const
    { if(mHash == 0) mHash = generateHash(weak); return mHash; }

   protected:
    virtual long generateHash(bool weak) const;
  };
  typedef MetaObjectImpl DatastoreType;
  
private:
  smart_ptr<MetaObjectImpl> mDatastore;
  smart_ptr<LoaderBroker> mBroker;

public:
  MetaObject();

  virtual ~MetaObject();
  
  virtual void acceptVisitor(MetaObjectVisitor *) = 0;
  
  void setBroker(const smart_ptr<LoaderBroker> &b) { mBroker = b;}

  const TagLocation& getDeclaration() const { return datastore().declaration; }
  void setDeclaration(const TagLocation& location) { datastore().declaration = location; }
  
  const TagLocation& getDefinition() const { return datastore().definition; }
  void setDefinition(const TagLocation &location) { datastore().definition = location; }
  
  //const std::string& getName() const { return datastore().fqn.name(); };
  //const std::string& getScopeName() const { return datastore().fqn.scope(); }
  
  smart_ptr<MetaObject> getScope() const { return datastore().scope; }
  void setScope(const smart_ptr<MetaObject>& scope) { datastore().scope = scope; }
  
  const FullQualifiedName& getFQN() const { return datastore().fqn; } 
  
  const std::list<smart_ptr<MetaObject> >& getInnerTags() const;
  void setInnerTags(const std::list<smart_ptr<MetaObject> >& tags) { datastore().innerTags = tags;}
  
  template<class II>
  void addInnerTags(const II& begin, const II end) 
  { datastore().innerTags.insert(datastore().innerTags.end(), begin, end);}

  void addInnerTag(const smart_ptr<MetaObject> &tag) { datastore().innerTags.push_back(tag); }
    
  virtual long hash(bool weak = false) const { return datastore().hash(weak); }
  
  virtual bool equals(const MetaObject& other) const
  { return this->datastore().hash() == other.datastore().hash(); }
  
  virtual AccessQualifier getAccessQualifier() const;
  virtual void setAccessQualifier(AccessQualifier);

protected:
  MetaObject(const smart_ptr<DatastoreType>&);
  
  DatastoreType& datastore(){ return *mDatastore; }
  const DatastoreType& datastore() const { return *mDatastore; }
  smart_ptr<LoaderBroker>& getBroker() { return mBroker; }
  const smart_ptr<LoaderBroker>& getBroker() const { return mBroker; }
};

class CppType : public MetaObject
{
public:
  typedef MetaObject super;
  typedef super::DatastoreType DatastoreType;

public:
  virtual ~CppType();
  
protected:
  CppType();
  CppType(const smart_ptr<DatastoreType>&);
  
  
};

#endif
