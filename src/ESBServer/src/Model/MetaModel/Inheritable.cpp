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

#include "Inheritable.h"

#include <Model/MetaModel/MetaObjectVisitors.h>
#include <Model/MetaModel/MetaModelUtilities.h>
#include <Utilities/smart_ptr.h>
#include <Utilities/stl_ext.h>

#include <algorithm>
#include <functional>
#include <list>
#include <map>

using namespace std;

Inheritable::InheritableImpl::InheritableImpl(const std::string &name, AccessQualifier access, const TagLocation &decl)
: super(name, access, decl)
{
}

Inheritable::Inheritable(const std::string &name, AccessQualifier access, const TagLocation &decl)
: super(smart_ptr<DatastoreType>(new DatastoreType(name, access, decl)))
{
}

Inheritable::Inheritable(const smart_ptr<DatastoreType> &ds)
: super(ds)
{
}

const std::list<smart_ptr<Inheritable> >& Inheritable::getExtendors() const
{ 
  if(!getBroker().isNull() && datastore().extendors.empty()) {
    datastore().extendors = getBroker()->getHierarchyLoader()->loadSubtypeHierarchy(*this);
    datastore().extendors.remove_if(is_null());
  }
  return datastore().extendors; 
}


const std::list<smart_ptr<Inheritable> >& Inheritable::getParents() const
{ 
  if(!getBroker().isNull() && datastore().parents.empty()) {
    datastore().parents = getBroker()->getHierarchyLoader()->loadSupertypeHierarchy(*this);
    datastore().parents.remove_if(is_null());
  }
  
  return datastore().parents; 
}


struct map_value_generator
{
  pair<long, std::list<smart_ptr<MetaObject> > > operator()(const smart_ptr<MetaObject> &value) const
  {
    typedef container_creator<list<smart_ptr<MetaObject> > > ListCreator;
    return make_pair(value->hash(true/*weak*/), ListCreator()(value));
  }
};


map<long, list<smart_ptr<MetaObject> > > Inheritable::getMembers(bool inherited) const
{
  typedef list<smart_ptr<Inheritable> > IList;
  typedef list<smart_ptr<MetaObject> > TList;
  typedef map<long, TList> MList;
  
  map<long, TList> members;
  transform(getInnerTags().begin(), getInnerTags().end(), inserter(members, members.begin()), map_value_generator());
  
  if(inherited) {
    for(IList::const_iterator parent = getParents().begin(); parent != getParents().end(); ++parent) {
      MList parentMembers = (*parent)->getMembers(inherited);
      
      for(MList::iterator member = parentMembers.begin(); member != parentMembers.end(); ++member) {
        TList memberList = member->second;
        memberList.remove_if(bind2nd(AccessQualifierIs(),ePrivate));
        if(!memberList.empty()) {
          members[member->first].splice(members[member->first].end(), memberList);
        }
      }
    }
  }
  
  return members;
}