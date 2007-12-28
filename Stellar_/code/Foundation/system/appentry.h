#pragma once
#ifndef SYSTEM_APPENTRY_H
#define SYSTEM_APPENTRY_H
//------------------------------------------------------------------------------
/**
    @class System::AppEntry
    
    Implements a platform-indepent app-entry point. In your main file,
    put the line
    
    ImplementStellarApplication();
    
    And then replace your main() function with:
    
    void StellarMain(const CmdLineArgs& args)

    (C) 2007 by ctuo
*/
#include "core/types.h"

//------------------------------------------------------------------------------
#if __WIN32__
#define ImplementStellarApplication() \
void StellarMain(const Util::CmdLineArgs& args); \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd) \
{ \
    Util::CmdLineArgs args(lpCmdLine); \
    StellarMain(args); \
    return 0; \
}
#else
#define ImplementStellarApplication() \
void StellarMain(const Util::CmdLineArgs& args); \
int __cdecl main(int argc, const char** argv) \
{ \
    Util::CmdLineArgs args(argc, argv); \
    StellarMain(args); \
    return 0; \
}
#endif
//------------------------------------------------------------------------------
#endif