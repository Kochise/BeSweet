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

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <list>
#include <set>
#include <string>

class Package
{
public:
	typedef std::set<std::string> FileSet;

private:
	FileSet mFiles;
	std::string mName;

public:
  Package() {}
	Package(const std::string &name, const FileSet &files) 
	: mName(name), mFiles(files) {}

	const std::string& getName() const { return mName; }

	FileSet& getFiles() { return mFiles; }
  const FileSet& getFiles() const { return mFiles; }

  static Package create(const std::string &name)
  { return Package(name, FileSet()); }

};

class Workspace
{
private:
	std::list<Package> mPackages;
  std::string mName;


public:
	Workspace(){};
  Workspace(const std::string& n, const std::list<Package> &p) : mName(n), mPackages(p) {};
	virtual ~Workspace() {};

  const std::string& getName() const { return mName; }
	std::list<Package>& getPackages() { return mPackages; }
  const std::list<Package>& getPackages() const { return mPackages; }

  std::list<std::string> getFiles() const
  {
    std::list<std::string> files;
    for(std::list<Package>::const_iterator package = getPackages().begin(); package != getPackages().end(); ++package) {
      files.insert(files.end(), package->getFiles().begin(), package->getFiles().end());
    }
    return files;
  }

};
/*
class WorkspaceManager
{
private:
  std::map<std::string, smart_ptr<Workspace> > mWorkspaces;

public:
  WorkspaceManager& instance();

  void addWorkspace(const smart_ptr<Workspace> &);
  smart_ptr<Workspace> getWorkspace(const std::string&);
  
};*/

#endif
