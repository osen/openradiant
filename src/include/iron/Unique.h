#ifndef IRON_UNIQUE_H
#define IRON_UNIQUE_H

#include "Rc.h"
#include "Zero.h"
#include "panic.h"
#include "Lock.h"

#include <utility>

namespace iron
{

template <typename T>
struct Weak;

template <typename T>
struct Unique
{
  template <typename U>
  friend struct Weak;

  template <typename U>
  friend struct Unique;

  /*************************************************************
   * Unique<Foo>::create();
   * Unique<Foo>::create(1);
   * Unique<Foo>::create(1, "foo");
   *************************************************************/
  template <typename... U>
  static Unique create(U&&... u)
  {
    Unique rtn;
    rtn.rc = new Rc();
    rtn.rc->ptr = new T(std::forward<U>(u)...);
    rtn.rc->deleter = deleter;
    rtn.rc->unique = 1;

    return rtn;
  }

  /*************************************************************
   * Unique<Foo> f;
   *************************************************************/
  Unique() { }

  /*************************************************************
   * Unique<Foo> f;
   * Unique<Foo> f2(f);
   * Unique<Foo> f3 = f;
   *************************************************************/
  Unique(const Unique& other)
  {
    reset(other.rc);
    other.release();
  }

  /*************************************************************
   * Unique<TestFoo> tf;
   * Unique<Foo> f(tf);
   * Unique<Foo> f2 = tf;
   *************************************************************/
  template <typename U>
  Unique(const Unique<U>& other)
  {
    T* t = static_cast<U*>(NULL);
    reset(other.rc);
    other.release();
  }

  ~Unique()
  {
    reset();
  }

  /*************************************************************
   * Unique<Foo> f;
   * Unique<Foo> f2;
   * f2 = f;
   *************************************************************/
  Unique& operator=(const Unique& other)
  {
    if(&other == this) return *this;

    // Reset could end up destroying other before we take
    // ownership. So take this first. This is a little
    // unnecessary because a panic would have occurred anyway
    // due to the strict access locking system in debug mode.

    Rc* otherRc = other.rc;
    other.release();
    reset(otherRc);

    return *this;
  }

  /*************************************************************
   * Unique<Foo> f;
   * f->bar();
   *************************************************************/
  Lock<T> operator->()
  {
    // TODO: Zero<T> cannot seem to expose internal void* to static_cast
    // directly.
    //return static_cast<T*>(static_cast<void*>(rc->ptr));
    Lock<T> rtn(rc);

    return rtn;
  }

  /*************************************************************
   * Unique<Foo> foo;
   * Weak<Foo> bar;
   * if(foo == bar) { }
   *************************************************************/
  template <typename U>
  bool operator==(const Weak<U>& other) const
  {
    return rc == other.rc;
  }

  /*************************************************************
   * Unique<Foo> foo;
   * if(foo) { }
   *************************************************************/
  operator bool() const
  {
    if(!rc) return false;
    if(!rc->unique) return false;

    return true;
  }

  /*************************************************************
   * void foo(Bar& bar);
   * Unique<Bar> bar;
   * foo(*bar);
   *************************************************************/
  Lock<T> operator*() const
  {
    Lock<T> rtn(rc);

    return rtn;
  }

  /*************************************************************
   * Unique<Foo> f;
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

  mutable Zero<Rc*> rc;

private:
  static void deleter(void* ptr)
  {
    T* t = static_cast<T*>(ptr);
    delete t;
  }

  //mutable Zero<Rc*> rc;

  void reset(Rc* otherRc = NULL)
  {
    if(otherRc == rc) return;

    if(rc)
    {
      if(rc->unique != 1)
      {
        // TODO: Should always be 1
        panic("Unique has invalid reference count");
      }

      if(rc->lock)
      {
        panic("Attempt to delete memory whilst in use");
      }

      if(rc->deleter)
      {
        // The deleter might end up deleting the new Rc so lock it.
        // See if release() can be called on previous owner to avoid this.
        if(otherRc) otherRc->lock++;
        rc->deleter(rc->ptr);
        rc->deleter = NULL;
        if(otherRc) otherRc->lock--;
      }

      rc->ptr = NULL;
      rc->unique = 0;

      // Delete if a weak pointer no longer needs Rc information.
      if(!rc->weak)
      {
        delete rc;
      }
    }

    rc = otherRc;
  }

  void release() const
  {
    rc = NULL;
  }

};

}

#endif
