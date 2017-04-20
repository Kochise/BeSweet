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

#ifndef IconDescriptor_H
#define IconDescriptor_H

#include <Model/MetaModel/MetaModel.h>

struct IconDecorators
{
  AccessQualifier accessDecorator;
  ImplementationQualifier implementationDecorator;
  
  bool hasAccessDecorator;
  bool hasConstDecorator;
  bool hasStaticDecorator;
  bool hasProblem;
  bool hasQuestion;
  bool isSelected;
  bool isPrototype;

  IconDecorators();
  size_t hash() const;
 
};

class IconDescriptor : public MetaObjectVisitor
{
public:
  MetaObjectType type;
  IconDecorators decorators;

public:
  IconDescriptor();

private:
  void decorate(const MetaObject&);
  void decorate(const Method&);
  void decorate(const Variable&);
  
  virtual void visitNamespace(Namespace*);
  virtual void visitClass(Class*);
  virtual void visitStruct(Struct*);
  virtual void visitUnion(Union*);
  virtual void visitEnum(Enum*);
  virtual void visitTypedef(Typedef*);
  virtual void visitField(Field*);
  virtual void visitVariable(Variable*);
  virtual void visitMethod(Method*);
  virtual void visitFunction(Function*);

  template<class T>
  void decorateConstStatic(const T &tag)
  {
    decorators.hasConstDecorator = tag.isConst();
    decorators.hasStaticDecorator = tag.isStatic();
  }

};

#endif