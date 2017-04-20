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

#ifndef smart_ptr_H
#define smart_ptr_H

#pragma warning( disable: 4786 )
#pragma warning( disable: 4284 )

#include "Stacktrace.h"

#include <string>
#include <typeinfo>
#include <exception>
#include <stdexcept>
#include <memory>

class NullPointerException : public TraceableException 
{
public:
  NullPointerException(const std::string &type) : TraceableException("NullPointerException: " + type) {};
};

class smart_ptr_dynamic_cast_tag {};

class Disposable
{
public:
  virtual ~Disposable(){};
  virtual void dispose() = 0;
};

template<class T>
class Deletor : public Disposable
{
private:
  T * mPointee;

public:
  Deletor(T *pointee) : mPointee(pointee) {}
  ~Deletor() {};

  void dispose() 
  { delete mPointee; mPointee = 0; }

private:
  Deletor();
  Deletor(const Deletor&);
  Deletor& operator=(const Deletor&);

};


class ReferenceCounter
{
  private:
    unsigned long* pmCounter;
    Disposable *mDtor;

    void IncrementReference()
    {
      ++(*pmCounter);
    }

    void DecrementReference()
    {
      if(--(*pmCounter) == 0)
      {
        delete pmCounter; 
        pmCounter = 0;
        mDtor->dispose();
        delete mDtor;
        mDtor = 0;
      }
    }


  public:
    explicit ReferenceCounter(std::auto_ptr<Disposable> dtor ) : mDtor(dtor.release()), pmCounter( new unsigned long(1)) {}
    ReferenceCounter( const ReferenceCounter& rhs ): mDtor(rhs.mDtor), pmCounter(rhs.pmCounter)
    {
      IncrementReference(); 
    }
    
    ~ReferenceCounter() 
    {
      DecrementReference();
    } 

    ReferenceCounter& operator=( const ReferenceCounter& rhs )
    {
      if( this != &rhs )
      {
        DecrementReference();
        pmCounter = rhs.pmCounter;
        mDtor = rhs.mDtor;
        IncrementReference();
      }
      return *this;
    }
};
  
template<class T>
class smart_ptr
{
private:
	ReferenceCounter mRefCounter;
	T* mPointee;

public:
  typedef T pointee_type;

	smart_ptr( int i = 0 );
	explicit smart_ptr(T* const pxOriginalObject);
 
  template <class U>
	smart_ptr(const smart_ptr<U>& rhs, smart_ptr_dynamic_cast_tag ) 
  : mPointee(0), mRefCounter(std::auto_ptr<Disposable>(new Deletor<T>(0)))
  {
     smart_ptr_dynamic_cast_assigner(*this, rhs);
  }

  template <class U>
  smart_ptr(const smart_ptr<U>& rhs)
  : mPointee(0), mRefCounter(std::auto_ptr<Disposable>(new Deletor<T>(0)))
  {
    invalid_cast((U*)NULL);
    smart_ptr_assigner(*this, rhs); 
  }


	~smart_ptr();

  template<class U>
	smart_ptr<T>& operator=(const smart_ptr<U> & rhs)
  {
    invalid_cast((U*)NULL);
    smart_ptr_assigner(*this, rhs); 
    return *this;
  }

	T& operator*() const;
	T* operator->() const;

  bool operator==(const smart_ptr<T> &rhs) const;
  bool operator!=(const smart_ptr<T> &rhs) const;

  T* get() const;
  bool isNull() const;
  bool isEqual(const smart_ptr<T>& ) const;

private:
  void RemoveReference();
  void ThrowIfNull() const;
  void invalid_cast(T*){};

  friend struct smart_ptr_dynamic_cast_assigner;
  friend struct smart_ptr_assigner;

};


/////////////////////////////////////////////////////////
template <class T>
smart_ptr<T>::smart_ptr(int NotZero) :
  mPointee(0),mRefCounter(std::auto_ptr<Disposable>(new Deletor<T>(0)))
{
  //sizeof(T);
  if(NotZero)
  {
    throw std::logic_error( "smart_ptr constructed with int value" );
  }
} 

template<class T>
smart_ptr<T>::smart_ptr(T* const pointee):
  mPointee( pointee ), mRefCounter(std::auto_ptr<Disposable>(new Deletor<T>(pointee)))
{
  sizeof(T);
}

template<class T>
smart_ptr<T>::~smart_ptr()
{
}


template<class T>
T&  smart_ptr<T>::operator*() const
{
  return *get();
}

template<class T>
T* smart_ptr<T>::operator->() const
{
  return get();
}

template<class T>
bool smart_ptr<T>::operator==(const smart_ptr<T> &rhs) const
{
  return isEqual(rhs);
}

template<class T>
bool smart_ptr<T>::operator!=(const smart_ptr<T>& rhs) const
{
  return !isEqual(rhs);
}


template<class T>
T* smart_ptr<T>::get() const
{
  ThrowIfNull();
  return mPointee;
}

template<class T>
bool smart_ptr<T>::isEqual(const smart_ptr<T> &rxComparisonSmartPtr) const
{
  return ( mPointee == rxComparisonSmartPtr.mPointee );
}

template<class T>
bool smart_ptr<T>::isNull()  const
{
  return mPointee == 0;
}

template<class T>
void smart_ptr<T>::ThrowIfNull() const
{
  if(isNull()) {
    throw NullPointerException(typeid(*this).name());
  }
}


struct smart_ptr_assigner {
  template <class T, class U>
  smart_ptr_assigner( smart_ptr<T> & lhs, const smart_ptr<U>& rhs )
  {
    std::cout << "assign" << std::endl;
    if( (void*)&lhs != (void*)&rhs ) 
    {
      std::cout << "assign" << std::endl;
      lhs.mPointee = rhs.mPointee;
      lhs.mRefCounter = rhs.mRefCounter;
    }
  }
};

struct smart_ptr_dynamic_cast_assigner {
  template <class LHS, class RHS>
  smart_ptr_dynamic_cast_assigner( smart_ptr<LHS> &lhs, const smart_ptr<RHS> &rhs)
  {
    lhs.mPointee = dynamic_cast<LHS*>( rhs.mPointee );
    if( lhs.mPointee != 0 )
    {
      lhs.mRefCounter = rhs.mRefCounter;
    }
  }
};

template <class LHS, class RHS>
smart_ptr<LHS> dynamic__cast( const smart_ptr<RHS>& rhs )
{
  //return smart_ptr<LHS>( rhs, smart_ptr_dynamic_cast_tag());
  smart_ptr<LHS> lhs;
  smart_ptr_dynamic_cast_assigner(lhs, rhs);
  return lhs;
};

template<class T>
smart_ptr<T> make_smart_ptr(const T &t)
{
  return smart_ptr<T>(new T(t));
}

template<class T>
smart_ptr<T> make_smart_ptr(T *t)
{
  return smart_ptr<T>(t);
}

struct is_null
{
  template<class T>
  bool operator()(const smart_ptr<T> &sp) const
  { return sp.isNull(); }
};

namespace boost
{
  template<class T> T* get_pointer(const smart_ptr<T>  &p)
  {
    return p.get();
  }
}

#endif 
