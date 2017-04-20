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

#ifndef Function_H
#define Function_H

#include <Model/MetaModel/MetaObject.h>
#include <Model/MetaModel/MetaModelDefines.h>
#include <Model/MetaModel/MetaModelPolicies.h>
#include <Model/MetaModel/Field.h>

#include <string>

class Function : public MetaObject
{
public:
  typedef MetaObject super;
  struct FunctionImpl : public super::DatastoreType
  {
    typedef super::DatastoreType super;
    std::string returnType;
    std::list<Field> parameters;

    FunctionImpl(const std::string& returnType, const std::string &name, const TagLocation&);
    FunctionImpl(const std::string& returnType, const std::string &name, std::list<Field> &parameters);
    
    void setupReturnType(const std::string&);
  };
  typedef FunctionImpl DatastoreType;
  
public:
  Function(const std::string& returnType, const std::string &name, const TagLocation&);
  Function(const std::string& returnType, const std::string &name, std::list<Field> &parameters);
  virtual ~Function();
  
  
  const std::string& getReturnType() const  
  { return datastore().returnType; };
  
  const std::list<Field>& getParameters() const 
  { return datastore().parameters; };
  
  void acceptVisitor(MetaObjectVisitor *v);
  
protected:
  Function(const smart_ptr<DatastoreType>&);

  DatastoreType& datastore()
  { return dynamic_cast<DatastoreType&>(super::datastore()); };
  
  const DatastoreType& datastore() const
  { return dynamic_cast<const DatastoreType&>(super::datastore()); };
};


#endif