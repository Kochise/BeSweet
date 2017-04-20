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

#include "CTagsModelBuilder.h"

#include <Model/LoaderBroker.h>
#include <Model/CTags/CTagsMetaObjectFactory.h>


using namespace std;

namespace CTags {

ModelBuilder::ModelBuilder(const smart_ptr<Workspace> &wspace)
: mWorkspace(wspace), mBroker(LoaderBroker::instance(wspace))
{

}

struct ParentIsGlobalNamespace : public unary_function<smart_ptr<MetaObject>, bool>
{
  bool operator()(const smart_ptr<MetaObject> &object) const
  { return object->getScope()->getFQN().toString() == ""; }
};

ModelBuilder::TagList ModelBuilder::createModel(Database::TagTable& table)
{
  return createModel(table, 0);
};

template<class T1, class T2 = T1>
struct IsInnerTagOf : public binary_function<smart_ptr<T1>,smart_ptr<T2>, bool>
{
  result_type operator()(const first_argument_type &inner, const second_argument_type &outer) const
  { return inner->getFQN().scope() == outer->getFQN().toString(); }
};

ModelBuilder::TagList ModelBuilder::createModel(Database::TagTable& table, size_t level)
{
  TagList result;

  Database::TagTable::iterator ctag = table.begin();
  
  while(ctag != table.end())
  {
    /*just for debuggin*/
    string fqn = ctag->getFQN().toString();
    size_t cnt = count_if(fqn.begin(), fqn.end(), bind2nd(equal_to<char>(), ':'));
    
    if(cnt/2 == level) {
      smart_ptr<MetaObject> object = createObject(ctag, table);
      if(!object.isNull()) {
        if(find_if(result.begin(), result.end(), bind2nd(hash_comparator<false>(), object)) == result.end()) {
          result.push_back(object);
        }
      }
    
      ctag = table.erase(ctag);

    } else {
      ++ctag;
    }
  }

  if(!table.empty()) {
    TagList tmp = createModel(table, level+1);
    tmp.sort(FQNSorter<MetaObject>());
    
    for(TagList::const_iterator outer = result.begin(); outer != result.end(); ++outer) {      
      TagList::iterator lowerbound = find_if(tmp.begin(), tmp.end(), bind2nd(IsInnerTagOf<MetaObject>(), *outer));
      TagList::iterator upperbound = find_if(lowerbound, tmp.end(), not1(bind2nd(IsInnerTagOf<MetaObject>(), *outer)));
      
      (*outer)->addInnerTags(lowerbound, upperbound);

       tmp.erase(lowerbound, upperbound);
    }

    if(!tmp.empty()) {
      result.splice(result.end(), tmp);
    }
  }
  
  return result;
}

smart_ptr<MetaObject> ModelBuilder::createObject(Database::TagTable::iterator &ctag, Database::TagTable& table)
{
  smart_ptr<MetaObject> object;
  
  switch(ctag->getType()) {
    case eNamespace :
      {       
        if(ctag->getFQN().toString() != "" && ctag->getFQN().toString() != GLOBAL_NAMESPACE) {        
          object = TypeFactory().createTag(*ctag);
        }
      
        break;
      }
    
    case eClass :
    case eStruct:
    case eUnion:
    case eEnum:
    case eTypedef:
      {          
        object = TypeFactory().createTag(*ctag);
        object->setBroker(broker());
        break;
      }
    case eEnumMember:
      {
        try {
          //Dynamic_cast<Enum>(parent)->addValue(ctag->getName());
              } catch(...){}
            
              break;
      }
    case eMethod:
      {
        Database::TagTable::iterator buddy;
        object = make_smart_ptr(MethodFactory(&table).create(*ctag, buddy));
      
        if(buddy != table.end() && buddy != table.begin()) {
          table.erase(buddy);
        }
        break;
      }
    case eField:
      {
        object = FieldFactory().createTag(*ctag);
        break;
      }
    case eFunction:
      {
        Database::TagTable::iterator buddy;
        object = FunctionFactory(&table).create(*ctag, buddy);
      
        if(buddy != table.end() && buddy != table.begin()) {
          table.erase(buddy);
        }
        break;
      }
    case eVariable:
      {         
        object = VariableFactory().createTag(*ctag);          
        break;
      }
  }

  return object;
}

} //namespace






















