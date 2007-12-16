#pragma once
#ifndef UTIL_GUID_H
#define UTIL_GUID_H
//------------------------------------------------------------------------------
/**
    @class Util::Guid
    
    Implements a GUID.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "utility/win32/win32guid.h"
namespace Util
{
typedef Win32::Win32Guid Guid;
}
#else
#error "IMPLEMENT ME!"
#endif
//------------------------------------------------------------------------------
#endif
    