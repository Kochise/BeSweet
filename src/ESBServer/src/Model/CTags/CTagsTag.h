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

#ifndef Tag_H
#define Tag_H

#include <Model/MetaModel/MetaModel.h>

#include <vector>
#include <string>
#include <list>

namespace CTags {

class Tag
{
private:
  mutable long mHash;
  mutable long mWeakHash;

  std::string mFile;
  std::string mCode;
  std::list<std::string> mInheritance;
  std::list<std::string> mSignature;

  MetaObjectType mType;

  std::string empty;
  bool mIsDeclaration;
  
  size_t mLine;
  
  std::string mScope;
  MetaObjectType mScopeType;

  FullQualifiedName mFQN;
  
  AccessQualifier mAccessQualifier;
  ImplementationQualifier mImplementationQualifier;


public:
  Tag(){};

  explicit Tag(std::string line);

  long hash() const;
  long weakhash() const;

	bool isBuddyOf(const Tag&) const;

  //const std::string& getName() const { return getFQN().getName(); }
  const std::string& getFile() const { return mFile; }
  const std::string& getCode() const { return mCode; }
  const std::list<std::string>& getInheritance() const { return mInheritance; }

  MetaObjectType getType() const { return mType; }
  const std::list<std::string>& getSignature() const { return mSignature; }

//  std::string getScope() const { return getFQN().getScope(); };
  MetaObjectType getScopeType() const { return mScopeType; };
  
  const FullQualifiedName& getFQN() const { return mFQN; }
  
  ImplementationQualifier getImplementationQualifier() const { return mImplementationQualifier; };
  AccessQualifier getAccessQualifier() const { return mAccessQualifier; };

  size_t getLine() const { return mLine; }

  bool isDeclaration() const { return mIsDeclaration; }

  std::string getReturnType() const;

  //bool operator<(const Tag &rhs) const { return hash() < rhs.hash(); };
  
private:
  std::string getMethodHash() const;

  void prepare(std::string&);
  bool setupTagType(const std::string &);
  bool setupScope(const std::string &);
  bool setupLine(const std::string&);
  bool setupImplementationQualifier(const std::string&);
  bool setupAccessQualifier(const std::string&);
  bool setupInheritance(const std::string&);
  bool setupSignature(const std::string&);

  long generateHash(bool weak) const;

};

struct isInFile : std::binary_function<std::string, Tag, bool>
{
  result_type operator() (const first_argument_type &file, const second_argument_type &tag) const
  { return tag.getFile() == file; }
};

} //namespace CTags

std::ostream& operator<<(std::ostream&, const CTags::Tag&);

#endif
