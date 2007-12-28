#pragma once
#ifndef DEBUG_MINIDUMP_H
#define DEBUG_MINIDUMP_H
//------------------------------------------------------------------------------
/**
    @class Debug::MiniDump
    
    Support for generating mini dumps. Mini dumps are automatically 
    created when s_assert() or s_error() triggers.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "debuging/win32/win32minidump.h"
namespace Debug
{
class MiniDump : public Win32::Win32MiniDump
{ };
}
#else
#error "Debug::MiniDump class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
