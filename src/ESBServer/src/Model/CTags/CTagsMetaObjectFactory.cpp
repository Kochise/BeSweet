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

#include "CTagsMetaObjectFactory.h"
#include <Model/MetaModel/MetaModel.h>

#include <string>
#include <sstream>

using namespace std;
using CTags::Database::TagTable;

namespace CTags
{
  struct create_field
  {
    Field operator()(const string& type) const
    { return Field(type, "name"); }
  };
  
smart_ptr<TypeCreator::result_type> TypeCreator::create(const Tag& tag, const smart_ptr<LoaderBroker> &broker)
{
  smart_ptr<TypeCreator::result_type> result = create(tag.getType(), tag.getFQN(), tag.getAccessQualifier(), TagLocation(tag.getFile(), tag.getLine()));
  result->setScope(create(tag.getScopeType(), tag.getFQN().scope()));
  result->setBroker(broker);
  return result;
} 

smart_ptr<TypeCreator::result_type> TypeCreator::create(MetaObjectType type, const FullQualifiedName &name, AccessQualifier access, const TagLocation &location)
{
  smart_ptr<CppType> result;
  
  switch(type) {
		case eNamespace: result = smart_ptr<CppType>(new Namespace(name)); break;
    case eClass :		 result = smart_ptr<CppType>(new Class(name, access, location));  break;
    case eStruct :	 result = smart_ptr<CppType>(new Struct(name, access, location));  break;
    case eUnion :	   result = smart_ptr<CppType>(new Union(name, access, location));  break;
    case eEnum :		 result = smart_ptr<CppType>(new Enum(name, access, location));  break;
    case eTypedef :	 result = smart_ptr<CppType>(new Typedef(name, access, location));  break;
      
    default: 
      {
        std::stringstream text;
        text << "TypeCreator::create(): 0x" << std::hex << type << " not supported!";
        throw std::runtime_error(text.str());
      }
  }
  return result;
}
  
smart_ptr<MethodInfoCreator::result_type> MethodInfoCreator::create(const Tag &tag, const smart_ptr<LoaderBroker>&)
{
  smart_ptr<MethodInfoCreator::result_type> result;
  
  string name = tag.getFQN();
  AccessQualifier access = tag.getAccessQualifier();
  TagLocation file(tag.getFile(), tag.getLine());

  list<Field> signature;
  transform(tag.getSignature().begin(), tag.getSignature().end(), back_inserter(signature), create_field());
  
  switch(tag.getType()) {
    case eMethod :	 result = make_smart_ptr(new Method(tag.getReturnType(), name, signature)); break;
    case eFunction : result = make_smart_ptr(new Function(tag.getReturnType(), name, signature)); break;
    default :
    {
	    std::stringstream text;
      text << "MethodFactoryPolicy::create(): 0x" << std::hex << tag.getType() << " not supported!";
      throw std::runtime_error(text.str());
    }
  }
  
  tag.isDeclaration() ? result->setDeclaration(file) : result->setDefinition(file);
  result->setAccessQualifier(access); 
  result->setScope(TypeCreator().create(tag.getScopeType(), tag.getFQN().scope()));
  return result;
} 

smart_ptr<FieldCreator::result_type> FieldCreator::create(const Tag& tag, const smart_ptr<LoaderBroker>&)
{
  smart_ptr<FieldCreator::result_type> result;
  
  string name = tag.getFQN();
  string returnType = tag.getReturnType();
  AccessQualifier access = tag.getAccessQualifier();
  TagLocation file(tag.getFile(), tag.getLine());
  
  switch(tag.getType()) {
    case eField :	
      {
        result = make_smart_ptr(new Field(returnType, name, access, file)); 
        Field *field = dynamic_cast<Field*>(result.get());
        if(returnType.find("static ") != string::npos) {
          field->isStatic(true);
        }
        if(returnType.find("const ") != string::npos) {
          field->isConst(true);
        }
        result = field;
      }
      break;

    case eVariable : result = make_smart_ptr(new Variable(returnType, name, file)); break;
    default :
      {
        std::stringstream text;
        text << "FieldCreator::create(): 0x" << std::hex << tag.getType() << " not supported!";
        throw std::runtime_error(text.str());
      }
  }
  
  result->setScope(TypeCreator().create(tag.getScopeType(), tag.getFQN().scope()));
  return result;
} 


CreatorPolicy<Method>::result_type MethodCreatorPolicy::create(Tag const* declaration, Tag const *definition) const
{
  Tag const *tag = declaration ? declaration : definition;

  list<Field> signature;
  transform(tag->getSignature().begin(), tag->getSignature().end(), back_inserter(signature), create_field());
  Method method(tag->getReturnType(), tag->getFQN(), signature);

  setDeclAndDefn(method, declaration, definition);

  method.setAccessQualifier(tag->getAccessQualifier());

  if(tag->getReturnType().find("static ") != string::npos) {
    method.isStatic(true);
  }

  size_t pos = tag->getCode().find(")");
  if(pos != string::npos && tag->getCode().substr(pos, -1).find("const") != string::npos) {
    method.isConst(true);
  }

  method.setImplementationQualifier(tag->getImplementationQualifier());
  return method;
}

smart_ptr<Function> FunctionCreatorPolicy::create(Tag const* declaration, Tag const *definition) const
{
  Tag const *tag = declaration ? declaration : definition;

  smart_ptr<Function> function(new Function(tag->getReturnType(), tag->getFQN(), list<Field>()));
	setDeclAndDefn(*function, declaration, definition);
  return function;
}


} //namespace CTags