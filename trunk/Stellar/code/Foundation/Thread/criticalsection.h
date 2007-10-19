#pragma once
#ifndef THREADING_CRITICALSECTION_H
#define THREADING_CRITICALSECTION_H
//------------------------------------------------------------------------------
/**
    @class Threading::CriticalSection

    Critical section objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and use its Enter() 
    and Leave() methods to protect critical sections of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "thread/win32/win32criticalsection.h"
namespace Threading
{
class CriticalSection : public Win32::Win32CriticalSection
{ };
};
#else
#error "Threading::CriticalSection not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
    