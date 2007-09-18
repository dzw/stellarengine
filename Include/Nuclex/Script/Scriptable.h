//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Scriptable.h - Scripting-enabled object             //
// ### # #      ###                                                      //
// # ### #      ###  Base class for scripting-enabled objects            //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NCX_SCRIPTABLE_H
#define NCX_SCRIPTABLE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Script/Command.h"
#include "Nuclex/Support/Exception.h"

namespace Nuclex {

//  //
//  Nuclex::Scriptable                                                      //
//  //
/// Scriptable object interface
/** Any classes whose methods should be remotely callable by scripts
    have to derive from this class. It contains a method for remotely
    invoking other methods of the class as well as a method which can
    be used to query the class for the methods it supports.
*/
class Scriptable {
  public:
    /// Invalid command
//    NUCLEX_DECLAREEXCEPTION(InvalidArgException, InvalidCommandException);

    /// Invokable methods list receiver
    class MethodList;

    /// Destructor
    /** Destroys an instance of IScriptable
    */
    NUCLEX_API virtual ~Scriptable(void) {}

  //
  // IScriptable implementation
  //
  public:
    /// Invoke a method of the class
    /** This method should be implemented by any scriptable class
        that has public methods. If the method is not known to the
        object, it should hand the call on to its base classes,
        finally reaching CScriptable which will fire an exception.

        @param  Command  Command object containing call informations
    */
    //NUCLEX_API virtual void invokeMethod(CCommand &Command);

    /// Get a list of the methods supports by the class
    /** Has to be implemented in any scriptable class. The MethodList
        should be filled with all methods the class' Invoke() method
        understands to carry out, exactly matching the the command
        identifier strings that would be used by CCommand::Is().
        The call should be handed down through any base classes the
        class may have.

        @param  MethodList  MethodList object to fill
    */
    //NUCLEX_API virtual void getMethods(IMethodList &MethodList);
};

//  //
//  Nuclex::Scriptable::MethodList                                          //
//  //
/** Stores information about invokable methods of a class and its
    base classes. Used by CScriptable::Methods() to obtain call
    information.
*/
class Scriptable::MethodList {
  public:
    /// Informations about a single method
    /** Contains informations about a method
    */
    struct Method {
      const char *pszDef;                             ///< Method definition
    };

    /// Destructor
    /** Destroys an instance of IMethodList
    */
    //NUCLEX_API virtual ~IMethodList(void) {};

  //
  // IMethodList implementation
  //
  public:
    /// Add a class' methods to the method list
    /** Supply the name of the class without C-prefix as the first
        argument, the second should be a NULL terminated array of
        methods which the scriptable object supports through Invoke()

        @param  sClass    Name of the class
        @param  pMethods  NULL terminated method array
    */
    //NUCLEX_API virtual void addClass(const String &sClass, const Method *pMethods) = 0;
};

} // namespace Nuclex

#endif NCX_SCRIPTABLE_H