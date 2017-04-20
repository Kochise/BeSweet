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

#ifndef TypeBrowser_H
#define TypeBrowser_H

#include <algorithm>
#include <list>

#include "TypeInfoLoader.h"
#include "ScopedNameBrowser.h"

#include "smart_ptr.h"

class ScopedNameBrowserRenderer;


class  TagbrowserItem : public ScopedNameBrowserItem
{
protected:
  smart_ptr<MetaObject> mTag;

public:
  TagbrowserItem(const smart_ptr<MetaObject>& cppType);

  std::string getNameLabel() const;
  int getNameIcon() const;

  std::string getScopeLabel() const; 
  int getScopeIcon() const;

  const smart_ptr<MetaObject>& getTag() const; 
};



class TypeBrowserModel : public ScopedNameBrowserModel
{
public:
  TypeBrowserModel(const TypeInfoLoader::TypeList &tl);
  
private:
  static smart_ptr<ScopedNameBrowserItem> makeDataItem(const smart_ptr<MetaObject> &type);

};

typedef ScopedNameBrowser TagBrowser;


class MethodBrowserItem : public TagbrowserItem
{
public:
  MethodBrowserItem(const smart_ptr<Function>&);

  int getScopeIcon() const;
  std::string getScopeLabel() const ;
};

class MethodBrowserModel : public ScopedNameBrowserModel
{
public: 
  MethodBrowserModel(const std::list<smart_ptr<Function> > &fl);

private:
  static smart_ptr<ScopedNameBrowserItem> makeDataItem(const smart_ptr<Function> &type);

};

#endif
