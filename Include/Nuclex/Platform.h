//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Nuclex.h - Main application header                  //
// ### # #      ###                                                      //
// # ### #      ###  General Nuclex inclusion header                     //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_PLATFORM_H
#define NUCLEX_PLATFORM_H

#include "Nuclex/Nuclex.h"

namespace Nuclex {

/// File system path formatting type
enum PathType {
  PT_NATIVE,                                          ///< Same as native file system
  PT_NUCLEX                                           ///< Always forward slashes
};

// ####################################################################### //
// # Nuclex::convertPath()                                               # // 
// ####################################################################### //
/** Converts a path between the operating systems native convention
    and the format used by nuclex.
    
    @param  sIn    Path to format
    @param  eType  Desired path convention
    @return The path formatted to the zlib convention
*/
inline string convertPath(const string &sIn, PathType eType = PT_NUCLEX) {
  string sTemp = sIn;
  
  switch(eType) {
    case PT_NATIVE: {
#ifdef NUCLEX_WIN32
      for(string::size_type Pos = 0; Pos < sTemp.length(); Pos++)
        if(sTemp[Pos] == '/')
          sTemp[Pos] = '\\';
#else
#error Not implemented yet
#endif
    }
    
    case PT_NUCLEX: {
#ifdef NUCLEX_WIN32
      for(string::size_type Pos = 0; Pos < sTemp.length(); Pos++)
        if(sTemp[Pos] == '\\')
          sTemp[Pos] = '/';
#else
#error Not implemented yet
#endif
      break;
    }
  }
  
  return sTemp;    
}

} // namespace Nuclex

#endif // NUCLEX_PLATFORM_H
