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

#include <stdafx.h>

#include "Method.h"

#include <Model/MetaModel/MetaObjectVisitors.h>
#include <Utilities/stl_ext.h>

#include <iterator>
#include <algorithm>
#include <functional>

using namespace std;

Method::MethodImpl::MethodImpl(const std::string& returnType, const std::string &name, const TagLocation &decl)
: super(returnType, name, decl), constStatic(0)
{
}

Method::MethodImpl::MethodImpl(const std::string& returnType, const std::string &name, std::list<Field> &parameters)
: super(returnType, name, parameters), constStatic(0)
{
}

long Method::MethodImpl::generateHash(bool weak) const
{ 
  stringstream ss;
//  if(isStatic()) {
//    ss << "static";
//  }
  
  ss << (weak ? fqn.name() : fqn) << "(";
  transform(parameters.begin(), parameters.end(), ostream_iterator<string>(ss, ","), mem_fun_ref(&Field::getType));
  ss << ")";
  
//  if(isConst()) {
//    ss << "const";
//  }
  return xxx_hash(ss.str());
}

Method::Method(const string &returnType, const string &name, AccessQualifier aq, const TagLocation &info)
: super(smart_ptr<DatastoreType>(new DatastoreType(returnType, name, info)))
{ 
  setAccessQualifier(aq); 
}

Method::Method(const string& returnType, const string &name, list<Field> &parameters) 
: super(smart_ptr<DatastoreType>(new DatastoreType(returnType, name, parameters)))
{
}

Method::~Method()
{
}

ImplementationQualifier Method::getImplementationQualifier() const
{
  return datastore().implementationQualifier;  
}

void Method::setImplementationQualifier(ImplementationQualifier q)
{
  datastore().implementationQualifier = q;
}

char Method::getConstStatic() const
{
  return datastore().constStatic;
}

void Method::setConstStatic(char cs)
{
  datastore().constStatic = cs;
}

void Method::acceptVisitor(MetaObjectVisitor *v) 
{ 
  v->visitMethod(this); 
}

