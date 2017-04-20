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

#ifndef Enum_H
#define Enum_H

#include <Model/MetaModel/MetaObject.h>
#include <Model/MetaModel/MetaModelDefines.h>
#include <Model/MetaModel/MetaModelPolicies.h>

class Enum : public CppType
{
public:
  typedef CppType super;
  struct EnumImpl : public super::DatastoreType
  {
    typedef super::DatastoreType super;
    std::list<std::string> values;
    EnumImpl(const std::string &name, AccessQualifier access, const TagLocation &decl);
  };
  typedef EnumImpl DatastoreType;
  
public:
  Enum(const std::string &name, AccessQualifier access, const TagLocation &decl);
  virtual ~Enum();
  
  void setValues(const std::list<std::string>& values)
  { datastore().values = values; }
  
  const std::list<std::string>& getValues() const
  { return datastore().values; }

  void addValue(const std::string &value)
  { datastore().values.push_back(value); }

  void acceptVisitor(MetaObjectVisitor *v);

protected:
  DatastoreType& datastore()
  { return dynamic_cast<DatastoreType&>(super::datastore()); };
  
  const DatastoreType& datastore() const
  { return dynamic_cast<const DatastoreType&>(super::datastore()); };
};

#endif