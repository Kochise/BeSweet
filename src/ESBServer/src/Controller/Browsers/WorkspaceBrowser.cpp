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
#include "Resource.h"


#include "WorkspaceBrowser.h"
#include "WorkspaceBrowserRenderer.h"

#include "Editor.h"

using namespace std;

WorkspaceBrowserItem::WorkspaceBrowserItem(const std::string &scope, const std::string &name) 
: mName(name), mScope(scope) 
{
}

std::string WorkspaceBrowserItem::getFile() const
{
  return mName;
}

string WorkspaceBrowserItem::getNameLabel() const 
{ 
  return extractFileName(mName); 
}

string WorkspaceBrowserItem::getScopeLabel() const 
{ 
  return file_name_is::nameof(mScope) + "." + file_suffix_is::suffixof(mScope) + " - " + file_path_is::pathof(mName);
}

int WorkspaceBrowserItem::getNameIcon() const 
{ 
	if(mName.rfind(".h") != string::npos || mName.rfind(".hpp") != string::npos) {
		return 0;
	} else if(mName.rfind(".c") != string::npos || mName.rfind(".cpp") != string::npos) {
		return 1;
	}

  return -1;
}
  
int WorkspaceBrowserItem::getScopeIcon() const 
{ 
  return 2;
}

string WorkspaceBrowserItem::extractFileName(const std::string &full) const
{
	size_t pos = full.rfind("\\");
	if(pos == string::npos) {
		return full;
	} 

	return full.substr(pos+1, string::npos);
}


WorkspaceBrowserModel::WorkspaceBrowserModel(const Workspace &w)
{
  typedef map<string, set<string> > PackageList;

  for(list<Package>::const_iterator package = w.getPackages().begin(); package != w.getPackages().end(); package++) {
    for(set<string>::const_iterator file = package->getFiles().begin(); file != package->getFiles().end(); file++) {
      getDatastore().push_back(make_item(package->getName(), *file));
    }
  }
}

smart_ptr<ScopedNameBrowserItem> WorkspaceBrowserModel::make_item(const std::string &scope, const std::string &name)
{
  return smart_ptr<ScopedNameBrowserItem>( new WorkspaceBrowserItem(scope, name));
}


WorkspaceBrowser::WorkspaceBrowser()
{
  setRenderer(smart_ptr<ScopedNameBrowserRenderer>(new WorkspaceBrowserRenderer()));
}
