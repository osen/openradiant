#ifndef IRON_EXCEPTION_H
#define IRON_EXCEPTION_H

#include "String.h"

#include <string>

namespace iron
{

namespace _
{

template <typename T>
struct Exception
{
  Exception(const String<int>& message)
  {
    this->message = message;
  }

  String<int> what()
  {
    return message;
  }

private:
  String<int> message;

};

}

typedef _::Exception<int> Exception;

}

#endif
