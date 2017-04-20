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

#ifndef Inheritable_H
#define Inheritable_H

#include <Model/MetaModel/MetaObject.h>
#include <Model/MetaModel/Field.h>
#include <Model/MetaModel/Method.h>

#include <Model/HierarchyLoader.h>

#include <map>
#include <algorithm>

using namespace std;


class Inheritable : public CppType
{
public:
  typedef CppType super;
  struct InheritableImpl : public super::DatastoreType
  {
    typedef super::DatastoreType super;
    mutable std::list<smart_ptr<Inheritable> > extendors;
    mutable std::list<smart_ptr<Inheritable> > parents;
    
    InheritableImpl(const std::string &name, AccessQualifier access, const TagLocation &decl = TagLocation());
  };
  typedef InheritableImpl DatastoreType;
      
public:
  Inheritable(const std::string &name, AccessQualifier access, const TagLocation &decl = TagLocation());
    
  std::list<smart_ptr<Field> > getFields() const
  { return selectInnerTags(eField, (Field*)NULL); }
  
  void addField(const smart_ptr<Field> &f) 
  { addInnerTag(f); }
  
  std::list<smart_ptr<Method> > getMethods() const
  { return selectInnerTags(eMethod, (Method*)NULL); }
  
  void addMethod(const smart_ptr<Method> &m) 
  { addInnerTag(m); }
  
  std::list<smart_ptr<CppType> > getTypes() const
  {  return selectInnerTags(eClass | eStruct | eUnion | eEnum | eTypedef, (CppType*)NULL); }

  void setExtendors(const std::list<smart_ptr<Inheritable> >& e)
  { datastore().extendors = e; datastore().extendors.remove_if(is_null());}
  
  const std::list<smart_ptr<Inheritable> >& getExtendors() const;
  
  void setParents(const std::list<smart_ptr<Inheritable> >& p)
  { datastore().parents = p; datastore().extendors.remove_if(is_null());}
  
  const std::list<smart_ptr<Inheritable> >& getParents() const;
  
  std::map<long, std::list<smart_ptr<MetaObject> > > getMembers(bool inherited = false) const;

protected:
  Inheritable(const smart_ptr<DatastoreType> &);

  DatastoreType& datastore()
  { return dynamic_cast<DatastoreType&>(super::datastore()); };
  
  const DatastoreType& datastore() const
  { return dynamic_cast<const DatastoreType&>(super::datastore()); };
  
  template<class T>
  std::list<smart_ptr<T> > selectInnerTags(size_t type, T*dummy = NULL) const
  {
    MetaObjectTyper tagTyper;
    std::list<smart_ptr<T> > result;
    typedef std::list<smart_ptr<MetaObject> > TagList;
    for(TagList::const_iterator tag = getInnerTags().begin(); tag != getInnerTags().end(); tag++) {
      (*tag)->acceptVisitor(&tagTyper);
      if(tagTyper.getType() & type) {
        result.push_back(dynamic__cast<T>(*tag));
      }
    }
    
    return result;
  }
};

#endif