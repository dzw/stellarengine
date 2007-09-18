//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Script.h - Nuclex script base class                 //
// ### # #      ###                                                      //
// # ### #      ###  Base class for a script                             //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_SCRIPT_H
#define NUCLEX_SCRIPT_SCRIPT_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Script/ArgumentList.h"
#include "Nuclex/Support/Variant.h"
#include "Nuclex/Support/String.h"

namespace Nuclex { namespace Script {

//  //
//  Nuclex::Script                                                       //
//  //
/// A script
/** Abstraction of a script in a scripting language
*/
class Script {
  public:
    /// Script error
//    NUCLEX_DECLAREEXCEPTION(InvalidArgumentException, ScriptErrorException);

    /// Destructor
    /** Destroys an instance of IScript
    */
    NUCLEX_API virtual ~Script() {}

  //
  // IScript implementation
  //
  public:
    /// Parse script
    /** Instructs the script to parse the script. For most implementations
        this means to convert the script string into bytecode, for some it
        includes to execute the global part of the script.

        @param  sScript  Script to parse
    */
    NUCLEX_API virtual void parseScript(const string &sScript) = 0;

    /// Call procedure
    /** Calls a procedure in the script. The script will try to execute
        the specified procedure. If procedures are not a concept of the
        scripting implementation, a CNotSupported exception will be thrown.

        @param  sProc  Name of the procedure to call
        @param  Args   Arguments for the procedure string
    */
    NUCLEX_API virtual Variant callProcedure(const string &sProc, const ArgumentList &Args) = 0;
    
    /// Execute command
    /** Executes a command in the scripting language. Similar to parsesScript,
        parses and executes the specified script string, but incrementally
        executes the command as if it where a line after the previous command
        in the same script.

        @param  sCommand  Script string to execute
    */
    NUCLEX_API virtual Variant executeCommand(const string &sCommand = "") = 0;
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_SCRIPT_H
