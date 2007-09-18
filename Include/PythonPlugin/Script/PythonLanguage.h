//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## PythonLanguage.h - Nuclex Python language           //
// ### # #      ###                                                      //
// # ### #      ###  Implements a flex scriting language using the       //
// #  ## #   # ## ## Python interpreter                                  //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_PYTHONLANGUAGE_H
#define NUCLEX_SCRIPT_PYTHONLANGUAGE_H

#include "PythonPlugin/PythonPlugin.h"
#include "Nuclex/Script/Language.h"

namespace Nuclex { namespace Script {

//  //
//  Nuclex::PythonLanguage                                               //
//  //
/// Python scripting language
/** Nuclex scripting language implementation for Python
*/
class PythonLanguage :
  public Language {
  public:
    /// Constructor
    NUCLEXPYTHON_API PythonLanguage();
    /// Destructor
    NUCLEXPYTHON_API virtual ~PythonLanguage();

  //
  // Language implementation
  //
  public:
    // Script services
    //
    /// Create script
    NUCLEXPYTHON_API shared_ptr<Script> createScript(const string &sScript = "");
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_PYTHONLANGUAGE_H
