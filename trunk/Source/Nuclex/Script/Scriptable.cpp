//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Scriptable.cpp - Scripting-enabled object           //
// ### # #      ###                                                      //
// # ### #      ###  Base class for scripting-enabled objects            //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Script/Scriptable.h"

using namespace Nuclex;

#if 0
// ####################################################################### //
// # Nuclex::Scriptable::invokeMethod()                                     # //
// ####################################################################### //
/** Perform a remote method call on the class. This method
    is used by the scripting interface.

    @param  Command  Command to execute; input & output arguments
*/
void Scriptable::invokeMethod(CCommand &Command) {
  throw 1; //InvalidCommandException("Nuclex::CScriptable::Invoke()",
           //                     String("Command '") + Command.getName() + "' not supported or wrong syntax");

  return;
}

// ####################################################################### //
// # Nuclex::Scriptable::getMethods()                                       # //
// ####################################################################### //
/** Retrieve a list of the methods supported by the class

    @param  List  A method list receiver into which all methods
                  the class publishes will be written
*/
void Scriptable::getMethods(Scriptable::MethodList &List) {
  static const Scriptable::MethodList::Method pMethods[] = {
    { "invokeMethod(s, ...)" },
    NULL
  };
  List.addClass("Scriptable", pMethods);

  return;
}
#endif