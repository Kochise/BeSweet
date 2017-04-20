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

#include "Variable.h"

#include <Model/MetaModel/MetaObjectVisitors.h>
#include <Utilities/stl_ext.h>

using namespace std;

Variable::VariableImpl::VariableImpl(const std::string &t, const std::string &name,  const TagLocation& location)
: super(name, ePublic, location), type(t), constStatic(0)
{
  size_t pos;
  if((pos = type.find("static ")) != string::npos) {
    type.erase(pos, 7);
    //isStatic(true);
  }
  if((pos = type.find("const ")) != string::npos) {
    type.erase(pos, 6);
    //isConst(true);
  }
  rm_dup_space(type);
}

Variable::Variable(const string &type, const string &name, const TagLocation &location)
: super(smart_ptr<DatastoreType>(new DatastoreType(type, name, location)))
{
}

Variable::Variable(const smart_ptr<DatastoreType> &ds)
: super(ds)
{
}

Variable::~Variable()
{
}

void Variable::acceptVisitor(MetaObjectVisitor *v) 
{ 
  v->visitVariable(this);
}

char Variable::getConstStatic() const
{
  DatastoreType debug = datastore();
  return datastore().constStatic;
}

void Variable::setConstStatic(char cs)
{
  datastore().constStatic = cs;
}
