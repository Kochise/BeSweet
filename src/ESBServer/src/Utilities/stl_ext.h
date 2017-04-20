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

#ifndef STL_EXT_H
#define STL_EXT_H

#include <cctype>
#include <cstring>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <typeinfo>
#include <list>
#include <locale>

#include <Utilities/smart_ptr.h>

template<class LHS, class RHS>
LHS lexical_cast(const RHS &rhs)
{
  LHS lhs;
  std::stringstream ss;
  ss << rhs;
  ss >> lhs;

  if(ss.fail()) {
    throw std::invalid_argument(std::string("lexical_cast(): fail to convert ") + typeid(RHS).name() + ss.str() + " to " + typeid(LHS).name());
  }

  return lhs;
}

template<typename ForwardIter, typename OutputIter, typename UnaryPred>
OutputIter copy_if(ForwardIter begin, ForwardIter end, OutputIter dest, UnaryPred f) 
{
  while(begin != end) {
    if(f(*begin))
      *dest++ = *begin;
    begin++;
  }
  return dest;
}

template <typename T>
void rm_dup_space (T &s) {
    typename T::size_type pos = 0;
    typename T::size_type size = s.size();

    while ((pos+1) < size) {
       if (isspace(s[pos]) && isspace(s[pos + 1])) {
           s.erase(pos+1, 1);
           size = s.size();
           continue;
       }

       pos++;
    }
}
/*
template <typename T>
void rm_space_newline(T &s) {
  typename T::size_type pos;
  
  while ( (pos = s.find('\n')) != T::npos) {
    s[pos] = ' ';
    rm_space_forward(s, pos+1);
    if (pos != 0) rm_space_reverse(s, pos-1);
    
    if ((pos + 1) == s.size()) {
      s.erase(pos); break;
    }
    
  }
}*/

template<typename CharType>
struct make_lower_case
{
  CharType operator() (CharType c) const
  { using namespace std; return tolower(c); }
};

inline char is_cntrl(char c)
{
  using namespace std;
  return iscntrl(c);
}

template <typename T>
T to_lower(const T& in)
{
  std::string out = in;
  std::transform(in.begin(), in.end(), out.begin(), make_lower_case<T::value_type>());
  return out;
}

struct select1st
{
  template<class T1, class T2>
  const T1& operator()(const std::pair<T1, T2> & p) const
  { return p.first; }
};

struct select2nd
{
  template<class T1, class T2>
  const T2& operator()(const std::pair<T1, T2> & p) const
  { return p.second; }
};

struct front_extractor
{
  template<class container_type>
  const typename container_type::value_type& operator()(const container_type &container) const
  { return container.front(); }

};

template<class container_type>
struct container_creator  : std::unary_function<typename container_type::value_type, container_type>

{
  result_type operator()(const argument_type &front) const
  { 
    container_type container;
    container.insert(container.begin(), front);
    return container;
  }
};

template<class T>
std::list<T> make_list(const T &front)
{
  std::list<T> li;
  li.push_front(front);
  return li
}

template<class T>
class tree_node
{
public:
  typedef T value_type;

private:
  typedef smart_ptr<tree_node<value_type> > smart_node;
  
  smart_node mParent;
  std::list<tree_node<value_type> > mChildren;
  T mUserObject;

public:
  tree_node(const T& value) 
  : mUserObject(value) {}


  bool isRoot() { mParent.isNull(); }
  bool isLeafe() { return children().empty(); }
  
  const tree_node& parent() { return mParent; }
  const std::list<tree_node<T> >& children() const { return mChildren; }
  
  value_type& object() { return mUserObject; }
  const value_type& object() const { return mUserObject; }

  void addChild(const tree_node &child)
  {
    mChildren.push_back(child);
  }
};


struct file_name_is : public std::binary_function<std::string,std::string,bool>
{
  result_type operator()(const std::string &full, const std::string &name) const
  { return nameof(full) == name; }

  static std::string nameof(const std::string &full) 
  {
    size_t start = full.find_last_of("\\/");
    size_t end = full.rfind(".");

    start = start == std::string::npos ? 0 : start + 1;
    return full.substr(start, end-start);
  }
};

struct file_suffix_is : public std::binary_function<std::string,std::string,bool>
{
  result_type operator()(const std::string &full, const std::string &suffix) const
  { return suffixof(full) == suffix; }

  static std::string suffixof(const std::string &full)
  {
    size_t start = full.rfind(".");
    return start != std::string::npos ? full.substr(start+1, std::string::npos) : "";
  }
};

struct file_path_is : public std::binary_function<std::string,std::string,bool>
{
  result_type operator()(const std::string &full, const std::string &path) const
  { return pathof(full) == path; }

  static std::string pathof(const std::string &full)
  {
    size_t start = full.find_last_of("\\/");
    return start != std::string::npos ? full.substr(0, start+1) : "";
  }
};

inline long xxx_hash(const std::string &str)
{
#if _MSC_VER > 1200  // VC7
  where the hell is the has-Function defined ?!?!
#else
  return std::hash<const char*>()(str.c_str());
  //std::collate<char>().hash(std.begin(), str.end());
#endif
}

#endif
