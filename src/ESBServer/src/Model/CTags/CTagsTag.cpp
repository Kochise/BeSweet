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

#include "stdAfx.h"

#include "CTagsTag.h"
#include "NameMangler.h"
#include "StringTokenizer.h"

#include <stl_ext.h>

#include <sstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <string>

using namespace std;

namespace CTags {

Tag::Tag(string line) 
: mIsDeclaration(true), mImplementationQualifier(eDefault), mAccessQualifier(ePublic),
  mScopeType(eNamespace), mScope(GLOBAL_NAMESPACE), mLine(0), mHash(0), mWeakHash(0)
{
    prepare(line);
    StringTokenizer<vector<string> > tokenizer(line, "\t");

    string name = FullQualifiedName(tokenizer.tokens()[0]);
    mFile = tokenizer.tokens()[1];
    mCode = tokenizer.tokens()[2];

    //remove_if(mCode.begin(), mCode.end(), bind2nd(equal_to<string>(), "/^"));
    mCode.erase(mCode.find("/^"), 2);
    mCode.erase(mCode.find("/;\""), string::npos);
    
    for(vector<string>::const_iterator token = tokenizer.tokens().begin()+3; token != tokenizer.tokens().end(); token++) {
      setupTagType(*token);
      setupScope(*token);
      setupLine(*token);
      setupImplementationQualifier(*token);
      setupAccessQualifier(*token);
      setupInheritance(*token);
      setupSignature(*token);
    }

    
    if(getType() == eFunction || getType() == eMethod) {
      mType = getScopeType() == eNamespace ? eFunction : eMethod;

    } else if(getType() == eTypedef) {
			mType = getCode().find("enum") != string::npos ? eEnum : eTypedef;

    } else if(getType() == eEnumMember) {
      size_t pos = mScope.rfind("::<anonymous>");
      if(pos != string::npos) {
        mScope = mScope.substr(0, pos);
      }
    } else if(getType() == eField && getScopeType() == eNamespace) {
      mType = eVariable;
    }

    if(mScope.empty() && (mType == eMethod || mType == eField)) {
      //message_box(("WTF: " + line).c_str());
      mScope="???";
    }
    mFQN = FullQualifiedName(mScope, name);

    if(mLine == 0 || mFile.empty()) {
      log(GENERAL, CTAGS, eError) << "ctags-tag doesn't contains a correct file location ?!?! "<< endlog;
    }
}

bool Tag::setupTagType(const std::string &token)
{
  size_t pos = token.find(":");
  if(pos != string::npos && token.substr(0, pos) == "kind") {
    string type = token.substr(pos+1, string::npos);

    if(type == "namespace") {
      mType = eNamespace;

    } else if(type == "class") {
      mType = eClass;

    } else if(type == "struct") {
      mType = eStruct;

    } else if(type == "union") {
      mType = eUnion;

    } else if(type == "enum") {
      mType = eEnum;

    } else if(type == "enumeration") {
      mType = eEnumMember;

    } else if(type == "enumerator") {
      mType = eEnumMember;

    } else if(type == "prototype") {
      mType = eFunction;
      //mType = getScope() == GLOBAL_NAMESPACE ? eFunction : eMethod;

    } else if(type == "member") {
      mType = eField;

    } else if(type == "function") {
      mType = eFunction;
      //mType = getScope() == GLOBAL_NAMESPACE ? eFunction : eMethod;
      mIsDeclaration = false;

    } else if(type == "typedef") {
      mType = eTypedef;

    } else if(type == "variable") {
      mType = eVariable;

    } else {
      return false;
    }
  }

  return true;
}

long Tag::hash() const
{
  if(mHash == 0) {
    mHash = generateHash(false);
  }
  return mHash;
}

long Tag::weakhash() const
{
  if(mWeakHash == 0) {
    mWeakHash = generateHash(true);
  }
  return mWeakHash;
}

long Tag::generateHash(bool weak) const
{
  string hashString;
  if(getType() == eMethod || getType() == eFunction) {
     hashString = getMethodHash();
  } else {
     hashString = getFQN().name();
  }
  if(!weak) {
    hashString = getFQN().scope() + "::" + hashString;
  } 
  
  return xxx_hash(hashString);
}

std::string Tag::getMethodHash() const
{
  return NameMangler().getMethodHash(getFQN().name(), getCode());
}

bool Tag::setupScope(const std::string &scope) 
{
  size_t pos = scope.find(":");
  if(pos != string::npos) {
    string scopeType = scope.substr(0, pos);

    if(scopeType == "namespace") {
      mScopeType = eNamespace;

    } else if(scopeType == "class") {
      mScopeType = eClass;

    } else if(scopeType == "struct") {
      mScopeType = eStruct;

    } else if(scopeType == "union") {
      mScopeType = eUnion;

    } else if(scopeType == "enum") {
      mScopeType = eEnum;

    } else {
      //message_box(("unknown scope type " + scopeType).c_str());
      return false;
    }

    mScope = scope.substr(pos+1, string::npos);
  }
  return false;
}

bool Tag::setupLine(const std::string &token)
{
  size_t pos = token.find(":");
  if(pos != string::npos && token.substr(0, pos) == "line") {
    mLine = atol(token.substr(pos+1, string::npos).c_str());
    return true;
  } 

  return false;
}

bool Tag::setupImplementationQualifier(const std::string &token)
{
  size_t pos = token.find(":");
  if(pos != string::npos && token.substr(0, pos) == "implementation") {
    string impl = token.substr(pos+1, string::npos);
    if(impl == "pure virtual") {
      mImplementationQualifier = eAbstract;

    } else if(impl == "virtual") {
      mImplementationQualifier = eVirtual;

    } else {
      log(GENERAL, CTAGS, eWarning) << "Tag::setupImplementationQualifier(): unkown implementation specifier " << token << endlog;      
      return false;
    }

    return true;
  }

  return false;
}

bool Tag::setupAccessQualifier(const std::string &token)
{
  size_t pos = token.find(":");
  if(pos != string::npos && token.substr(0, pos) == "access") {
    string access = token.substr(pos+1, string::npos);
    if(access == "public") {
      mAccessQualifier = ePublic;

    } else if(access == "protected") {
      mAccessQualifier = eProtected;

    } else if(access == "private") {
      mAccessQualifier = ePrivate;

    } else if(access == "friend") {
      /*ignore*/;

    } else {
      log(GENERAL, CTAGS, eWarning) << "Tag::setupAccessQualifier() unkown access specifier " << token << endlog;      
      return false;
    }

    return true;
  }

  return false;
}

void Tag::prepare(string &line)
{
  if(line.empty()) {
   throw invalid_argument("line is empty"); 
  }

  //strip tab in /^ the code line $/;
  size_t codeStart = line.find("/^");
  size_t codeEnd = line.find("/;\"");
  char blank = ' ';

  if(codeStart == string::npos || codeEnd == string::npos) {
    throw invalid_argument("code-line seems to be invalid"); 
  }
  
  replace_if(line.begin()+codeStart, line.begin()+codeEnd, bind2nd(equal_to<char>(), '\t'), blank);
  replace_if(line.begin()+codeStart, line.begin()+codeEnd, bind2nd(equal_to<char>(), '$'), blank);
}

std::string Tag::getReturnType() const 
{
  if(getType() == eMethod || getType() == eFunction || getType() == eField || getType() == eVariable) {
    size_t pos;
    string returnType;

    if((pos = getCode().find(getFQN().toString())) != string::npos) {
      returnType = getCode().substr(0, pos);

    } else {
      //void Namespace::Class::create()
      int end = getCode().find(getFQN().name());
      int pos = getCode().rfind(" ", end);
      if(pos != string::npos) {
        returnType = getCode().substr(0, pos);
      }
    }

    if((pos = returnType.find("virtual")) != string::npos) {
      returnType.erase(pos, 7);

    }// else if ((pos = returnType.find("static")) != string::npos) {
     // returnType.erase(pos, 6);
    //}
    return returnType;
  }

  return "";
}

bool Tag::setupInheritance(const std::string &token)
{
  size_t pos = token.find(":");
  if(pos != string::npos && token.substr(0, pos) == "inherits") {
    string parents = token.substr(pos+1, string::npos);
    mInheritance = StringTokenizer<list<string> >(parents, ",").tokens();
    return true;
  }
  
  return false;
}

bool Tag::setupSignature(const std::string &token)
{
  size_t pos = token.find(":");
  if(pos != string::npos && token.substr(0, pos) == "signature") {
    string signature = token.substr(pos+1, string::npos);
    vector<string> parameters = NameMangler().tokenizeParameterlist(signature);
    mSignature.assign(parameters.begin(), parameters.end());

    stringstream ss;
    copy(mSignature.begin(), mSignature.end(), ostream_iterator<string>(ss, "||"));
    string x = ss.str();

    return true;
  }
  
  return false;
}

bool Tag::isBuddyOf(const Tag &tag) const
{
	if(hash() != tag.hash()) {
		return false;
	}

	return (isDeclaration() && !tag.isDeclaration()) ||
		    (!isDeclaration() &&  tag.isDeclaration());
}

} //namespace CTags

std::ostream& operator<<(std::ostream &os, const CTags::Tag &tag)
{
  MetaObjectTypeTranslator translator(tag.getType());
  return os << tag.getFQN() << endl
    << "type: " << translator.asString() << hex << "(0x" << translator.asType() << ")" << dec << endl
    << tag.getFile() << ":" << tag.getLine();
}
