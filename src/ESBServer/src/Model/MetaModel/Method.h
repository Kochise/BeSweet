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

#ifndef Method_H
#define Method_H

#include <Model/MetaModel/MetaObject.h>
#include <Model/MetaModel/MetaModelDefines.h>
#include <Model/MetaModel/MetaModelPolicies.h>
#include <Model/MetaModel/Function.h>
#include <Model/MetaModel/Field.h>

#include <string>

class Method : public Function, public ConstStaticPolicy, public ImplementationPolicy
{
public:
  typedef Function super;
  struct MethodImpl : public super::DatastoreType
  {
    typedef Function::DatastoreType super;
    ImplementationQualifier implementationQualifier;

    char constStatic;
    
    MethodImpl(const std::string& returnType, const std::string &name, const TagLocation&);
    MethodImpl(const std::string& returnType, const std::string &name, std::list<Field> &parameters);
  
  private:
    virtual long generateHash(bool weak = false) const;  
    
  };
  typedef MethodImpl DatastoreType;
  
public:
  Method(const std::string& returnType, const std::string &name, AccessQualifier aq, const TagLocation&);
  Method(const std::string& returnType, const std::string &name, std::list<Field> &parameters);
  virtual ~Method();
  
  void acceptVisitor(MetaObjectVisitor *v);

  virtual ImplementationQualifier getImplementationQualifier() const;
  virtual void setImplementationQualifier(ImplementationQualifier);
  
protected:
  DatastoreType& datastore()
  { return dynamic_cast<DatastoreType&>(super::datastore()); };
  
  const DatastoreType& datastore() const
  { return dynamic_cast<const DatastoreType&>(super::datastore()); };

  virtual char getConstStatic() const;
  virtual void setConstStatic(char);
};

#endif