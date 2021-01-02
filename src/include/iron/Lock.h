#ifndef IRON_LOCK_H
#define IRON_LOCK_H

#include "Zero.h"
#include "Rc.h"

namespace iron
{

template <typename T>
struct Lock
{
  /***************************
   * Otherwise structs containing this cannot get a default constructor
   ***************************/
  Lock() { }

  Lock(Rc* rc) : rc(rc)
  {
    rc->lock++;
  }

  Lock(const Lock& other) : rc(other.rc)
  {
    rc->lock++;
  }

  ~Lock()
  {
    if(rc) rc->lock--;
  }

  Lock& operator=(const Lock& other)
  {
    if(this == &other) return *this;
    if(rc == other.rc) return *this;
    if(rc) rc->lock--;
    rc = other.rc;
    if(rc) rc->lock++;

    return *this;
  }

  T* operator->()
  {
    // TODO: Zero<T> cannot seem to expose internal void* to static_cast
    // directly.
    return static_cast<T*>(static_cast<void*>(rc->ptr));
  }

  operator T&() const
  {
    return *static_cast<T*>(static_cast<void*>(rc->ptr));
  }

private:
  Zero<Rc*> rc;

};

}

#endif
