#pragma once
#ifndef TIMING_CALENDARTIME_H
#define TIMING_CALENDARTIME_H
//------------------------------------------------------------------------------
/**
    @class Timing::CalendarTime
    
    Allows to obtain the current point in time as year, month, day, etc...
    down to milliseconds, convert between filetime and CalendarTime, and
    format the time to a human readable string.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__)
#include "time/win32/win32calendartime.h"
namespace Timing
{
class CalendarTime : public Win32::Win32CalendarTime
{ };
}
#else
#error "Timing::CalendarTime not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
