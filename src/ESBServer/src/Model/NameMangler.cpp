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

#include "NameMangler.h"
#include "StringTokenizer.h"
#include "stl_ext.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;
string::size_type npos = string::npos;

struct getParameterType
{
  std::string operator()(const string &parameter) const
  {
    if(parameter.empty()) {
      return "";
    }
    
    //strip name and default parameter "const string&x=string("5")" -->"int *&"
    
    //1) check if we have a default param foo(int i=0)
    //2) search parameter for modifiers "&*" up to 
    //  begin of default param i.e foo(int&*i=0)
    size_t eqs = parameter.find('=');
    size_t end = parameter.find_last_of("&*", eqs);
    
    //if we don't have any type modifiers (&*) search for the start
    //of the param name i.e foo(int i). if we have modifiers, make sure
    //they are copied as well !! (..the +1)
    end = end == npos ? parameter.rfind(' ') : end + 1;
  
    //ok, we don't even have a parameter name i.e foo(int)
    // so we can copy up to the default param name foo(int=0)
    if(end == npos) {
      end = eqs;
    }
    string type = end == npos ? parameter : parameter.substr(0, end);

    //now start to erase namespaces i.e std::list<std::string> --> list<string>
    while((end = type.rfind("::")) != npos)
    {
      size_t start = 0;
      start = type.find_last_of(" &*<", end);
      if(start == npos) {
        start = 0;

      } else if(type[start] == ' ') {
        start += 1; //lets keep the blank !!
      }

      type.erase(start, end+2-start);
    }
   
    return type;  
  }
};


std::string NameMangler::getMethodHash(const std::string &name, const std::string &_code) const
{
  string code = stripUnnecessaryWithspaces(_code);

  vector<string> parameters = tokenizeParameters(getParameters(code));
  transform(parameters.begin(), parameters.end(), parameters.begin(),getParameterType());

  stringstream hash;
  hash << name << "(";
  copy(parameters.begin(), parameters.end(), ostream_iterator<string>(hash,","));
  hash << ")";

  //if method is const
  if(code.find("const", code.find(")") ) != npos) {
    hash << "const";
  }

  string mangel = hash.str();
  mangel.erase(remove_if(mangel.begin(),mangel.end(),bind2nd(equal_to<char>(), ' ')), mangel.end());

  return mangel;
}

NameMangler::ParameterList NameMangler::tokenizeParameterlist(const std::string &code)
{
  vector<string> parameters = tokenizeParameters(getParameters(stripUnnecessaryWithspaces(code)));
  transform(parameters.begin(), parameters.end(), parameters.begin(),getParameterType());
  return parameters;
}


std::string NameMangler::stripUnnecessaryWithspaces(const std::string &str) const
{
  string result;
  if(!iswspace(str[0])) {
    result.insert(result.end(), str[0]);
  }

  for(size_t i = 1; i < str.length()-1; i++) {

    if(!iswspace(str[i])) {
      result.insert(result.end(), str[i]);
    } 
    else if(isalnum(str[i+1]) && isalnum(result[result.length()-1])) {
      result.insert(result.end(), str[i]);
    } 
  }

  if(!iswspace(str[str.length()-1])) {
    result.insert(result.end(), str[str.length()-1]);
  }

  //std::cout << "stripped: " << result << std::endl;
  return result;
}

std::string NameMangler::getParameters(const string &hashValue) const
{
  size_t start = hashValue.find('(');
  size_t end = hashValue.rfind(')');
  return hashValue.substr(start, end-start+1);
}

vector<string> NameMangler::tokenizeParameters(const string &paramStr) const
{
  int openGT = 0;
  bool isParameterEnd = false;
  size_t parameterStart = 1;
  vector<string> parameters;
  for(size_t i = parameterStart; i < paramStr.length(); i++) {
    switch(paramStr[i]) {
      case ',' : if(!openGT) isParameterEnd = true; break;
      case '<' : openGT++; break;
      case '>' : openGT--; break;
      case ')' : if(i != paramStr.length()-1) ++i; isParameterEnd = true; break;
    }

    if(isParameterEnd) {
      isParameterEnd = false;
      parameters.push_back(paramStr.substr(parameterStart,i-parameterStart));
      parameterStart = i+1;
    }
  }

  return parameters;
}