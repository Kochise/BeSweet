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

#include "HierarchyLoaderSimulator.h"

#include <Model/MetaModel/MetaModel.h>

using namespace std;

namespace Simulators
{

Inheritable& HierarchyLoader::loadHierarchy(Inheritable &type, bool)
{
  type.setParents(loadSupertypeHierarchy(type));
  type.setExtendors(loadSubtypeHierarchy(type));

  return type;
}

std::list<smart_ptr<Inheritable> > HierarchyLoader::loadSubtypeHierarchy(const Inheritable&, bool)
{
  TagLocation location;
  std::list<smart_ptr<Inheritable> > extendors; 

 
  smart_ptr<Struct> child1(new Struct("::Child1", ePublic, location));
  child1->addMethod(smart_ptr<Method>(new Method("void", "Child1::publicMethod", ePublic, TagLocation())));
  child1->addMethod(smart_ptr<Method>(new Method("void", "Child1::protectedMethod", eProtected, TagLocation())));
  child1->addMethod(smart_ptr<Method>(new Method("void", "Child1::privateMethod", ePrivate, TagLocation())));

  smart_ptr<Class> child2(new Class("::Child2", ePublic, location));
  
  smart_ptr<Class> child2_1(new Class("Namespace1::Child2_1", ePublic, location));
  smart_ptr<Class> child2_2(new Class("::Child2_2", ePublic, location));
  smart_ptr<Struct> child2_2_1(new Struct("::Child2_2_1", ePublic, location));
  
  smart_ptr<Class> child2_3(new Class("::?Child2_3", ePublic, location));

  extendors.clear();
  extendors.push_back(child2_2_1);
  child2_2->setExtendors(extendors);

  extendors.clear();
  extendors.push_back(child2_1);
  extendors.push_back(child2_2);
  extendors.push_back(child2_3);
  child2->setExtendors(extendors);

  extendors.clear();
  extendors.push_back(child1);
  extendors.push_back(child2);
  return extendors;
}

std::list<smart_ptr<Inheritable> > HierarchyLoader::loadSupertypeHierarchy(const Inheritable &tag, bool)
{
  TagLocation location;
  std::list<smart_ptr<Inheritable> > parents; 
  
  smart_ptr<Struct> parent1(new Struct("::Parent1", ePublic, location));
  smart_ptr<Class> parent1_1(new Class("Namespace1::Parent_1_1", ePublic, location));
  smart_ptr<Class> parent1_2(new Class("::Parent1_2", ePublic, location));
  smart_ptr<Struct> parent1_2_1(new Struct("::Parent1_2_1", ePublic, location));
  
  smart_ptr<Class> parent2(new Class("::Parent2", ePublic, location));


  parents.clear();
  parents.push_back(parent1_2_1);
  parent1_2->setParents(parents);

  parents.clear();
  parents.push_back(parent1_1);
  parents.push_back(parent1_2);
  parent1->setParents(parents);
  
  parents.clear();
  parents.push_back(parent1);
  parents.push_back(parent2);

  return parents;
}

}
