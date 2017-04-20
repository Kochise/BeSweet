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

#include <StdAfx.h>

#include <Model/CTags/CTagsTypeInfoLoader.h>
#include <Model/CTags/CTagsDatabase.h>
#include <Model/CTags/CTagsMetaObjectFactory.h>
#include <Model/CTags/CTagsModelBuilder.h>
#include <Model/Workspace.h>
#include <Utilities/StringTokenizer.h>

#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

namespace CTags {


using CTags::Database::TagTable;

TypeInfoLoader::TypeInfoLoader(const smart_ptr<Workspace> &wspace)
: mWorkspace(wspace)
{
}

TypeInfoLoader::TypeList TypeInfoLoader::load(const std::string &type_name)
{
	TagTable types = db()->selectTagsWhere(make_query(NameIs(), type_name));
  types.remove_if(bind2nd(TypeIs(), eMethod | eFunction | eField | eVariable));

  return make(types);
}

TypeInfoLoader::TypeList TypeInfoLoader::load()
{
  return make(db()->selectTagsWhere(make_query(TypeIs(), TypeIs::ANY_TYPE)));
}

TypeInfoLoader::TypeList TypeInfoLoader::make(const Database::TagTable &table)
{
  TypeFactory factory;
  factory.setBroker(LoaderBroker::instance(mWorkspace));
	
  TypeList result;
  transform(table.begin(), table.end(), inserter(result, result.begin()), factory);
  return result;
}

smart_ptr<Database> TypeInfoLoader::db()
{
  return DatabaseManager::instance().getDatabase(mWorkspace);
}

} //namespace CTags


