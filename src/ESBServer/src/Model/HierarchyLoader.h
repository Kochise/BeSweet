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

#ifndef HierarchyLoader_H
#define HierarchyLoader_H

#include <Utilities/smart_ptr.h>

#include <list>

class Inheritable;
class MetaObject;

class AmbiguityResolver
{
public:
  virtual smart_ptr<MetaObject> resolve(const std::list<smart_ptr<MetaObject> > &) const = 0;
};

class HierarchyLoader
{
public:

public:
  virtual ~HierarchyLoader();

  virtual Inheritable& loadHierarchy(Inheritable&, bool recursive = false) = 0;
  virtual std::list<smart_ptr<Inheritable> > loadSubtypeHierarchy(const Inheritable&, bool recursive = false) = 0;
  virtual std::list<smart_ptr<Inheritable> > loadSupertypeHierarchy(const Inheritable&, bool recursive = false) = 0;

  virtual void setAmbiguityResolver(const smart_ptr<AmbiguityResolver>&) = 0;
};

#endif