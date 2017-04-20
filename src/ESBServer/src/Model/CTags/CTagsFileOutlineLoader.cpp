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

#include <Model/MetaModel/MetaModel.h>

#include <Model/CTags/CTagsFileOutlineLoader.h>
#include <Model/CTags/CTagsModelBuilder.h>

#include <Utilities/Properties.h>
#include <Utilities/smart_ptr.h>
#include <Utilities/stl_ext.h>
#include <Utilities/StringTokenizer.h>

#include <sstream>
#include <map>
#include <algorithm>
#include <iterator>


using namespace std;
using FileOutlineLoader::TagList;
using CTags::Database::TagTable;

namespace Browsers{
  namespace Outline
  {
    extern const std::string SHOW_HEADER_AND_SOURCE;
  }
}
using namespace Browsers::Outline;

namespace CTags {

FileOutlineLoader::FileOutlineLoader(const smart_ptr<Workspace> &wspace)
: mWorkspace(wspace)
{

}


bool same_name(string fullname, string target)
{
  if(file_name_is::nameof(fullname) != file_name_is::nameof(target)) {
    return false;
  }
  
  string suffix = file_suffix_is::suffixof(fullname);
  string targetsuffix = file_suffix_is::suffixof(target);
  if(targetsuffix == "h" || targetsuffix == "hpp") {
    return suffix == "cpp";

  } else if(targetsuffix == "cpp") {
    return suffix == "h" || suffix == "hpp";
  }

  return false;
}

TagList FileOutlineLoader::load(const string &file)
{
  list<string> files;
  if(PropertiesManager::instance().getProperty(SHOW_HEADER_AND_SOURCE, "") == "1") {
    files = mWorkspace->getFiles();
    //files.remove_if(not1(bind2nd(file_name_is(), file_name_is::nameof(file))));
    files.remove_if(not1(bind2nd(ptr_fun(same_name), file)));

  }
  files.push_back(file);
  //remove duplicates
  files.sort();  
  files.unique();

  return load(files);
}

TagList FileOutlineLoader::load(const list<string> &files)
{
  return loadFiles(files);
}


TagList FileOutlineLoader::loadFiles(const list<string> &files)
{
  smart_ptr<CTags::Database> db = DatabaseManager::instance().getDatabase(mWorkspace);
  
  //select tags from all requiered files....
  TagTable fileTags;
  for(list<string>::const_iterator file = files.begin(); file != files.end(); file++) {
    fileTags.splice(fileTags.end(), db->selectTagsWhere(make_query(FileIs(), *file)));
  }

  //select all "scopes"....
  TagTable tagTable;
  copy_if(fileTags.begin(), fileTags.end(), inserter(tagTable, tagTable.begin()), make_query(TypeIs(), TypeIs::ANY_TYPE | eNamespace | eFunction | eVariable));
  
  //remove everything where the scope-types isn't a 
  //namespace (global scope is considered as namespace !)
  tagTable.remove_if(not1(bind2nd(ScopeTypeIs(), eNamespace)));

  return createModelFromTagList(tagTable);
}

TagList FileOutlineLoader::createModelFromTagList(Database::TagTable &table)
{
  return ModelBuilder(mWorkspace).createModel(table);
}

} //namespace CTags

