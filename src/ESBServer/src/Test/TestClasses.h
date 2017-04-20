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

/*class C{};
struct S{};
enum E{};
union U{};
typedef E T;*/
 class Auto
 {
   void fahre();
   
 };
struct SU
{
  void foo();
};

namespace NS1
{
  class Bar_NS1 : public Foo
  {
  };
}

namespace NS2
{
  class Bar_NS2 : public Foo
  {
  };
}
//class SomeClass
//{
//public:
//  enum SomeEnum { eRunning, eWaiting };
//  
//private:
//  static Foo *mTheInstance;
//
//protected:
//  SomeEnum mProtectedField;
//  SomeClass(const Foo&);
//  virtual ~SomeClass() = 0;
//
//public:
//  static SomeEnum& staticMethod();
//
//  virtual void publicVirtualMethod(const Bar&) const; 
//private:
//  bool privateConstMethod() const;
//};
//
//string GlobalFunction();
/*
class ConfigurationDataConverter
{
public:
  //static void ByteArrayToConfigurationDataEntry (const std::vector<byte>& pxBuffer, DataMember& rxProperty);
  static void ConfigurationDataEntryToByteArray (DataMember& rxProperty, std::vector<byte>& pxBuffer);
  
};*/
//struct SomeStruct
//{
//  int x;
//  int y;
//
//
//private:
//  int reserved;
//};
//
const static std::string globalVariable = "";

/*
class Loader
{
  void load(string);
};
void function(int i)
{
}

class Z
{
  virtual static void foo() const { function(0); };

public:
  class PublicInnerClass{};
protected:
   class ProtectedInnerClass{};
private:
   class PrivateInnerClass{};
};*/

class TestClass
{
  public:
    int publicField;
    static int publicStaticField;
    const int publicConstField;
    static const int publicStaticConstField;

    void publicMethod();
    void publicConstMethod() const;
    static void publicStaticMethod();
    virtual void publicVirtualMethod();
    virtual void publicVirtualConstMethod() const;
    virtual void publicAbstractMethod() = 0;
    virtual void publicAbstractConstMethod() const = 0;

  protected:
    int protectedField;
    static int protectedStaticField;
    const int protectedConstField;
    const static int protectedStaticConstField;
    void protectedMethod();
    void protectedConstMethod() const;
    static void protectedStaticMethod();
    virtual void protectedVirtualMethod();
    virtual void protectedVirtualConstMethod() const;
    virtual void protectedAbstractMethod() = 0;
    virtual void protectedAbstractConstMethod() const = 0;

  private:
    int privateField;
    static int privateStaticField;
    const int privateConstField;
    static const int privateStaticConstField;
    void privateMethod();
    void privateConstMethod() const;
    static void privateStaticMethod();
    virtual void privateVirtualMethod();
    virtual void privateVirtualConstMethod() const;
    virtual void privateAbstractMethod() = 0;
    virtual void privateAbstractConstMethod() const = 0;

  public:
    void a();
    void ba();
    void aa();
};
*/
typedef enum {
            e1, 
            e2 
          } GlobalEnum;

enum GlobalEnum2 { e1, e2  } GlobalEnum2;

typedef TestClass GobalTypedef;

union GlobalUnion
{
};

struct GlobalStruct
{
};
*/
class OuterClass
{
public:
  typedef TestClass InnerTypedef;
  typedef enum {e1, e2 } InnerEnum;
	class InnerClass
	{
		public:
			class InnerInnerClass
			{
        void innerInnerFunction();
        int innerFunction();
			};

      void innerFunction();
	};

  void foo();

  struct InnerStruct
  {
  };

  union InnerUnion
  {
  };

private:
  class PrivateInnerClass
  {
  };

  void outerFunction();
};
/*
void globalFunction(int);

namespace AnotherNamespace
{
  int globalFunction();
}

inline void globalFunction(int)
{
}

inline void globalInlineFunction(int)
{
}


class TestClass2
{

};

void foo();

class TestClass3
{
  void aMethod() RAISES(bla);
  static virtual void aMethod2() RAISES(bla) = 0;
  bool operator ==();

  int aField;
  int aSecondField;
};
*/