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

#include "IconDescriptor.h"

#include <string>

using namespace std;

IconDescriptor::IconDescriptor()
: type(eClass)
{
}

void IconDescriptor::visitNamespace(Namespace *n)
{
  type = eNamespace;
  decorate(*n);
  decorators.hasProblem = false;
  decorators.hasQuestion = false;
}

void IconDescriptor::visitClass(Class *c)
{
  type = eClass;
  decorate(*c);
}

void IconDescriptor::visitStruct(Struct *s)
{
  type = eStruct;
  decorate(*s);
}

void IconDescriptor::visitUnion(Union *u)
{
  type = eUnion;
  decorate(*u);
}

void IconDescriptor::visitEnum(Enum *e)
{
  type = eEnum;
  decorate(*e);
}

void IconDescriptor::visitTypedef(Typedef *t)
{
  type = eTypedef;
  decorate(*t);
}

void IconDescriptor::visitField(Field *f)
{
  type = eField;
  decorators.hasAccessDecorator = true;
  decorate(*f);
}

void IconDescriptor::visitVariable(Variable *v)
{
  type = eVariable;
  decorate(*v);
}

void IconDescriptor::visitMethod(Method *m)
{
  type = eMethod;
  decorators.hasAccessDecorator = true;
  decorate(*m);
}
void IconDescriptor::visitFunction(Function *f)
{
  type = eFunction;
  decorate(*f);
}

void IconDescriptor::decorate(const MetaObject &tag)
{
  decorators.accessDecorator = tag.getAccessQualifier();

  //if(mDecorateWithInner) {
  //  bitmask |= (mParent != TVI_ROOT ? DecoratorBitMask::INNER : 0);
  //}
  if(decorators.accessDecorator != ePublic) {
    decorators.hasAccessDecorator = true;
  }
  
  if(tag.getDeclaration().getFile().empty() && tag.getDefinition().getFile().empty()) {
    decorators.hasProblem = true;
    decorators.hasQuestion = true;
  }
}

void IconDescriptor::decorate(const Method &method)
{
  decorate((MetaObject&)method);
  decorateConstStatic(method);
  decorators.implementationDecorator = method.getImplementationQualifier();
}

void IconDescriptor::decorate(const Variable &variable)
{
  decorate((MetaObject&)variable);
  decorateConstStatic(variable);
}

namespace DecoratorBitMask
{
const size_t PUBLIC  = 0x01;
const size_t PROTECTED = 0x02;
const size_t PRIVATE = 0x04;

const size_t STATIC = 0x08;
const size_t _CONST = 0x10;

const size_t VIRTUAL = 0x20;
const size_t ABSTRACT = 0x40;
const size_t OVERW = 0x80;

const size_t INNER = 0x100;
const size_t PROBLEM = 0x200;
const size_t QUESTIONMARK = 0x400;
const size_t SELECTION_ARROW = 0x800;

const size_t PROTOTYPE = 0x1000;

}

IconDecorators::IconDecorators()
: accessDecorator(ePublic), implementationDecorator(eDefault),
  hasConstDecorator(false), hasStaticDecorator(false), hasAccessDecorator(false),
  hasProblem(false), hasQuestion(false), isSelected(false), isPrototype(false)
{
}

size_t IconDecorators::hash() const
{
  size_t bitmask = 0;
  
  if(hasAccessDecorator) {
    bitmask |= DecoratorBitMask::INNER;

    switch(accessDecorator) {
      case ePublic: bitmask |= DecoratorBitMask::PUBLIC; break;
      case eProtected: bitmask |= DecoratorBitMask::PROTECTED; break;
      case ePrivate: bitmask |= DecoratorBitMask::PRIVATE; break;
    }
  }

  switch(implementationDecorator) {
    case eVirtual: bitmask |= DecoratorBitMask::VIRTUAL; break;
    case eAbstract: bitmask |= DecoratorBitMask::ABSTRACT; break;
  }
    
  if(hasConstDecorator) {
    bitmask |= DecoratorBitMask::_CONST;
  }
    
  if(hasStaticDecorator) {
    bitmask |= DecoratorBitMask::STATIC;
  }
  
  if(hasProblem) {
    bitmask |= DecoratorBitMask::PROBLEM;
  }
  
  if(hasQuestion) {
    bitmask |= DecoratorBitMask::QUESTIONMARK;
  }

  if(isSelected) {
    bitmask |= DecoratorBitMask::SELECTION_ARROW;
  }

  if(isPrototype) {
    bitmask |= DecoratorBitMask::PROTOTYPE;
  }
  
  return bitmask;
}