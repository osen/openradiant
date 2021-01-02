#ifndef IRON_PANIC_H
#define IRON_PANIC_H

#include <stdio.h>
#include <stdlib.h>

#define panic(STR) \
  do                                                                                     \
  {                                                                                      \
    /*const char* castTest = STR; */                                                     \
    printf("Panic: %s\n  File: %s\n  Line: %i\n", (const char*)STR, __FILE__, __LINE__); \
    abort();                                                                             \
  }                                                                                      \
  while(0)

#endif
