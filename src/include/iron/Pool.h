#include "Zero.h"

//#define THREADPOOL_DISABLE
#define THREADPOOL_THREADS 4

#ifndef THREADPOOL_DISABLE
  #ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
  #else
    #include <pthread.h>
    #include <semaphore.h>
  #endif
#endif

#include <vector>

namespace iron
{

template <typename T>
struct Pool
{
  Pool()
  {
#ifndef THREADPOOL_DISABLE
    units.resize(THREADPOOL_THREADS);
#else
    units.resize(1);
#endif

    for(typename std::vector<Unit>::iterator it =
      units.begin(); it != units.end(); it++)
    {
      it->tp = this;

#ifndef THREADPOOL_DISABLE
  #ifdef _WIN32
      it->start = CreateSemaphore(NULL, 0, 1, NULL);

      if(it->start == NULL)
      {
        throw std::exception();
      }

      it->stop = CreateSemaphore(NULL, 0, 1, NULL);

      if(it->stop == NULL)
      {
        throw std::exception();
      }

      it->thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entry, (LPVOID)(void *)&(*it), 0, NULL);

      if(it->thread == NULL)
      {
        throw std::exception();
      }
  #else
      if(sem_init(&it->start, 0, 0) == -1)
      {
        throw std::exception();
      }

      if(sem_init(&it->stop, 0, 0) == -1)
      {
        throw std::exception();
      }

      if(pthread_create(&it->thread, NULL, entry,
        (void *)&(*it)) != 0)
      {
        it->thread = pthread_self();
        throw std::exception();
      }
  #endif
#endif
    }
  }

  ~Pool()
  {
    func = NULL;

#ifndef THREADPOOL_DISABLE
    for(typename std::vector<Unit>::iterator it =
      units.begin(); it != units.end(); it++)
    {
  #ifdef _WIN32
      ReleaseSemaphore(it->start, 1, NULL);
      WaitForSingleObject(it->thread, INFINITE);
      CloseHandle(it->thread);
      CloseHandle(it->start);
      CloseHandle(it->stop);
  #else
      sem_post(&it->start);
      pthread_join(it->thread, NULL);
      sem_destroy(&it->start);
      sem_destroy(&it->stop);
  #endif
    }
#endif
  }

  size_t size()
  {
    return units.size();
  }

  T& at(size_t idx)
  {
    return units.at(idx).t;
  }

  void execute(void (*func)(T& t))
  {
    this->func = func;

#ifndef THREADPOOL_DISABLE
    for(typename std::vector<Unit>::iterator it =
      units.begin(); it != units.end(); it++)
    {
  #ifdef _WIN32
      ReleaseSemaphore(it->start, 1, NULL);
  #else
      sem_post(&it->start);
  #endif
    }

    for(typename std::vector<Unit>::iterator it =
      units.begin(); it != units.end(); it++)
    {
  #ifdef _WIN32
      WaitForSingleObject(it->stop, INFINITE);
  #else
      sem_wait(&it->stop);
  #endif
    }

#else
    func(units.at(0).t);
#endif
    this->func = NULL;
  }

private:
  struct Unit
  {
    T t;
#ifndef THREADPOOL_DISABLE
  #ifdef _WIN32
    HANDLE thread;
    HANDLE start;
    HANDLE stop;
  #else
    pthread_t thread;
    sem_t start;
    sem_t stop;
  #endif
#endif
    Pool *tp;
  };

  std::vector<Unit> units;
  Zero<void (*)(T& t)> func;

#ifndef THREADPOOL_DISABLE
#ifdef _WIN32
  static DWORD WINAPI entry(LPVOID ptr)
#else
  static void *entry(void *ptr)
#endif
  {
    Unit *tu = (Unit*)ptr;

    while(true)
    {
  #ifdef _WIN32
      WaitForSingleObject(tu->start, INFINITE);
  #else
      sem_wait(&tu->start);
  #endif

      if(!tu->tp->func)
      {
        break;
      }
      else
      {
        tu->tp->func(tu->t);
      }

  #ifdef _WIN32
      ReleaseSemaphore(tu->stop, 1, NULL);
  #else
      sem_post(&tu->stop);
  #endif
    }

    return NULL;
  }
#endif

};

}

