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

#ifndef FullQualifiedName_H
#define FullQualifiedName_H

#include <string>
#include <iostream>

extern const std::string GLOBAL_NAMESPACE;
;

class FullQualifiedName
{
private:
  std::string mScope;
  std::string mName;
  const static std::string empty;
  
public:
  FullQualifiedName(){}
  FullQualifiedName(const std::string &fqn);
  FullQualifiedName(const std::string& scope, const std::string& name);

  const std::string& scope()  const;
  const std::string& name() const { return mName; };

  std::string toString() const;

  operator std::string() const { return toString(); }

  bool operator ==(const FullQualifiedName& rhs) const
  { return scope() == rhs.scope() && name() == rhs.name(); }
  
  bool operator< (const FullQualifiedName& rhs) const
  { return toString() < rhs.toString(); }

  friend std::ostream& operator<<(std::ostream& os, const FullQualifiedName &fqn)
  { return os << fqn.toString(); }
  
private:
  void setScope(const std::string&);
	static std::string getScopeOf(const std::string &fqn);
	static std::string getNameOf(const std::string &fqn);
};

#endif
