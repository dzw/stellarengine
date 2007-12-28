#pragma once
#ifndef COREGRAPHICS_ADAPTER_H
#define COREGRAPHICS_ADAPTER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::Adapter
    
    Display adapter enum.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "utility/string.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class Adapter
{
public:
    /// enum
    enum Code
    {
        Primary = 0,
        Secondary,    
        None,
    };

    /// convert adapter code from string
    static Code FromString(const Util::String& str);
    /// convert adapter code to string
    static Util::String ToString(Code code);
};

} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
