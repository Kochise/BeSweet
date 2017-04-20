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

#ifndef VSWorkspaceLoader_H
#define VSWorkspaceLoader_H

#include "Workspace.h"
#include "smart_ptr.h"

#include <string>
#include <list>
#include <functional>

class VisualStudioWorkspaceLoader
{
private:
  std::string mName;

public:
  VisualStudioWorkspaceLoader(const std::string &n) : mName(n) {};

	smart_ptr<Workspace> load(const std::list<std::string> &projects);
	smart_ptr<Workspace> load(const std::list<Package> &projects);

private:

};

class VSProjectLoader : public std::unary_function<std::string, Package>
{
public:
  bool setup(Package &package);

	bool operator()(Package &package) 
  { return setup(package); }

	Package load(const std::string&) const;

	Package operator() (const std::string &project) const
  { return load(project); }

};

class ProjectFileParser
{
protected:
  std::string mFileIndicatorTag;

public:
  ProjectFileParser(const std::string &t) : mFileIndicatorTag(t) {}
  virtual ~ProjectFileParser() {};

  virtual std::list<std::string> parse(const std::string &dsp);

protected:
  virtual std::string fileFound(const std::string &) const = 0;

  bool isValidFile(const std::string &file) const;

  bool isRelativePath(const std::string &p)
  { return !isAbsolutePath(p); }
  
  bool isAbsolutePath(const std::string &p);


	std::string extractPath(const std::string&) const;
  std::string mergePaths(const std::string&, const std::string&) const;
	std::string extractFileName(const std::string &full) const;
};

class DotDSPParser : public ProjectFileParser
{
public:
  DotDSPParser() : ProjectFileParser("SOURCE=") {}
  virtual std::string fileFound(const std::string &t) const;
  
};

class DotVCPROJParser : public ProjectFileParser
{
public:
  DotVCPROJParser() : ProjectFileParser("RelativePath=") {}
  virtual std::string fileFound(const std::string &) const;

};
#endif
