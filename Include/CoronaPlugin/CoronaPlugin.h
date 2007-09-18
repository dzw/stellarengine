//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## CoronaPlugin.h - Nuclex image extension module      //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_CORONAPLUGIN_H
#define NUCLEX_CORONAPLUGIN_H

#include "Nuclex/Nuclex.h"
//#include "MemoryTracker/DisableMemoryTracker.h"
#include "Corona/corona.h"
//#include "MemoryTracker/MemoryTracker.h"
#include "Nuclex/Video/Surface.h"
#include "Nuclex/Kernel.h"

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the nuclex library and nowhere else.
//
// If you dynamically link against nuclex, you should define NUCLEX_DLL for
// your project in order to import the dll symbols from nuclex.
//
#ifdef CORONAPLUGIN_EXPORTS
  #define NUCLEXCORONA_API __declspec(dllexport)
#else
  #define NUCLEXCORONA_API __declspec(dllimport)
#endif

namespace Nuclex { namespace Video {

// ####################################################################### //
// # Nuclex::Image::CoronaPixelFormat2PixelFormat()                      # //
// ####################################################################### //
/** Converts a corona pixel format into the nuclex pixel format

    @param  eFormat  Corona pixel format
    @return The matching nuclex pixel format
*/
inline Video::Surface::PixelFormat PixelFormatFromCoronaPixelFormat(corona::PixelFormat eFormat) {
  switch(eFormat) {
    case corona::PF_R8G8B8A8: return Video::Surface::PF_ARGB_8_8_8_8;
    case corona::PF_R8G8B8:   return Video::Surface::PF_RGB_8_8_8;
    case corona::PF_I8:       return Video::Surface::PF_RGB_3_3_2;
    //case corona::PF_B8G8R8A8: return Surface::PixelFormat::PF_ARGB_8_8_8_8;
    //case corona::PF_B8G8R8:   return Surface::PixelFormat::PF_RGB_8_8_8;
    default:                  return Video::Surface::PF_NONE;
  }
}

}} // namespace Nuclex::VIDEO

DECLARATIONPACKAGE(CoronaPlugin);

#endif // NUCLEX_CORONAPLUGIN_H
