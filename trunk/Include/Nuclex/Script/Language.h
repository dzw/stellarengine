//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Language.h - Nuclex scripting language              //
// ### # #      ###                                                      //
// # ### #      ###  Base class for a scripting language implementation  //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_LANGUAGE_H
#define NUCLEX_SCRIPT_LANGUAGE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
//#include "Nuclex/Support/Class.h"

namespace Nuclex { namespace Script {

class Script;

//  //
//  Nuclex::Language                                                     //
//  //
/// Nuclex scripting language
/** Base class for scripting languages. A scripting language can provide
    global options for the language and serves as factory for the scripts
    of the implemented language.
    Scripting languages in extension modules usually do not create and
    register a scripting language themselfes, but rather provide the
    kernel with a factory through which the language object can be created.
*/
class Language {
  public:
    /// Destructor
    NUCLEX_API virtual ~Language() {}

  //
  // Language implementation
  //
  public:
    // ----- Script services -----
    //
    /// Create script
    NUCLEX_API virtual shared_ptr<Script> createScript(const string &sScript = "") = 0;
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_LANGUAGE_H
