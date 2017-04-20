
namespace OuterNS
{
  namespace InnerNS
  {
    class InnerClass
    {
    };    
  }
}

namespace OuterNS { namespace InnerNS { namespace InnerstNS {
  class InnerstClass : public OuterNS::InnerNS::InnerClass 
  {
    int foo();
  };
} } }

class NoNSClass : public OuterNS::InnerNS::InnerClass
{
};