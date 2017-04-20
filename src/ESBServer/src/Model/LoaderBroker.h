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

#ifndef LoaderBroker_H
#define LoaderBroker_H

/*#include <Model/TypeInfoLoader.h>
#include <Model/CppTypeLoader.h>
#include <Model/MethodLoader.h>
#include <Model/HierarchyLoader.h>
*/
#include <Model/Workspace.h>

#include <smart_ptr.h>
#include <map>

class TypeInfoLoader;
class CppTypeLoader;
class MethodLoader;

class HierarchyLoader;
class OutlineLoader;

class LoaderBroker
{
private:  
  smart_ptr<OutlineLoader> mMetaObjectLoader;
  //smart_ptr<TypeInfoLoader> mTypeInfoLoader;
  //smart_ptr<CppTypeLoader> mTypeLoader;
  //smart_ptr<MethodLoader> mMethodLoader;
  smart_ptr<HierarchyLoader> mHierarchyLoader;
  

public:
  ~LoaderBroker();

  static const smart_ptr<LoaderBroker>& instance(const smart_ptr<Workspace>&);

  void setOutlineLoader(const smart_ptr<OutlineLoader> &mol)
  { mMetaObjectLoader = mol; }
  
  const smart_ptr<OutlineLoader>& getOutlineLoader() const
  { return mMetaObjectLoader; }
  
//  void setTypeInfoLoader(const smart_ptr<TypeInfoLoader>&);
//  const smart_ptr<TypeInfoLoader>& getTypeInfoLoader() const;
//
//  void setTypeLoader(const smart_ptr<CppTypeLoader>&);
//  const smart_ptr<CppTypeLoader>& getTypeLoader() const;
//
//  void setMethodLoader(const smart_ptr<MethodLoader>&);
//  const smart_ptr<MethodLoader>& getMethodLoader() const;

  void setHierarchyLoader(const smart_ptr<HierarchyLoader> &hl) 
  { mHierarchyLoader = hl; };

  const smart_ptr<HierarchyLoader>& getHierarchyLoader() const 
  { return mHierarchyLoader; };

  

    
private:
  LoaderBroker();
};

#endif