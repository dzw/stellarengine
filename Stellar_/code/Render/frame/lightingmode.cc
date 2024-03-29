//------------------------------------------------------------------------------
//  lightingmode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/lightingmode.h"

namespace Frame
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
LightingMode::Code
LightingMode::FromString(const String& str)
{
    if (str == "None")              return None;
    else if (str == "SinglePass")   return SinglePass;
    else if (str == "MultiPass")    return MultiPass;
    else
    {
        s_error("LightingMode::FromString(): invalid string '%s'!", str.c_str());
        return None;
    }
}

//------------------------------------------------------------------------------
/**
*/
String
LightingMode::ToString(Code c)
{
    switch (c)
    {
        case None:          return "None";
        case SinglePass:    return "SinglePass";
        case MultiPass:     return "MultiPass";
        default:
            s_error("LightingMode::ToString(): invalid code!");
            return "";
    }
}

} // namespace Frame
