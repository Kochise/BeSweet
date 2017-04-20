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

#ifndef MetaModelPolicies_h
#define MetaModelPolicies_h

#include <Model/MetaModel/MetaModelDefines.h>

class AccessPolicy
{
public:
  bool isPublic() const { return getAccessQualifier() == ePublic; }
  bool isProtected() const { return getAccessQualifier() == eProtected; }
  bool isPrivate() const { return getAccessQualifier() == ePrivate; }
  
  virtual AccessQualifier getAccessQualifier() const  = 0;
  virtual void setAccessQualifier(AccessQualifier q) = 0;
  
};

class ConstStaticPolicy
{
public:
  enum ConstStatic { eConst = 0x01, eStatic = 0x02 };
 
public:
  virtual ~ConstStaticPolicy(){};
  
  void isStatic(bool isStatic) { mark(isStatic, eStatic); }
  bool isStatic() const { return (getConstStatic() & eStatic) != 0; }
  
  void isConst(bool isConst) { mark(isConst, eConst); }
  bool isConst() const { return (getConstStatic() & eConst) != 0; }

private:
  void mark(bool set, ConstStatic flag)
  {
    if(set) {
      setConstStatic(getConstStatic() | flag);
    } else {
      setConstStatic(getConstStatic() ^ flag);
    }
  }
  
  virtual char getConstStatic() const = 0;
  virtual void setConstStatic(char) = 0;
};

class ImplementationPolicy
{
public:
  bool isVirtual() const { return getImplementationQualifier() == eVirtual; }
  bool isAbstract() const { return getImplementationQualifier() == eAbstract; }
  
private:
  virtual ImplementationQualifier getImplementationQualifier() const = 0;
  virtual void setImplementationQualifier(ImplementationQualifier q)  = 0;
};

#endif