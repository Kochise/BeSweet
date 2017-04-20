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

#include "VSWorkspaceLoader.h"
#include <Utilities/StringTokenizer.h>
#include <Utilities/stl_ext.h>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

string ProjectFileParser::extractPath(const std::string &full) const
{
	int pos = full.rfind("\\");
  string path = pos == string::npos ? string() : full.substr(0, pos);
	if(path.size() >= 1 && path[0] == '.' ) {
		
    if(path.find("..") == string::npos) {
      path = path.substr(1, string::npos);
    }
  }

	return path;
}

string ProjectFileParser::mergePaths(const string &abs, const  string &rel) const
{
  vector<string> absolute = StringVectorTokenizer(abs, "\\/").tokens();
  vector<string> relative = StringVectorTokenizer(rel, "\\/").tokens();

  int dotdotCnt = count(relative.begin(), relative.end(), "..");

  stringstream ss;
  vector<string> full(absolute.begin(), absolute.end()-dotdotCnt);
  full.insert(full.end(), relative.begin()+dotdotCnt, relative.end());
  
  copy(full.begin(), full.end(), ostream_iterator<string>(ss, "\\"));
  return ss.str();
}

string ProjectFileParser::extractFileName(const std::string &full) const
{
	size_t pos = full.rfind("\\");
	if(pos == string::npos) {
		return full;
	} 

	return full.substr(pos+1, string::npos);
}

bool VSProjectLoader::setup(Package &package)
{
  package = load(package.getName());
  return true;
}

list<string> ProjectFileParser::parse(const std::string &dsp)
{
  list<string> files;
  ifstream file(dsp.c_str());
  if(!file) {
    throw invalid_argument("project file " + dsp + " not found");
  }

  string projectPath = file_path_is::pathof(dsp);

  string line;
  size_t pos;
  while(!file.eof()) {
    getline(file, line);
    if((pos = line.find(mFileIndicatorTag)) != string::npos && isValidFile(line)){
      string full = line.substr(pos+mFileIndicatorTag.size(), string::npos);
      full = fileFound(full);
      
      if(isRelativePath(full)) {
        string relativePath = extractPath(full);
        full = mergePaths(projectPath, relativePath) + extractFileName(full);
      }
      files.push_back(to_lower(full));
    }
  }
  return files;
}

bool ProjectFileParser::isAbsolutePath(const std::string &path)
{
  //  /path/file.h
  if(!path.empty() && (path[0] == '\\' || path[0] == '/')) {
    return true;
  
  // d:/path/file/file.h  
  } else if(path.size() > 1 && (isalpha(path[0]) && path[1] == ':')) {
    return true;
  }

  return false;
}


bool ProjectFileParser::isValidFile(const std::string &file) const
{
	return	file.find(".h") != string::npos || 
					file.find(".hpp") != string::npos || 
					file.find(".cpp") != string::npos;
}

string DotDSPParser::fileFound(const std::string &file) const
{
  //visual studio sometimes fucks up the case of the file-path (converts all chars to upper !)
  size_t begin = file.find('"');
  size_t end = file.rfind('"');
  
  return begin != string::npos ? to_lower(file.substr(begin+1, end-begin-1)) : to_lower(file);
}

string DotVCPROJParser::fileFound(const std::string &file) const
{
  size_t begin = file.find('"');
  size_t end = file.rfind('"');
  return to_lower(file.substr(begin+1, end-begin-1));
}

Package VSProjectLoader::load(const std::string &dsp) const
{
  smart_ptr<ProjectFileParser> parser;
  if(to_lower(dsp).find(".dsp") != string::npos) {
    parser = make_smart_ptr(DotDSPParser());

  } else if(to_lower(dsp).find(".vcproj") != string::npos) {
    parser = make_smart_ptr(DotVCPROJParser());
  }
  
  list<string> files = parser->parse(dsp);
  return Package(dsp, Package::FileSet(files.begin(), files.end()));
}

smart_ptr<Workspace> VisualStudioWorkspaceLoader::load(const list<string> &projects)
{
	list<Package> packages;
	transform(projects.begin(), projects.end(), inserter(packages, packages.begin()),
						VSProjectLoader());

	return smart_ptr<Workspace>(new Workspace(mName, packages));
}

smart_ptr<Workspace> VisualStudioWorkspaceLoader::load(const list<Package> &projects)
{
	list<Package> packages = projects;
  for_each(packages.begin(), packages.end(), VSProjectLoader());

	return smart_ptr<Workspace>(new Workspace(mName, packages));
}


