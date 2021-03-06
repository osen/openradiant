#ifndef IRON_ZERO_H
#define IRON_ZERO_H

namespace iron
{

template <typename T>
struct Zero
{
  /*************************************************************
   * Zero<int> foo;
   *************************************************************/
  Zero() : t() { }

  /*************************************************************
   * struct Foo
   * {
   *   Zero<int> bar;
   * };
   *
   * Foo f;
   * Foo f2(f);
   *************************************************************/
  Zero(const Zero& other) : t(other.t) { }

  /*************************************************************
   * Zero<int> foo;
   * foo = 4;
   *************************************************************/
  Zero(const T& t) : t(t) { }

  /*************************************************************
   * Zero<int> foo;
   * int f = foo;
   *
   * Zero<int> foo;
   * foo--;
   *
   * const Zero<int> foo;
   * int f = foo;
   *************************************************************/
  operator T&() const
  {
    return t;
  }

  /*************************************************************
   * Zero<Foo*> foo;
   * foo->bar();
   *************************************************************/
  T& operator->() const
  {
    return t;
  }

private:
  mutable T t;

};

}

#endif
