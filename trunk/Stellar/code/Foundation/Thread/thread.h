#pragma once
#ifndef THREADING_THREAD_H
#define THREADING_THREAD_H
//------------------------------------------------------------------------------
/**
    @class Threading::Thread
    
    @todo describe Thread class
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "thread/win32/win32thread.h"
namespace Threading
{
class Thread : public Win32::Win32Thread
{ 
    DeclareClass(Thread);
};
}
#else
#error "Threading::Thread not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
