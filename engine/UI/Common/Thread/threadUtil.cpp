
#include <cstring>
#include <cstdint>

#include "engine.h"
#include "threadUtil.h"

#if defined(__ANDROID__) || defined(__APPLE__) || (defined(__GLIBC__) && defined(_GNU_SOURCE))
    #include <pthread.h>
#endif

#ifdef TLS_SUPPORTED
    static thread_local const char *curThreadName;
#endif



void setCurrentThreadName(const char* threadName)
{
    // Set the locally known threadname using a thread local variable.
#ifdef TLS_SUPPORTED
    curThreadName = threadName;
#endif
}

void AssertCurrentThreadName(const char *threadName)
{
#ifdef TLS_SUPPORTED
    if (strcmp(curThreadName, threadName) != 0)
    {
        LOG_CORE_ERROR("Thread name assert failed: Expected {0}, was {1}", threadName, curThreadName);
    }
#endif
}
