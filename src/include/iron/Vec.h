#ifndef IRON_VEC_H
#define IRON_VEC_H

#include "Zero.h"
#include "panic.h"

#include <new>

namespace iron
{

template <typename T>
struct Vec
{
  struct Iter
  {
    bool operator==(const Iter& other) const
    {
      return t == other.t;
    }

    bool operator!=(const Iter& other) const
    {
      return t != other.t;
    }

    Iter operator++(int)
    {
      Iter rtn = *this;
      t++;
      return rtn;
    }

    Iter operator+=(size_t val)
    {
      Iter rtn = *this;
      t += val;
      return rtn;
    }

    T* operator->() const
    {
      return t;
    }

    T& operator*() const
    {
      return *t;
    }

    Iter operator+(size_t val) const
    {
      Iter rtn = *this;
      rtn.t += val;
      return rtn;
    }

  private:
    friend struct Vec;

    Zero<T*> t;
  };

  Vec() { }

  /*************************************************************
   * Vec<Foo> foo;
   * Vec<Foo> bar(foo);
   * Vec<Foo> zip = foo;
   *************************************************************/
  Vec(const Vec& other)
  {
    reserve(other._size);

    for(size_t i = 0; i < other._size; i++)
    {
      new(raw + i) T(other.raw[i]);
    }

    _size = other._size;
  }

  ~Vec()
  {
    for(size_t i = 0; i < _size; i++)
    {
      raw[i].~T();
    }

    if(raw) free(raw);
  }

  /*************************************************************
   * Vec<Foo> foo;
   * Vec<Foo> bar
   * bar = foo;
   *************************************************************/
  Vec& operator=(const Vec& other)
  {
    if(&other == this) return *this;

    clear();
    reserve(other._size);

    for(size_t i = 0; i < other._size; i++)
    {
      new(raw + i) T(other.raw[i]);
    }

    _size = other._size;

    return *this;
  }

  T& operator[](size_t index) const
  {
    if(index >= _size)
    {
      panic("Array access out of bounds");
    }

    return raw[index];
  }

  void push(const T& elem)
  {
    reserve(_size + 1);
    new(raw + _size) T(elem);
    _size++;
  }

  void clear()
  {
    for(size_t i = 0; i < _size; i++)
    {
      raw[i].~T();
    }

    _size = 0;
  }

  size_t size() const
  {
    return _size;
  }

  Iter begin() const
  {
    Iter rtn;

    rtn.t = raw;

    return rtn;
  }

  Iter end() const
  {
    Iter rtn;

    rtn.t = raw + _size;

    return rtn;
  }

  Iter erase(Iter& it)
  {
    Iter rtn = it;
    Iter c = it;
    Iter n = c + 1;
    Iter e = end();

    while(n != e)
    {
      *c = *n;
      c = n;
      n++;
    }

    raw[_size - 1].~T();
    _size--;

    return rtn;
  }

  void resize(size_t size)
  {
    if(size < _size)
    {
      for(size_t i = size - 1; i < _size; i++)
      {
        raw[i].~T();
      }

      _size = size;
    }
    else if(size > _size)
    {
      reserve(size);

      for(size_t i = _size - 1; i < size; i++)
      {
        new(raw + i) T();
      }

      _size = size;
    }
  }

private:
  Zero<T*> raw;
  Zero<size_t> _size;
  Zero<size_t> reserved;

  void reserve(size_t amount)
  {
    if(reserved >= amount) return;

    size_t n = 1;

    while(n < amount)
    {
      n *= 2;
    }

    amount = n;
    T* arr = (T*)calloc(amount, sizeof(T));

    for(size_t i = 0; i < _size; i++)
    {
      new(arr + i) T(raw[i]);
    }

    for(size_t i = 0; i < _size; i++)
    {
      raw[i].~T();
    }

    if(raw) free(raw);
    raw = arr;
    reserved = amount;
  }

};

}

#endif
