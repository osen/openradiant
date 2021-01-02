#ifndef IRON_ATOMIC_H
#define IRON_ATOMIC_H

#include "Zero.h"
#include "panic.h"

//#include <pthread.h>
#include <atomic>
//#include <stdatomic.h>

namespace iron
{

template <typename T>
struct Atomic
{
  /*************************************************************
   * Atomic<int> foo;
   *************************************************************/
  Atomic() : t() { }

  /*************************************************************
   * struct Foo
   * {
   *   Atomic<int> bar;
   * };
   *
   * Foo f;
   * Foo f2(f);
   *************************************************************/
  Atomic(const Atomic& other) : t(other.t) { }

  /*************************************************************
   * Atomic<int> foo;
   * Atomic<int> bar;
   * foo = bar;
   *************************************************************/
  Atomic& operator=(const Atomic& other)
  {
    t = other.t;

    return *this;
  }

  T operator++()
  {
    return ++t;
  }

  T operator++(int)
  {
    return t++;
  }

  T operator--()
  {
    return --t;
  }

  T operator--(int)
  {
    return t--;
  }

  /*************************************************************
   * Atomic<size_t> foo;
   * if(foo) { }
   *************************************************************/
  operator bool() const
  {
    return t;
  }

/*
  bool operator==(const T& value) const
  {
    //pthread_mutex_lock(&lock);
    T tc = t;
    //pthread_mutex_unlock(&lock);

    if(tc == value) return true;

    return false;
  }
*/

private:
  //mutable Zero<T> t;
  //std::atomic<T> t;
  //mutable _Atomic T t;
  //mutable _Atomic(T) t;
  mutable std::atomic<T> t;
  //mutable pthread_mutex_t lock;

};

}

#endif
