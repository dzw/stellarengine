//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ZipPlugin.h - Zip archive reader plugin                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Provides the required dll function to register the                        //
// #  ## #   # ## ## zip archive reader plugin to nuclex                                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_ZIPPLUGIN_H
#define NUCLEX_ZIPPLUGIN_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Synchronization.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Exception.h"
#include "ZLib/ZLib.h"
#include "Nuclex/Kernel.h"

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the flex library and nowhere else.
//
// If you dynamically link against flex, you should define NUCLEX_DLL for
// your project in order to import the dll symbols from flex.
//
#ifdef ZIPPLUGIN_EXPORTS
  #define NUCLEXZIP_API __declspec(dllexport)
#else
  #define NUCLEXZIP_API __declspec(dllimport)
#endif

DECLARATIONPACKAGE(ZipPlugin);

#endif // NUCLEX_ZIPPLUGIN_H
