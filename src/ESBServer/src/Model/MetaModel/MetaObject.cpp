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

#include "MetaObject.h"

#include <Model/OutlineLoader.h>
#include <Utilities/stl_ext.h>

#include <string>
#include <locale>

MetaObject::MetaObjectImpl::MetaObjectImpl() 
: fqn(""), accessQualifier(ePublic), mHash(0) 
{
}

MetaObject::MetaObjectImpl::MetaObjectImpl(const std::string &fqn, AccessQualifier access, const TagLocation &decl) 
: fqn(fqn), accessQualifier(access), declaration(decl), mHash(0) 
{
}

MetaObject::MetaObjectImpl::~MetaObjectImpl()
{
}

MetaObject::MetaObject(const smart_ptr<MetaObject::DatastoreType> &ds)
: mDatastore(ds)
{
}

AccessQualifier MetaObject::getAccessQualifier() const
{
  return datastore().accessQualifier;
}
void MetaObject::setAccessQualifier(AccessQualifier q)
{
  datastore().accessQualifier = q;
}

const std::list<smart_ptr<MetaObject> >& MetaObject::getInnerTags() const 
{ 
  if(!getBroker().isNull() && datastore().innerTags.empty()) {
    datastore().innerTags = getBroker()->getOutlineLoader()->loadOutlineOf(*this);
  }

  return datastore().innerTags;
}

long MetaObject::MetaObjectImpl::generateHash(bool weak) const
{ 
	return xxx_hash(weak ? fqn.name() : fqn.toString()); 
}

MetaObject::MetaObject()
: mDatastore(new DatastoreType()){
}

MetaObject::~MetaObject()
{
};

CppType::CppType() 
{
}

CppType::CppType(const smart_ptr<DatastoreType> &ds)
: super(ds)
{
}


CppType::~CppType()
{
}
