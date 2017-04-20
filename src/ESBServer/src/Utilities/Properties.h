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

#ifndef Properties_H
#define Properties_H

#include <string>
#include <set>
#include <map>
#include <stdexcept>
#include <fstream>
#include <iterator>

#pragma warning( disable: 4290 )

template<char Delimiter='='>
class Property : public std::pair<std::string, std::string>
{

public:
  Property(){}
  Property(const std::pair<std::string, std::string> &p)
  : std::pair<std::string, std::string>(p) {}

  explicit Property(const std::string &line)
  {
    size_t pos = line.find(Delimiter);
    if(pos != std::string::npos) {
      first = line.substr(0, pos);
      second = line.substr(pos+1, std::string::npos);
      std::replace_if(second.begin(), second.end(), std::bind2nd(std::equal_to<char>(), '\t'), '\n');
    }
  }

  Property(const std::string &name, const std::string &value)
   : std::pair<std::string, std::string>(name, value){ }

  ~Property(){}

  Property& operator=(const std::pair<std::string, std::string> &p)
  { std::pair<std::string, std::string>::operator=(p); }

  const std::string& getName() const { return first; }
  const std::string& getValue() const { return second; }

  friend std::ostream& operator<<(std::ostream &os, const Property& p)
  { 
    if(!p.getName().empty()) {
      std::string value = p.getValue();
      std::replace_if(value.begin(), value.end(), std::bind2nd(std::equal_to<char>(), '\n'), '\t');
      os << p.getName() << Delimiter << value << std::endl;
    }
    return os;
  }

  friend std::istream& operator>>(std::istream &is, Property& p)
  {
    char buffer[2048];
    is.getline(buffer, sizeof(buffer));
    p = Property(buffer);
    return is;
  }
};

class Properties
{
private:
  struct PropertyComperator
  {
    bool operator()(const Property<'='> &lhs, const Property<'='> &rhs) const
    { return lhs.getName() < rhs.getName(); }
  };

  typedef Property<'='> PropertyType;
  typedef std::set<PropertyType , PropertyComperator> DatastoreType;

  std::string mFilename;
  DatastoreType mDatastore;

public:
  Properties(const std::string &filename = "");
  virtual ~Properties();

  void load(const std::string &filename = "");
  void dump(const std::string &filename = "");

  std::string getProperty(const std::string &name) const throw(std::invalid_argument);
  std::string getProperty(const std::string &name, const std::string &defaultValue) const throw();
  
  void setProperty(const std::string &name, const std::string &value);
  
  void removeProperty(const std::string &name);

  std::map<std::string, std::string> asMap() const;
  operator std::map<std::string, std::string> () const { return asMap(); }
  
private:
  const DatastoreType& getDatastore() const;
  DatastoreType& getDatastore();

  template<class StreamType>
  StreamType& openfile(StreamType &file, const std::string& filename)
  {
    std::string fn = filename.empty() ? mFilename : filename;
    file.open(fn.c_str());
    if(!file) {
      throw std::runtime_error("could not open file " + fn);
    }

    return file;
  }

};

class PropertiesManager : public Properties
{
public:
  static PropertiesManager& instance();

private:
  PropertiesManager(const std::string &file); 
};

#endif
