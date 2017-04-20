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

#ifndef MetaModelUtilities_H
#define MetaModelUtilities_H

#include <Model/MetaModel/MetaModel.h>
#include <Utilities/smart_ptr.h>
#include <Utilities/stl_ext.h>

#include <list>
#include <map>
#include <functional>

class MetaObjectTypeTranslator
{
private:
  MetaObjectType mType;

public:
  MetaObjectTypeTranslator(const std::string &type) 
    : mType(asType(type)){ }
  
  MetaObjectTypeTranslator(MetaObjectType type) 
    : mType(type){ }

  std::string asString() const
  { return asString(mType); }

  MetaObjectType asType() const
  { return mType; }

  static std::string asString(MetaObjectType type)
  {
    switch(type) {
      case eNamespace: return "namespace";
      case eClass: return "class";
      case eStruct: return "struct";
      case eUnion: return "union";
      case eEnum: return "enum";
      case eTypedef: return "typedef";
      case eEnumMember: return "enum_member";
      case eField: return "field";
      case eVariable: return "variable";
      case eMethod: return "method";
      case eFunction: return "function";
    }

    throw std::runtime_error("unknown MetaObjectType " + lexical_cast<string>(type));
  }

  static MetaObjectType asType(const std::string &type)
  {
    if(type == "namespace") {
      return eNamespace;
    } else if(type == "class") {
      return eClass;
    } else if(type == "struct") {
      return eStruct;
    } else if(type == "union") {
      return eUnion;
    } else if(type == "enum") {
       return eEnum;
    } else if(type == "enum_member") {
      return eEnumMember;
    } else if(type == "typedef") {
      return eTypedef;
    } else if(type == "field") {
      return eField;
    } else if(type == "variable") {
      return eVariable;
    } else if(type == "method") {
      return eMethod;
    } else if(type == "function") {
      return eFunction;
    }

    throw std::runtime_error("unknown MetaObjectType '" + type + "'");
  }
};


class RootFinder
{
public:
  typedef std::map<size_t, std::list<smart_ptr<Inheritable> > > RootMap;

public:
  RootMap findRootOf(const smart_ptr<Inheritable>&);

};

struct IsRelativeOf : public std::binary_function<smart_ptr<Inheritable>, smart_ptr<Inheritable>, bool>
{  
  bool operator()(const smart_ptr<Inheritable> &r1, const smart_ptr<Inheritable> &r2) const;
  
  static bool isParentOf(const smart_ptr<Inheritable> &parent, const smart_ptr<Inheritable> &child);
  static bool isExtendorOf(const smart_ptr<Inheritable> &child, const smart_ptr<Inheritable> &parent);
};

template<bool weak, class T1 = smart_ptr<MetaObject>, class T2 = T1>
struct hash_comparator : public std::binary_function<T1, T2, bool>
{
  bool operator()(const first_argument_type &t1, const second_argument_type& t2) const
  { return t1->hash(weak) == t2->hash(weak); }
};

template<class T1, class T2 = T1>
struct AccessSorter : public std::binary_function<smart_ptr<T1>,smart_ptr<T2>,bool>
{
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getAccessQualifier() < rhs->getAccessQualifier(); }
};

struct AccessQualifierIs : public std::binary_function<smart_ptr<MetaObject>, size_t, bool>
{
  result_type operator()(const first_argument_type &tag, const second_argument_type &x) const
  { return (tag->getAccessQualifier() & x) != 0; }
};

template<class T1, class T2 = T1>
struct NameSorter : public std::binary_function<smart_ptr<T1>,smart_ptr<T2>,bool>
{
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getFQN().name() < rhs->getFQN().name(); }
};

template<class T1, class T2 = T1>
struct NameComparator : public std::binary_function<smart_ptr<T1>,smart_ptr<T2>,bool>
{
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getFQN().name() == rhs->getFQN().name(); }
};

template<class T1, class T2 = T1>
struct ScopeSorter : std::binary_function<smart_ptr<T1>, smart_ptr<T2>, bool> {
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getFQN().scope() < rhs->getFQN().scope(); }
};

template<class T1, class T2 = T1>
struct ScopeComparator : std::binary_function<smart_ptr<T1>, smart_ptr<T2>, bool> {
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getScopeName() < rhs->getScopeName(); }
};

template<class T1, class T2 = T1>
struct FQNSorter : public std::binary_function<smart_ptr<T1>,smart_ptr<T2>,bool>
{
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getFQN().toString() < rhs->getFQN().toString(); }
};

template<class T1, class T2 = T1>
struct FQNComparator : public std::binary_function<smart_ptr<T1>,smart_ptr<T2>,bool>
{
  bool operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
  { return lhs->getFQN().toString() == rhs->getFQN().toString(); }
};

#endif