//------------------------------------------------------------------------------
//  adapter.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/adapter.h"

namespace CoreGraphics
{

//------------------------------------------------------------------------------
/**
*/
Adapter::Code
Adapter::FromString(const Util::String& str)
{
    if ("None" == str) return None;
    else if ("Primary" == str) return Primary;
    else if ("Secondary" == str) return Secondary;
    else
    {
        s_error("Invalid adapter string '%s'!", str.c_str());
        return Primary;
    }
}

//------------------------------------------------------------------------------
/**
*/
Util::String
Adapter::ToString(Code code)
{
    switch (code)
    {
        case None:      return "None";
        case Primary:   return "Primary";
        case Secondary: return "Secondary";
        default:
            s_error("Invalid adapter code!");
            return "";
    }
}

} // namespace CoreGraphics
