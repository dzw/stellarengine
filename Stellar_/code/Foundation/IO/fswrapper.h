#pragma once
#ifndef INTERNAL_FSWRAPPER_H
#define INTERNAL_FSWRAPPER_H
//------------------------------------------------------------------------------
/**
    @class Internal::FSWrapper
    
    This is an internal IO class used to wrap platform specific
    filesystem access into a generic class. To port the filesystem code
    to a new platform all that has to be done is to write a new 
    FSWrapper class.
    
    (C) 2006 Radon Labs GmbH
*/
#if __WIN32__
#include "io/win32/win32fswrapper.h"
namespace Internal
{
class FSWrapper : public Win32::Win32FSWrapper
{ };
}
#else
#error "FSWrapper class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

