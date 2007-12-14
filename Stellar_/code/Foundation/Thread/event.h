#pragma once
#ifndef THREADING_EVENT_H
#define THREADING_EVENT_H
//------------------------------------------------------------------------------
/**
    @class Threading::Event
  
    @todo describe Event class
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "thread/win32/win32event.h"
namespace Threading
{
class Event : public Win32::Win32Event
{ };
}
#else
#error "Threading::Event not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
