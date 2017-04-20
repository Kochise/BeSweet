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

#ifndef MetaModelFilter_H
#define MetaModelFilter_H

#include <Model/MetaModel/MetaObject.h>
#include <Model/MetaModel/MetaObjectVisitors.h>
#include <Utilities/smart_ptr.h>

#include <string>
#include <list>

class MetaModelFilter
{
public:
  MetaModelFilter(const std::string id)
	: mIsEnabled(true), mId(id) {}
  
  virtual ~MetaModelFilter();

  virtual std::list<smart_ptr<MetaObject> > apply(const std::list<smart_ptr<MetaObject> > &) = 0;
  
  std::string id() 
  { return mId; }
  
  void next(const smart_ptr<MetaModelFilter> &n);
   
  const smart_ptr<MetaModelFilter>& next() const 
  { return mNext; }

  bool isEnabled() const 
  { return mIsEnabled; }
  
  void enable(bool e) 
  { mIsEnabled = e; }

private:
  std::string mId;
  bool mIsEnabled;
  smart_ptr<MetaModelFilter> mNext;

};

class MemberSorter : public MetaModelFilter
{
public:
  enum SortOrder { eNoSort, eByName, eByAccess};

public:
  MemberSorter(const std::string id)
	: MetaModelFilter(id) {}

  virtual ~MemberSorter();

  virtual std::list<smart_ptr<MetaObject> > apply(const std::list<smart_ptr<MetaObject> > &);
  
  void setOrder(SortOrder order)
  { mSortOrder = order; }
  
  SortOrder getOrder() const
  { return mSortOrder; }

private:
  std::list<smart_ptr<MetaObject> > selectTags(const std::list<smart_ptr<MetaObject> >&, size_t type) const;

private:
  SortOrder mSortOrder;

};


class MemberFilter : public MetaModelFilter, public MetaObjectVisitorAdaptor
{
public:
  struct Filter
  {
    Filter() : access(0), other(0) {}
    
    std::string regex;
    size_t access;
    size_t other;
  };


public:
  MemberFilter(const std::string id)
	: MetaModelFilter(id) {}
	
  virtual ~MemberFilter();

  virtual std::list<smart_ptr<MetaObject> > apply(const std::list<smart_ptr<MetaObject> > &);

  bool isFiltered(const CppType&);
  bool isFiltered(const Field &);
  bool isFiltered(const Method&);

  void setTypeFilter(const Filter &f) {
    mTypeFilter = f;
  }
  
  const Filter& getTypeFilter() const {
    return mTypeFilter;
  }

  void setMethodFilter(const Filter &f) {
    mMethodFilter = f;
  }
  
  const Filter& getMethodFilter() const {
    return mMethodFilter;
  }

  void setFieldFilter(const Filter &f) {
     mFieldFilter = f;
  }

  const Filter& getFieldFilter() const {
    return mFieldFilter;
  }

private:
  Filter mTypeFilter;
  Filter mFieldFilter;
  Filter mMethodFilter;
 

  bool mIsFiltered;
  bool mChildrenVisible;
  bool isFiltered() const { return mIsFiltered; }

  virtual void visitType(CppType*);
  virtual void visitNamespace(Namespace*);
  virtual void visitField(Field*);
  virtual void visitMethod(Method*);

};

#endif