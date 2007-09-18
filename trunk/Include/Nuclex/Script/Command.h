//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Command.h - Nuclex Command                          //
// ### # #      ###                                                      //
// # ### #      ###  Encapsulates a script command                       //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#define NCX_COMMAND_H
#ifndef NCX_COMMAND_H
#define NCX_COMMAND_H

#include "Nuclex/Config.h"
#include "Nuclex/Storage/Persistable.h"
#include "Nuclex/Support/ArgumentList.h"

namespace Nuclex {

//  //
//  Nuclex::CCommand                                                        //
//  //
/// Command encapsulation
/** The command object stores all data required for a remote call to
    any scriptable class. This object is typically passed to the Invoke()
    method of classes derived from CScriptable.
    A command consists of a procedure name, a variable number of input
    arguments as well as output arguments.
*/
class CCommand :
  public Persistable,
  public CObject {
  public:
    /// Constructor
    NUCLEX_API CCommand(void);
    /// Copy constructor
    NUCLEX_API CCommand(const CCommand &Command);
    /// Construct from argument string
    NUCLEX_API CCommand(const String &sName, const String &sArgs);
    /// Destructor
    NUCLEX_API virtual ~CCommand(void);

  //
  // CCommand implementation
  //
  public:
    // General services
    //
    /// Clear command of all settings
    NUCLEX_API void clearCommand(void);

    /// Retrieve command name
    NUCLEX_API const String &getName(void) const;
    /// Set command name
    NUCLEX_API void setName(const String &sCommand);
    /// Check if command matches definition
    NUCLEX_API bool matchesDefinition(const String &sID);

    // Argument services
    //
    /// Retrieve command arguments
    NUCLEX_API ArgumentList &getArgs(void);
    /// Retrieve command arguments (const)
    NUCLEX_API const ArgumentList &getArgs(void) const;
    /// Retrieve return values
    NUCLEX_API ArgumentList &getReturns(void);
    /// Retrieve return values (const)
    NUCLEX_API const ArgumentList &getReturns(void) const;

  //
  // IPersistable implementation
  //
  public:
    /// Load class from serializer
    NUCLEX_API void loadObject(ISerializer &Serializer);
    /// Save class into serializer
    NUCLEX_API void saveObject(ISerializer &Serializer) const;

  private:
    String       m_sName;                            ///< Command name
    ArgumentList m_Args;                             ///< Arguments
    ArgumentList m_Returns;                          ///< Return values
};

} // namespace Nuclex

#endif NCX_COMMAND_H
