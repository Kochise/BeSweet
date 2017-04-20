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

#ifndef MetaModelDefines_H
#define MetaModelDefines_H

enum AccessQualifier
{
  ePublic = 0x01, 
  eProtected = 0x02, 
  ePrivate = 0x04

};

enum ImplementationQualifier
{
  eDefault, 
  eVirtual = 0x01, 
  eAbstract = 0x02

};

const size_t eStatic = 0x04;
const size_t eConst = 0x08;

enum MetaObjectType
{
  eNamespace = 0x0001, 
  eClass = 0x0002, 
  eStruct = 0x0004, 
  eUnion = 0x0008, 
  eEnum = 0x0010, 
  eTypedef = 0x0020,
  eEnumMember = 0x0040,
  eField = 0x0080, 
  eVariable = 0x0100,
  eMethod = 0x0200, 
  eFunction = 0x0400
        
};


class MetaObjectVisitor;
class MetaObject;


class Namespace;
class Class;
class Struct;
class Union;
class Enum;
class Typedef;
class Field;
class Method;
class Variable;
class Function;

#endif