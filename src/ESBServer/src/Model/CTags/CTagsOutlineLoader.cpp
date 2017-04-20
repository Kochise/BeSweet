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

#include "CTagsOutlineLoader.h"

#include <Model/CTags/CTagsDatabase.h>
#include <Model/CTags/CTagsModelBuilder.h>

using namespace std;
using CTags::Database::TagTable;

namespace CTags {
  
OutlineLoader::OutlineLoader(const smart_ptr<Workspace> &wspace)
  : mWorkspace(wspace)
{
}

MetaObject& OutlineLoader::load(MetaObject &object)
{
  return object;
}

std::list<smart_ptr<MetaObject> > OutlineLoader::loadOutlineOf(const MetaObject &object) const
{
  smart_ptr<Database> db = DatabaseManager::instance().getDatabase(mWorkspace);
  Database::TagTable tags = db->selectTagsWhere(make_query(ScopeNameIs(), object.getFQN()));
  return ModelBuilder(mWorkspace).createModel(tags);
}

}