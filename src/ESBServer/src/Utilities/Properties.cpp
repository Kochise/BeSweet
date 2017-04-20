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

#include "Properties.h"

#include <vector>
#include <algorithm>

using namespace std;

/////////////////////////////////////////////////
Properties::Properties(const string &filename)
: mFilename(filename)
{
  try {
    load();
  } catch(exception&) {
    if(!mFilename.empty()) throw;
  }
}

Properties::~Properties() 
{
}

void Properties::load(const string &filename)
{
  typedef istream_iterator<PropertyType > istream_iter;
  ifstream file;
  copy(istream_iter(openfile(file, filename)), istream_iter(), inserter(getDatastore(), getDatastore().end()));
}

void Properties::dump(const string &filename)
{
  typedef ostream_iterator<PropertyType > ostream_iter;
  ofstream file;
  copy(getDatastore().begin(), getDatastore().end(), ostream_iter(openfile(file, filename)));
}

void Properties::setProperty(const std::string &name, const std::string &value)
{
  getDatastore().erase(PropertyType(name,""));
  getDatastore().insert(PropertyType(name,value));
}

string Properties::getProperty(const string &name) const throw(std::invalid_argument)
{ 
  DatastoreType::const_iterator value = getDatastore().find(PropertyType(name, "")); 
  if(value == getDatastore().end()) {
    throw invalid_argument("property <" + name + "> not defined !");
  }
  return value->getValue();
}

string Properties::getProperty(const string &name, const std::string &defaultValue) const throw()
{ 
  try {
    return getProperty(name); 

  } catch(invalid_argument &) {
    return defaultValue;
  }
}

const Properties::DatastoreType& Properties::getDatastore() const
{
  return mDatastore;
}

Properties::DatastoreType& Properties::getDatastore()
{
  return mDatastore;
}

map<string, string> Properties::asMap() const
{
  map<string, string> properties;
  copy(getDatastore().begin(), getDatastore().end(), inserter(properties, properties.end()));

  for(DatastoreType::const_iterator p = mDatastore.begin(); p != mDatastore.end(); p++) {
    properties[p->getName()] = p->getValue();
  }
  
  return properties;
}

#include <direct.h>

std::string pwd()
{
  vector<char> buffer(1024);
  return _getcwd(&buffer[0], buffer.size());
}

PropertiesManager::PropertiesManager(const std::string &file)
  :Properties(pwd() + "\\" + file)
{
}

PropertiesManager& PropertiesManager::instance()
{
  static PropertiesManager theInstance("esbserver.ini");
  return theInstance;
}
