//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NuclexFreeType.h - FreeType plugin implementation   //
// ### # #      ###                                                      //
// # ### #      ###  The plugin implementation for nuclex                //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_FREETYPE_H
#define NUCLEX_FREETYPE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Synchronization.h"

#include "Nuclex/Kernel.h"

#include "FT2Build.h"
#include FT_FREETYPE_H

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the nuclex library and nowhere else.
//
// If you dynamically link against nuclex, you should define NUCLEX_DLL for
// your project in order to import the dll symbols from nuclex.
//
#ifdef FREETYPEPLUGIN_EXPORTS
  #define NUCLEXFREETYPE_API __declspec(dllexport)
#else
  #define NUCLEXFREETYPE_API __declspec(dllimport)
#endif

namespace Nuclex { namespace Text {

/// Get the FreeType libraries handle
NUCLEXFREETYPE_API FT_Library getFreeTypeLibrary();

/// Get the FreeType mutex used for all thread-critical regions
NUCLEXFREETYPE_API Mutex &getFreeTypeMutex();

// ############################################################################################# //
// # Nuclex::Text::getFreeTypeErrorDescription()                                               # //
// ############################################################################################# //
/// Obtain a textual description of a FreeType error code
/** Returns a human-readable description of an error given its error code

    @param  nErrorCode  The error code whose textual description to look up
    @return The text associated with the error code specified when calling this function
*/
NUCLEXFREETYPE_API inline string getFreeTypeErrorDescription(int nErrorCode) {
  // A little hack to enforce the header to be processed a second time
  #undef __FTERRORS_H__

  // This is FreeType's way to help define our own structure for storing the
  // FreeType error codes
  #define FT_ERRORDEF(e, v, s) { e, s },
  #define FT_ERROR_START_LIST {
  #define FT_ERROR_END_LIST { 0, 0 } };

  // By including the header below, we actually provide this structure
  // definition with a list of all the error codes known to FreeType
  static const struct {
    int         nErrorCode;
    const char *pszErrorMessage;
  } FreeTypeErrors[] =
  #include FT_ERRORS_H

  // We're done, undef the symbols to not pollute the global namespace
  #undef FT_ERRORDEF
  #undef FT_ERROR_START_LIST
  #undef FT_ERROR_END_LIST

  // This is the actual error lookup, performed by as a simple incremental search ;)
  for(
    size_t ErrorIndex = 0;
    ErrorIndex < sizeof(FreeTypeErrors) / sizeof(*FreeTypeErrors);
    ++ErrorIndex
  ) {

    // If this is the error code we're looking for, return it!
    if(FreeTypeErrors[ErrorIndex].nErrorCode == nErrorCode)
      return FreeTypeErrors[ErrorIndex].pszErrorMessage;

  }

  // Error code not found, report it as being an unknown error
  return string("unknown error ") + lexical_cast<string>(nErrorCode);
}

}} // namespace Nuclex::Text


using namespace Nuclex;

//  //
//  FreeTypeInitTerm                                                     //
//  //
/// FreeType global initialization class
/** Initializes global DLL data
*/
class FreeTypeInitTerm {
  public:
    /// Constructor
    /** Initializes global data when the dll enters a process
    */
    FreeTypeInitTerm() {
      FT_Error Error = ::FT_Init_FreeType(&m_FTLibrary);
      if(Error) {
        Kernel::logMessage(Kernel::MT_WARNING,
                           "FreeType library could not be initialized. FreeType support disabled.");
        return;
      }

      Kernel::logMessage(Kernel::MT_INFO,
                         "FreeType initialized successfully.");
    }

    /// Destructor
    /** Destroys global data when the dll leaves a process
    */
    ~FreeTypeInitTerm() {
      ::FT_Done_FreeType(m_FTLibrary);
      m_FTLibrary = NULL;
    }
    
    FT_Library getFTLibrary() const { return m_FTLibrary; }
    
  private:
    FT_Library m_FTLibrary;
};


DECLARATIONPACKAGE(FreeTypePlugin);

#endif // NUCLEX_FREETYPE_H