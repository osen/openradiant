#ifndef IRON_EXCEPTION_H
#define IRON_EXCEPTION_H

#include <string>

namespace iron
{

namespace _
{

template <typename T>
struct Exception
{
  Exception(std::string message)
  {
    this->message = message;
  }

  std::string what()
  {
    return message;
  }

private:
  std::string message;

};

}

typedef _::Exception<int> Exception;

}

#endif
