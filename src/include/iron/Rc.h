#ifndef IRON_RC_H
#define IRON_RC_H

#include "Zero.h"
#include "Atomic.h"

#include <stdlib.h>

namespace iron
{

struct Rc
{
  Zero<void*> ptr;
  Zero<void(*)(void*)> deleter;
  Zero<size_t> unique;
  Atomic<size_t> weak;
  Atomic<size_t> lock;
};

}

#endif
