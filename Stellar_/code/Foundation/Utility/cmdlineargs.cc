//------------------------------------------------------------------------------
//  utility/cmdlineargs.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "utility/cmdlineargs.h"

namespace Util
{
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CmdLineArgs::CmdLineArgs()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CmdLineArgs::CmdLineArgs(const String& l)
{
	this->args = Tokenize(" \t\n=", "\"");
}

//------------------------------------------------------------------------------
/**
*/
CmdLineArgs::CmdLineArgs(int argc, const char** argv)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        this->args.push_back(argv[i]);
    }
}

//------------------------------------------------------------------------------
/**
    Put out a missing argument error.
*/
void
CmdLineArgs::Error(const String& argName) const
{
    s_error("CmdLine: arg '%s' not found!", argName.c_str());
}

//------------------------------------------------------------------------------
/**
    Returns the command name.
*/
const String&
CmdLineArgs::GetCmdName() const
{
    s_assert(this->args.size() > 0);
    return this->args[0];
}

//------------------------------------------------------------------------------
/**
    Returns true if argument exists.
*/
bool
CmdLineArgs::HasArg(const String& name) const
{
    return (this->args.Find(name) != this->args.end());
}

//------------------------------------------------------------------------------
/**
    Returns index of argument. Throws an error if argument not found.
*/
int
CmdLineArgs::FindArgIndex(const String& name) const
{
    /*s_assert(!name.empty());
    IndexT i = this->args.FindIndex(name);
    if (InvalidIndex == i)
    {
        this->Error(name);
    }
    return i;*/
	return 0;
}

//------------------------------------------------------------------------------
/**
*/
const String&
CmdLineArgs::GetString(const String& name, const String& defaultValue) const
{
    /*if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1];
    }
    else
    {
        return defaultValue;
    }*/
	return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
int
CmdLineArgs::GetInt(const String& name, int defaultValue) const
{
    /*if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsInt();
    }
    else
    {
        return defaultValue;
    }*/
	return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
float
CmdLineArgs::GetFloat(const String& name, float defaultValue) const
{
    /*if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsFloat();
    }
    else
    {
        return defaultValue;
    }*/

	return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
bool
CmdLineArgs::GetBool(const String& name) const
{
    return this->HasArg(name);
}

//------------------------------------------------------------------------------
/**
*/
float4
CmdLineArgs::GetFloat4(const String& name, const float4& defaultValue) const
{
   /* if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsFloat4();
    }
    else
    {
        return defaultValue;
    }*/
	 return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
matrix44
CmdLineArgs::GetMatrix44(const String& name, const matrix44& defaultValue) const
{
    /*if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsMatrix44();
    }
    else
    {
        return defaultValue;
    }*/
	 return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
SizeT
CmdLineArgs::GetNumArgs() const
{
    s_assert(this->args.size() > 0);
    return (SizeT)(this->args.size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
const String&
CmdLineArgs::GetStringAtIndex(IndexT index) const
{
    return this->args[index + 1];
}

//------------------------------------------------------------------------------
/**
*/
int
CmdLineArgs::GetIntAtIndex(IndexT index) const
{
    return 0;//this->args[index + 1].c_str();
}

//------------------------------------------------------------------------------
/**
*/
float
CmdLineArgs::GetFloatAtIndex(IndexT index) const
{
    return 0.0f;//this->args[index + 1].c_str();
}

//------------------------------------------------------------------------------
/**
*/
bool
CmdLineArgs::GetBoolAtIndex(IndexT index) const
{
    return false;//this->args[index + 1].AsBool();
}

//------------------------------------------------------------------------------
/**
*/
float4
CmdLineArgs::GetFloat4AtIndex(IndexT index) const
{
    return float4(0,0,0,0);//this->args[index + 1].AsFloat4();
}

//------------------------------------------------------------------------------
/**
*/
matrix44
CmdLineArgs::GetMatrix44AtIndex(IndexT index) const
{
	return matrix44::identity();//this->args[index + 1].AsMatrix44();
}

} // namespace Util
