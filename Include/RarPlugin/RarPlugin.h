//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RarPlugin.h - Rar archive reader plugin                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Provides the required dll function to register the                        //
// #  ## #   # ## ## rar archive reader plugin to nuclex                                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_RARPLUGIN_H
#define NUCLEX_RARPLUGIN_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Synchronization.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Kernel.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "UnRar/UnRar.h"

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the nuclex library and nowhere else.
//
// If you dynamically link against nuclex, you should define NUCLEX_DLL for
// your project in order to import the dll symbols from nuclex.
//
#ifdef RARPLUGIN_EXPORTS
  #define NUCLEXRAR_API __declspec(dllexport)
#else
  #define NUCLEXRAR_API __declspec(dllimport)
#endif

namespace Nuclex {

// ############################################################################################# //
// # Nuclex::RarCheck()                                                                        # //
// ############################################################################################# //
/** Rar result code check

    @param  sSource  Source of the call
    @param  sMethod  Method being called
    @param  nResult  Method result code
*/
inline void RarCheck(const string &sSource, const string &sMethod, int nResult) {
  if(nResult) {
    string sError = string("Unexpected failure in ") + sMethod + ": ";
    switch(nResult) { 
      case ERAR_END_ARCHIVE:    sError += "End of archive reached"; break;
      case ERAR_NO_MEMORY:      sError += "Out of memory"; break;
      case ERAR_BAD_DATA:       sError += "Data corrupted or wrong password"; break;
      case ERAR_BAD_ARCHIVE:    sError += "Archive damaged"; break;
      case ERAR_UNKNOWN_FORMAT: sError += "Unsupported archive format"; break;
      case ERAR_EOPEN:          sError += "Can't open file"; break;
      case ERAR_ECREATE:        sError += "Unable to create file"; break;
      case ERAR_ECLOSE:         sError += "Can't close file"; break;
      case ERAR_EREAD:          sError += "Read error"; break;
      case ERAR_EWRITE:         sError += "Write error"; break;
      case ERAR_SMALL_BUF:      sError += "Buffer too small"; break;
      case ERAR_UNKNOWN:        sError += "Unknown error"; break;
      default:                  sError += "Unknown result code"; break;
    }

    throw UnexpectedException(sSource, sError);
  }
}

/// Get mutex for rar api
NUCLEXRAR_API Mutex &getRarMutex();

} // namespace Nuclex

DECLARATIONPACKAGE(RarPlugin);

#endif // NUCLEX_RARPLUGIN_H
