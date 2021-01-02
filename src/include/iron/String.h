#ifndef IRON_STRING_H
#define IRON_STRING_H

#include "Vec.h"

#include <string>

#include <string.h>

namespace iron
{

namespace _
{

template <typename T>
struct String
{
  /*************************************************************
   * String foo;
   *************************************************************/
  String()
  {
    raw.push(0);
  }

  /*************************************************************
   * String foo("bar");
   * String bar = "bar";
   *************************************************************/
  String(const char* cstr)
  {
    size_t len = strlen(cstr);

    for(size_t i = 0; i < len; i++)
    {
      raw.push(cstr[i]);
    }

    raw.push(0);
  }

  /*************************************************************
   * String foo(std::string("foo"));
   * String bar = std::string("bar");
   * TODO: Legacy
   *************************************************************/
  String(const std::string& str)
  {
    for(size_t i = 0; i < str.length(); i++)
    {
      raw.push(str[i]);
    }

    raw.push(0);
  }

  /*************************************************************
   * String foo;
   * foo = "bar";
   *************************************************************/
  String& operator=(const char* cstr)
  {
    size_t len = strlen(cstr);
    raw.clear();

    for(size_t i = 0; i < len; i++)
    {
      raw.push(cstr[i]);
    }

    raw.push(0);

    return *this;
  }

  /*************************************************************
   * String foo;
   * foo += 'b';
   * foo += 'a';
   * foo += 'r';
   *************************************************************/
  String& operator+=(const char& c)
  {
    raw[raw.size() - 1] = c;
    raw.push(0);

    return *this;
  }

  /*************************************************************
   * String foo;
   * foo += "bar";
   *************************************************************/
  String& operator+=(const char* cstr)
  {
    size_t len = strlen(cstr);

    for(size_t i = 0; i < len; i++)
    {
      (*this) += cstr[i];
    }

    return *this;
  }

  /*************************************************************
   * String foo;
   * String bar = foo + "bar";
   *************************************************************/
  String operator+(const char* str)
  {
    size_t len = strlen(str);
    String rtn = *this;

    for(size_t ci = 0; ci < len; ci++)
    {
      rtn += str[ci];
    }

    return rtn;
  }

  /*************************************************************
   * String foo;
   * const char* bar = foo;
   *************************************************************/
  operator const char*() const
  {
    return &raw[0];
  }

  /*************************************************************
   * String foo;
   * if(foo == "bar") { }
   *************************************************************/
  bool operator==(const char* other)
  {
    if(strcmp(&raw[0], other))
    {
      return false;
    }

    return true;
  }


  /*************************************************************
   * String foo;
   * if(foo != "bar") { }
   *************************************************************/
  bool operator!=(const char* other)
  {
    if(strcmp(&raw[0], other))
    {
      return true;
    }

    return false;
  }

  size_t length() const
  {
    return raw.size() - 1;
  }

  String substring(size_t start, size_t num) const
  {
    String rtn;

    if(start + num > length())
    {
      num = length() - start;
    }

    for(size_t i = 0; i < num; i++)
    {
      rtn += (*this)[start + i];
    }

    return rtn;
  }

private:
  Vec<char> raw;

};

}

typedef _::String<int> String;

}

#endif
