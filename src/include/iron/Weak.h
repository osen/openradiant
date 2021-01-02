#include "Zero.h"
#include "Rc.h"
#include "panic.h"

#include <iostream>

namespace iron
{

template <typename T>
struct Unique;

template <typename T>
struct Weak
{
  /*************************************************************
   * Weak<Foo> foo;
   *************************************************************/
  Weak() { }

  /*************************************************************
   * Weak<Foo> foo;
   * Weak<Foo> bar(foo);
   * Weak<Foo> zip = bar;
   *************************************************************/
  Weak(const Weak& other)
  {
    reset(other.rc);
  }

  /*************************************************************
   * Unique<Foo> foo;
   * Weak<Foo> bar(foo);
   * Weak<Foo> zip = foo;
   *************************************************************/
  Weak(const Unique<T>& other)
  {
    reset(other.rc);
  }

  /*************************************************************
   * Unique<TestFoo> tf;
   * Weak<Foo> f(tf);
   * Weak<Foo> f2 = tf;
   *************************************************************/
  template <typename U>
  Weak(const Unique<U>& other)
  {
    T* t = static_cast<U*>(NULL);
    reset(other.rc);
  }

  ~Weak()
  {
    reset();
  }

  /*************************************************************
   * Weak<Foo> foo;
   * Weak<Foo> bar;
   * bar = foo;
   *************************************************************/
  Weak& operator=(const Weak& other)
  {
    if(&other == this) return *this;
    reset(other.rc);

    return *this;
  }

  /*************************************************************
   * Unique<Foo> foo;
   * Weak<Foo> bar;
   * bar = foo;
   *************************************************************/
  Weak& operator=(const Unique<T>& other)
  {
    reset(other.rc);

    return *this;
  }

  /*************************************************************
   * Weak<Foo> f;
   * Weak<DerivedFoo> df;
   * df = f.cast<DerivedFoo>();
   *************************************************************/
  template <typename U>
  Weak<U> cast()
  {
    U* u = dynamic_cast<U*>(static_cast<T*>(static_cast<void*>(rc->ptr)));
    if(!u) return Weak<U>();

    Weak<U> rtn;
    rtn.reset(rc);

    return rtn;
  }

  /*************************************************************
   * Weak<Foo> foo;
   * Weak<Foo> bar;
   * if(foo == bar) { }
   *************************************************************/
  bool operator==(const Weak& other) const
  {
    return rc == other.rc;
  }

  /*************************************************************
   * Weak<Foo> foo;
   * if(foo) { }
   *************************************************************/
  operator bool() const
  {
    if(!rc) return false;
    if(!rc->unique) return false;

    return true;
  }

  /*************************************************************
   * Weak<Foo> f;
   * f->bar();
   *************************************************************/
  Lock<T> operator->() const
  {
    // TODO: Zero<T> cannot seem to expose internal void* to static_cast
    // directly.
    //return static_cast<T*>(static_cast<void*>(rc->ptr));
    Lock<T> rtn(rc);

    return rtn;
  }

  /*************************************************************
   * void foo(Bar& bar);
   * Weak<Bar> bar;
   * foo(*bar);
   *************************************************************/
  Lock<T> operator*() const
  {
    Lock<T> rtn(rc);

    return rtn;
  }

  /*************************************************************
   * TODO
   *************************************************************/
  Lock<T> lock() const
  {
    return Lock<T>(rc);
  }

  mutable Zero<Rc*> rc;

private:
  template <typename U>
  friend struct Unique;

  template <typename U>
  friend struct Weak;

  void reset(Rc* otherRc = NULL)
  {
    if(rc == otherRc) return;

    if(rc)
    {
      rc->weak--;

      // Delete if no other Weak references and the original Unique
      // has also been destroyed.
      if(!rc->weak && !rc->unique)
      {
        delete rc;
      }
    }

    rc = otherRc;
    if(rc == NULL) return;
    rc->weak++;
  }

};

}
