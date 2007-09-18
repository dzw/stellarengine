//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## TerrainPlugin.h - Nuclex image extension module      //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_TERRAINPLUGIN_H
#define NUCLEX_TERRAINPLUGIN_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::Scene;

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the nuclex library and nowhere else.
//
// If you dynamically link against nuclex, you should define NUCLEX_DLL for
// your project in order to import the dll symbols from nuclex.
//
#ifdef TERRAINPLUGIN_EXPORTS
  #define NUCLEXTERRAIN_API __declspec(dllexport)
#else
  #define NUCLEXTERRAIN_API __declspec(dllimport)
#endif

DECLARATIONPACKAGE(TerrainPlugin);

#endif // NUCLEX_TERRAINPLUGIN_H
