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

#ifndef MetaObjectVisitor_H
#define MetaObjectVisitor_H

#include <Model/MetaModel/MetaModelDefines.h>
#include <Utilities/smart_ptr.h>

class MetaObjectVisitor
{
public:
  virtual ~MetaObjectVisitor(){}
  
  virtual void visitNamespace(Namespace*) = 0;
  
  virtual void visitClass(Class*) = 0;
  virtual void visitStruct(Struct*) = 0;
  virtual void visitUnion(Union*) = 0;
  virtual void visitEnum(Enum*) = 0;
  virtual void visitTypedef(Typedef*) = 0;
  virtual void visitField(Field*) = 0;
  virtual void visitVariable(Variable*) = 0;
  virtual void visitMethod(Method*) = 0;
  virtual void visitFunction(Function*) = 0;
};

class MetaObjectTyper : public MetaObjectVisitor
{
private:
  MetaObjectType mType;
  
public:
  MetaObjectType getType() { return mType; }
  
  virtual void visitNamespace(Namespace *) { setType(eNamespace); }
  virtual void visitClass(Class*) { setType(eClass); }
  virtual void visitStruct(Struct*) { setType(eStruct); }
  virtual void visitUnion(Union*) { setType(eUnion); }
  virtual void visitEnum(Enum*) { setType(eEnum); }
  virtual void visitTypedef(Typedef*) { setType(eTypedef); }
  virtual void visitField(Field *f)  { setType(eField); }
  virtual void visitVariable(Variable*) { setType(eVariable); }
  virtual void visitMethod(Method *m)  { setType(eMethod); }
  virtual void visitFunction(Function*)  { setType(eFunction); }
  
private:
  void setType(MetaObjectType t) { mType = t; }
};

class Inheritable;
class CppType;

class MetaObjectVisitorAdaptor : public MetaObjectVisitor
{
public:
  virtual void visitTag(MetaObject*);
  virtual void visitType(CppType *t);
  virtual void visitInheritable(Inheritable *h);
  
  virtual void visitNamespace(Namespace *);
  virtual void visitClass(Class*);
  virtual void visitStruct(Struct*);
  virtual void visitUnion(Union*);
  virtual void visitEnum(Enum*);
  virtual void visitTypedef(Typedef*);
  virtual void visitField(Field *f);
  virtual void visitVariable(Variable*);
  virtual void visitMethod(Method *m);
  virtual void visitFunction(Function*);
};

class MetaObjectCloner : public MetaObjectVisitor
{
private:
  smart_ptr<MetaObject> mClone;
  
public:
  const smart_ptr<MetaObject>& getClone() { return mClone; }
  
  virtual void visitNamespace(Namespace *n);
  virtual void visitClass(Class *c);
  virtual void visitStruct(Struct *s);
  virtual void visitUnion(Union *u);
  virtual void visitEnum(Enum *e);
  virtual void visitTypedef(Typedef *t);
  virtual void visitField(Field *f);
  virtual void visitVariable(Variable *v);
  virtual void visitMethod(Method *m);
  virtual void visitFunction(Function*f);
  
private:
  void setClone(const smart_ptr<MetaObject>& c) { mClone = c; }
  
};

#endif