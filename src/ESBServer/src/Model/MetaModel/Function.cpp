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

#include "Function.h"

#include <Model/MetaModel/MetaObjectVisitors.h>
#include <Utilities/stl_ext.h>

using namespace std;

Function::FunctionImpl::FunctionImpl(const std::string& rt, const std::string &name, const TagLocation &decl)
: super(name, ePublic, decl)
{
  setupReturnType(rt);
}

Function::FunctionImpl::FunctionImpl(const std::string& rt, const std::string &name, std::list<Field> &p)
: super(name)
{
  setupReturnType(rt); 
  parameters = p;
}

void Function::FunctionImpl::setupReturnType(const std::string &rt)
{
  returnType = rt;

  size_t pos;
  if((pos = returnType.find("virtual ")) != string::npos) {
    returnType.erase(pos, 7);
  }
  if((pos = returnType.find("static ")) != string::npos) {
    returnType.erase(pos, 6);
  }

  rm_dup_space(returnType);
}

Function::Function(const string& returnType, const string &name, const TagLocation &decl) 
: super(smart_ptr<DatastoreType>(new DatastoreType(returnType, name, decl)))
{ 
}

Function::Function(const string& returnType, const string &name, list<Field> &parameters) 
: super(smart_ptr<DatastoreType>(new DatastoreType(returnType, name, parameters)))
{
}

Function::Function(const smart_ptr<DatastoreType> &ds)
: super(ds)
{
}

Function::~Function()
{
}

void Function::acceptVisitor(MetaObjectVisitor *v) 
{ 
  v->visitFunction(this); 
}