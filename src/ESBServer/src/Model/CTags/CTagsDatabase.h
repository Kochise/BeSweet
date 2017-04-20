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

#ifndef Database_H
#define Database_H

#pragma warning( disable: 4800 )

#include <string>
#include <list>
#include <set>
#include <map>
#include <functional>
#include "stl_ext.h"

#include <Model/CTags/CTagsTag.h>
#include <Model/Workspace.h>

namespace CTags {


class Database
{
public:
  typedef std::list<CTags::Tag> TagTable;

private:
  std::string mName;
  TagTable mTags;

public:
  Database() {};
  Database(const std::string& name);
  Database(const std::list<std::string>& files);
  Database(const Workspace& workspace);


  template<class QueryType>
	TagTable selectTagsWhere(const QueryType &query)
	{
		TagTable result;
		copy_if(mTags.begin(), mTags.end(), std::inserter(result, result.end()), query);
		return result;
	}

  void refreshTagsReferencingFile(const std::string&);

private:
  const TagTable& getTags() const { return mTags; }
  void createTagFile(const std::list<std::string>&, const std::string&);
  void eraseTagsReferencingFile(const std::string&);
  TagTable loadTags(const std::string&);
  void setupTagTable(const std::list<std::string>&files);
};

template<class QueryType, class ArgumentType>
std::binder2nd<QueryType> make_query(const QueryType &query, const ArgumentType &arg)
{
  return std::bind2nd(query, arg);
}

struct TypeIs : public std::binary_function<CTags::Tag, int, bool>
{
	const static int ANY_TYPE;
		
	bool operator()(const first_argument_type &tag, second_argument_type type) const
	{ return static_cast<bool>(tag.getType() & type); }
};

struct ScopeTypeIs : public std::binary_function<CTags::Tag, int, bool>
{
  bool operator()(const first_argument_type &tag, const second_argument_type &type) const
  { return static_cast<bool>(tag.getScopeType() & type); }
};

struct FileIs : public std::binary_function<CTags::Tag, std::string, bool>
{
	bool operator()(const first_argument_type &tag, const second_argument_type &file) const
	{ return tag.getFile()  == file; }
};

struct ScopeOrNameIs : public std::binary_function<CTags::Tag, std::string, bool>
{
	bool operator()(const first_argument_type &tag, const second_argument_type &id) const
	{ return tag.getFQN().scope() == id || tag.getFQN().name() == id; }
};

struct NameIs : public std::binary_function<CTags::Tag, std::string, bool>
{
	bool operator()(const first_argument_type &tag, const second_argument_type &id) const
	{ return tag.getFQN().name() == id; }
};

struct ScopeNameIs : public std::binary_function<CTags::Tag, std::string, bool>
{
	bool operator()(const first_argument_type &tag, const second_argument_type &id) const
	{ return tag.getFQN().scope() == id; }
};

struct FullQualifiedNameIs : public std::binary_function<CTags::Tag, FullQualifiedName, bool>
{
	result_type operator()(const first_argument_type &tag, const second_argument_type &fqn) const
	{ return tag.getFQN() == fqn; }
};

struct TagSorter : public std::binary_function<CTags::Tag, CTags::Tag, bool>
{
	result_type operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
	{ return lhs.getFQN().toString() < rhs.getFQN().toString(); }  
};

struct TagComperator : public std::binary_function<CTags::Tag, CTags::Tag, bool>
{
	result_type operator()(const first_argument_type &lhs, const second_argument_type &rhs) const
	{ return lhs.getFQN() == rhs.getFQN(); }  
};

class DatabaseManager
{
private:
  typedef  std::map<std::string, smart_ptr<Database> > DBMap;

  DBMap mManagedDatabases;

public:
  static DatabaseManager& instance();

  smart_ptr<Database> getDatabase(const smart_ptr<Workspace> &);
  smart_ptr<Database> removeDatabase(const smart_ptr<Workspace> &);

private:
  DatabaseManager();
};

} // namespace CTags
#endif
