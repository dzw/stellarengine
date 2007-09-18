//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NuclexLua.h - Lua plugin implementation             //
// ### # #      ###                                                      //
// # ### #      ###  The plugin implementation for nuclex                //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_LUA_H
#define NUCLEX_LUA_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Kernel.h"

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the nuclex library and nowhere else.
//
// If you dynamically link against nuclex, you should define NUCLEX_DLL for
// your project in order to import the dll symbols from nuclex.
//
#ifdef LUAPLUGIN_EXPORTS
  #define NUCLEXLUA_API __declspec(dllexport)
#else
  #define NUCLEXLUA_API __declspec(dllimport)
#endif

DECLARATIONPACKAGE(LuaPlugin);

#endif // NUCLEX_LUA_H