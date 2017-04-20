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

#include <stdafx.h>

#include "MetaObjectVisitors.h"

#include <Model/MetaModel/MetaModel.h>

void MetaObjectVisitorAdaptor::visitTag(MetaObject*) 
{
}

void MetaObjectVisitorAdaptor::visitType(CppType *t) 
{ 
  visitTag(t); 
}

void MetaObjectVisitorAdaptor::visitInheritable(Inheritable *h) 
{ 
  visitType(h); 
}

void MetaObjectVisitorAdaptor::visitNamespace(Namespace *n) 
{  
  visitType(n);
}

void MetaObjectVisitorAdaptor::visitClass(Class *c) 
{ 
  visitInheritable(c);
}

void MetaObjectVisitorAdaptor::visitStruct(Struct *s)
{ 
  visitInheritable(s);
}

void MetaObjectVisitorAdaptor::visitUnion(Union *u)
{ 
  visitInheritable(u);
}

void MetaObjectVisitorAdaptor::visitEnum(Enum *e) 
{  
  visitType(e);
}

void MetaObjectVisitorAdaptor::visitTypedef(Typedef *t)
{  
  visitType(t);
}

void MetaObjectVisitorAdaptor::visitField(Field *f)
{ 
  visitTag(f); 
}

void MetaObjectVisitorAdaptor::visitVariable(Variable *v)
{ 
  visitTag(v); 
}

void MetaObjectVisitorAdaptor::visitMethod(Method *m)
{ 
  visitTag(m); 
}

void MetaObjectVisitorAdaptor::visitFunction(Function *f)
{ 
  visitTag(f); 
}

void MetaObjectCloner::visitNamespace(Namespace *n) { setClone(smart_ptr<MetaObject>(new Namespace(*n))); }
void MetaObjectCloner::visitClass(Class *c)         { setClone(smart_ptr<MetaObject>(new Class(*c))); }
void MetaObjectCloner::visitStruct(Struct *s)       { setClone(smart_ptr<MetaObject>(new Struct(*s))); }
void MetaObjectCloner::visitUnion(Union *u)         { setClone(smart_ptr<MetaObject>(new Union(*u))); }
void MetaObjectCloner::visitEnum(Enum *e)           { setClone(smart_ptr<MetaObject>(new Enum(*e))); }
void MetaObjectCloner::visitTypedef(Typedef *t)     { setClone(smart_ptr<MetaObject>(new Typedef(*t))); }
void MetaObjectCloner::visitField(Field *f)         { setClone(smart_ptr<MetaObject>(new Field(*f))); }
void MetaObjectCloner::visitVariable(Variable *v)   { setClone(smart_ptr<MetaObject>(new Variable(*v))); }
void MetaObjectCloner::visitMethod(Method *m)       { setClone(smart_ptr<MetaObject>(new Method(*m))); }
void MetaObjectCloner::visitFunction(Function*f)    { setClone(smart_ptr<MetaObject>(new Function(*f))); }

