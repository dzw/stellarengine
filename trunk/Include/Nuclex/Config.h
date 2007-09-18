//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Config.h - Build configuration                      //
// ### # #      ###                                                      //
// # ### #      ###  Contains various #defines for configuring           //
// #  ## #   # ## ## the Nuclex build process                            //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_CONFIG_H
#define NUCLEX_CONFIG_H

#ifndef __cplusplus
  #error Nuclex requires C++
#endif

// #include "MemoryTracker/MemoryTracker.h"

// Defines the current version of nuclex. Can be used by 
// applications to identify compatibility issues
//
#define NUCLEX_VERSION 0x050

// Platform recognition
//
#if defined(WIN32) || defined(_WIN32)
  #define NUCLEX_WIN32
#else
  #define NUCLEX_LINUX
#endif

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (nuclex library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the nuclex library and nowhere else.
//
#ifdef NUCLEX_EXPORTS
  #define NUCLEX_API __declspec(dllexport)
#else
  #define NUCLEX_API //__declspec(dllimport)
#endif

#include <cstddef>
#include <numeric>

namespace Nuclex {

// Import size_t from std into the nuclex namespace
using std::size_t;

// These are only used for some special hardware-dependant operations
// such as interfacing with the graphics adapter or writing image files
#ifdef _MSC_VER
  #pragma warning(disable:4786) // Warning: Debug symbol truncated to 255 characters
  typedef unsigned __int8 unsigned_8;
  typedef unsigned __int16 unsigned_16;
  typedef unsigned __int32 unsigned_32;
  const size_t BitsPerByte = 8;
#else
  #pragma warning("Unknown byte size for current platform. Assuming x86 layout")
  typedef unsigned char unsigned_8;
  typedef unsigned short unsigned_16;
  typedef unsigned long unsigned_32;
  const size_t BitsPerByte = 8;
#endif

} // namespace Nuclex

#endif // NUCLEX_CONFIG_H
