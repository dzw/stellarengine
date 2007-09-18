//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Invocation.h - Objectified method/function call     //
// ### # #      ###                                                      //
// # ### #      ###  Stores an actual method/function call including     //
// #  ## #   # ## ## arguments as an object                              //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_INVOCATION_H
#define NUCLEX_SUPPORT_INVOCATION_H

#include "Nuclex/Nuclex.h"

// Usage
//
// Example 1: Call a simple function
//   Invocation MyTest = MakeInvocation(printf, "hello world");
//   MyTest();
//
// Example 3: Call a functor
//   Invocation MyTest = MakeFunctorInvocation(SomeFunctor);
//
// Example 2: Call a method on an object
//   Invocation MyTest = MakeObjectInvocation(cout, ostream::operator <<, "hello world");
//   MyTest();
//

namespace Nuclex { namespace Support {

class Invocation;

//  //
//  Nuclex::Support::InvocationImplBase                                  //
//  //
/// Base class for all invocations
struct InvocationImplBase {
  /// Constructor
  InvocationImplBase() : m_refs(0) {}
  /// Executes the invocation
  virtual void operator()() = 0;
  /// Number of references to the invocation
  std::size_t m_refs;
};

//  //
//  Nuclex::Support::Invocation                                          //
//  //
/// Objectified function/method/functor call
class Invocation {
  /// Call holder
  /** Stores the reference counted call implementation
  */
  struct Holder {
    /// Constructor
    Holder(InvocationImplBase *impl) : m_impl(impl) { ++m_impl->m_refs; }
    /// Copy constructor with addref
    Holder(const Holder &other) : m_impl(other.m_impl) { ++m_impl->m_refs; }
    /// Destructor with decref
    ~Holder() {
      if(!--m_impl->m_refs)
        delete m_impl;
    }
    /// Assignment
    void operator =(const Holder &other) {
      ++other.m_impl->m_refs;
      if(!--m_impl->m_refs)
        delete m_impl;
      m_impl = other.m_impl;
    }
    /// Execute invocation
    void operator()() { m_impl->operator()(); }
    
    private:
      InvocationImplBase *m_impl;                     ///< The actual invocation
  };

  public:
    /// Constructor
    template<typename T> Invocation(const T &t) : m_holder(new T(t)) {}

    /// Execute invocation
    void operator()() { m_holder(); }
    
    // Generated copy-ctor, assignment and dtor suffice
  
  private:
    Holder m_holder;                                  ///< Invocation holder
};

//  //
//  Nuclex::Support::InvocationImpl0                                     //
//  //
/// Invocation of a function without arguments
template<typename F>
struct InvocationImpl0 : public InvocationImplBase {
  InvocationImpl0(F fn) : m_fn(fn) {}
  void operator()() { m_fn(); }
  private:
    F m_fn;
};
/// Builds an Invocation of a function without arguments
template<typename F>
inline InvocationImpl0<F> MakeInvocation(F fn) {
  return InvocationImpl0<F>(fn);
}

//  //
//  Nuclex::Support::InvocationImpl1                                     //
//  //
/// Invocation of a function with 1 argument
template<typename F, typename P1>
struct InvocationImpl1 : public InvocationImplBase {
  InvocationImpl1(F fn, P1 p1) : m_fn(fn), m_p1(p1) {}
  void operator()() { m_fn(m_p1); }
  private:
    F  m_fn;
    P1 m_p1;
};
/// Builds an Invocation of a function with 1 argument
template<typename F, typename P1>
inline InvocationImpl1<F, P1> MakeInvocation(F fn, P1 p1) {
  return InvocationImpl1<F, P1>(fn, p1);
}

//  //
//  Nuclex::Support::InvocationImpl2                                     //
//  //
/// Invocation of a function with 2 arguments
template<typename F, typename P1, typename P2>
struct InvocationImpl2 : public InvocationImplBase {
  InvocationImpl2(F fn, P1 p1, P2 p2) : m_fn(fn), m_p1(p1), m_p2(p2) {}
  void operator()() { m_fn(m_p1, m_p2); }
  private:
    F  m_fn;
    P1 m_p1;
    P2 m_p2;
};
/// Builds an Invocation of a function with 2 arguments
template<typename F, typename P1, typename P2>
inline InvocationImpl2<F, P1, P2> MakeInvocation(F fn, P1 p1, P2 p2) {
  return InvocationImpl2<F, P1, P2>(fn, p1, p1);
}

//  //
//  Nuclex::Support::ObjectInvocationImpl0                               //
//  //
/// Invocation of a method with no arguments
template<class O, typename M>
struct ObjectInvocationImpl0 : public InvocationImplBase {
  ObjectInvocationImpl0(O &o, M mt) : m_o(o), m_mt(mt) {}
  void operator()() { (m_o.*m_mt)(); }
  private:
    O m_o;
    M m_mt;
};
/// Builds an Invocation of a method with no arguments
template<class O, typename M>
inline ObjectInvocationImpl0<O, M> MakeObjectInvocation(O &o, M mt) {
  return ObjectInvocationImpl0<O, M>(o, mt);
}

//  //
//  Nuclex::Support::ObjectInvocationImpl1                               //
//  //
/// Invocation of a method with 1 argument
template<class O, typename M, typename P1>
struct ObjectInvocationImpl1 : public InvocationImplBase {
  ObjectInvocationImpl1(O &o, M mt, P1 p1) : m_o(o), m_mt(mt), m_p1(p1) {}
  void operator()() { (m_o.*m_mt)(m_p1); }
  private:
    O  m_o;
    M  m_mt;
    P1 m_p1;
};
/// Builds an Invocation of a method with 1 argument
template<class O, typename M, typename P1>
inline ObjectInvocationImpl1<O, M, P1> MakeObjectInvocation(O &o, M mt, P1 p1) {
  return ObjectInvocationImpl1<O, M, P1>(o, mt, p1);
}

//  //
//  Nuclex::Support::ObjectInvocationImpl2                               //
//  //
/// Invocation of a method with 2 arguments
template<class O, typename M, typename P1, typename P2>
struct ObjectInvocationImpl2 : public InvocationImplBase {
  ObjectInvocationImpl2(O &o, M mt, P1 p1, P2 p2) : m_o(o), m_mt(mt), m_p1(p1), m_p2(p2) {}
  void operator()() { (m_o.*m_mt)(m_p1, m_p2); }
  private:
    O  m_o;
    M  m_mt;
    P1 m_p1;
    P2 m_p2;
};
/// Builds an Invocation of a method with 2 arguments
template<class O, typename M, typename P1, typename P2>
inline ObjectInvocationImpl2<O, M, P1, P2> MakeObjectInvocation(O &o, M mt, P1 p1, P2 p2) {
  return ObjectInvocationImpl2<O, M, P1, P2>(o, mt, p1, p2);
}

//  //
//  Nuclex::Support::FunctorInvocationImpl0                              //
//  //
/// Invocation of a functor
template<class O>
struct FunctorInvocationImpl0 : public InvocationImplBase {
  FunctorInvocationImpl0(O &o) : m_o(o) {}
  void operator()() { m_o(); }
  private:
    O m_o;
};
/// Builds an Invocation of a functor
template<class O>
inline FunctorInvocationImpl0<O> MakeFunctorInvocation(O &o) {
  return FunctorInvocationImpl0<O>(o);
}

//  //
//  Nuclex::Support::FunctorInvocationImpl1                              //
//  //
/// Invocation of a functor
template<class O, typename P1>
struct FunctorInvocationImpl1 : public InvocationImplBase {
  FunctorInvocationImpl1(O &o, P1 p1) : m_o(o), m_p1(p1) {}
  void operator()() { m_o(m_p1); }
  private:
    O  m_o;
    P1 m_p1;
};
/// Builds an Invocation of a functor
template<class O, typename P1>
inline FunctorInvocationImpl1<O, P1> MakeFunctorInvocation(O &o, P1 p1) {
  return FunctorInvocationImpl1<O, P1>(o, p1);
}

//  //
//  Nuclex::Support::FunctorInvocationImpl2                              //
//  //
/// Invocation of a functor
template<class O, typename P1, typename P2>
struct FunctorInvocationImpl2 : public InvocationImplBase {
  FunctorInvocationImpl2(O &o, P1 p1, P2 p2) : m_o(o), m_p1(p1), m_p2(p2) {}
  void operator()() { m_o(m_p1, m_p2); }
  private:
    O  m_o;
    P1 m_p1;
    P2 m_p2;
};
/// Builds an Invocation of a functor
template<class O, typename P1, typename P2>
inline FunctorInvocationImpl2<O, P1, P2> MakeFunctorInvocation(O &o, P1 p1, P2 p2) {
  return FunctorInvocationImpl2<O, P1, P2>(o, p1, p2);
}

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_INVOCATION_H
