#pragma once
#ifndef CORE_SYSFUNC_H
#define CORE_SYSFUNC_H
//------------------------------------------------------------------------------
/**
    @class Core::SysFunc
    
    Wrap some platform specific low-level functions.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "core/win32/sysfunc.h"
namespace Core
{
class SysFunc : public Win32::SysFunc
{
    // empty
};
} // namespace Core
#else
#error "Core::SysFunc not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
