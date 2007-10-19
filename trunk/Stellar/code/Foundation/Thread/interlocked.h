#pragma once
#ifndef THREADING_INTERLOCKED_H
#define THREADING_INTERLOCKED_H
//------------------------------------------------------------------------------
/**
    @class Threading::Interlocked
    
    Provide simple atomic operations on memory variables.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "thread/win32/win32interlocked.h"
namespace Threading
{
class Interlocked : public Win32::Win32Interlocked
{ };
}
#else
#error "Threading::Interlocked not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
