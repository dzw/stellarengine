//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## PythonScript.h - Nuclex Python script               //
// ### # #      ###                                                      //
// # ### #      ###                                                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_PYTHONSCRIPT_H
#define NUCLEX_SCRIPT_PYTHONSCRIPT_H

#include "PythonPlugin/PythonPlugin.h"
#include "Nuclex/Script/Script.h"
#include "Nuclex/Support/String.h"

#include "Python/Python.h"

namespace Nuclex { namespace Script {

//  //
//  Nuclex::PythonScript                                                 //
//  //
/// Python script
/** A script in the Python scripting language
*/
class PythonScript :
  public Script {
  public:
    /// Constructor
    NUCLEXPYTHON_API PythonScript(const string &sScript = "");
    /// Destructor
    NUCLEXPYTHON_API inline virtual ~PythonScript();

  //
  // PythonScript implementation
  //
  public:

  //
  // Script implementation
  //
  public:
    /// Parse script
    NUCLEXPYTHON_API void parseScript(const string &sScript);
    /// Call procedure
    NUCLEXPYTHON_API Variant callProcedure(const string &sProc, const ArgumentList &Args);
    /// Execute command
    NUCLEXPYTHON_API Variant executeCommand(const string &sCommand = "");

  private:
    PyInterpreterState *m_pPythonInterpreter;
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_PYTHONSCRIPT_H
