//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Command.cpp - Nuclex Command                        //
// ### # #      ###                                                      //
// # ### #      ###  Encapsulates a script command                       //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Script/Command.h"

#if 0 

using namespace Nuclex;

// ####################################################################### //
// # Nuclex::CCommand::CCommand()                               Constructor # // 
// ####################################################################### //
/** Creates an instance of an empty command.
*/
CCommand::CCommand(void) :
  m_sName(),
  m_Args(),
  m_Returns() {

  return;
}

// ####################################################################### //
// # Nuclex::CCommand::CCommand()                               Constructor # // 
// ####################################################################### //
/** Creates an instance of CCommand and copy of an existing command

    @param  Command  Command object to copy
*/
CCommand::CCommand(const CCommand &Command) :
  m_sName(Command.m_sName),
  m_Args(Command.m_Args),
  m_Returns(Command.m_Returns) {

  return;
}

// ####################################################################### //
// # Nuclex::CCommand::CCommand()                               Constructor # // 
// ####################################################################### //
/** Creates an instance of an empty command.

    @param  sName  Command name
    @param  sArgs  Argument string to be parsed by CArgList::ParseArgs()
*/
CCommand::CCommand(const String &sName, const String &sArgs) :
  m_sName(sName),
  m_Args(sArgs),
  m_Returns() {

  return;
}

// ####################################################################### //
// # Nuclex::CCommand::~CCommand()                               Destructor # // 
// ####################################################################### //
/** Destroys an instance of CCommand. Clear() will automatically be
    called before the class is destroyed.
*/
CCommand::~CCommand(void) {
  clearCommand();

  return;
}

// ####################################################################### //
// # Nuclex::CCommand::clearCommand()                                       # // 
// ####################################################################### //
/** Removes all input and output arguments from the command and clears
    its command name. The command will be in the same state as a newly
    constructed command object.
*/
void CCommand::clearCommand(void) {
  m_sName = "";
  m_Args.clearArguments();
  m_Returns.clearArguments();

  return;
}

// ####################################################################### //
// # Nuclex::CCommand::getName()                                            # // 
// ####################################################################### //
/** Retrieves the name of the method to be called.

    @return String containing the name of the method to call
*/
const String &CCommand::getName(void) const {
  return m_sName;
}

// ####################################################################### //
// # Nuclex::CCommand::setName()                                            # // 
// ####################################################################### //
/** Sets the name of the method that is to be called by the command
    object. This name should be a string representation of the original
    method's name.    

    @param  sName  Name of the method to execute
*/
void CCommand::setName(const String &sName) {
  m_sName = sName;

  return;
}

// ####################################################################### //
// # Nuclex::CCommand::matchesDefinition()                                  # // 
// ####################################################################### //
/** Checks whether the command matches a description string. The string
    is formed as
      Name(t[,t[..]])
    where Name is the name of the command and t is one of
      b Bool
      n Number (int)
      v Value (double)
      s String
      B Blob
      O Object
      ? Any
      * Multiple

    Example:
      "Get()"
      "Set(v,v)"

    @param  sID  Name of the method to execute
    @todo Currently very strict, doesn't allow spaces. Rewrite.
    @todo Maybe add value access properties "X" instead "X()"
*/
bool CCommand::matchesDefinition(const String &sID) {
  unsigned int nPos = sID.find('(');
  if(nPos == std::string::npos)
    throw InvalidArgumentException("Nuclex::CCommand::SetCommand()",
                                   String("Invalid command identifier string: '" + sID + "'"));

  if(m_sName != sID.substr(0, nPos))
    return false;

  unsigned long nArg = 0;

  nPos++;
  while(nPos < sID.length()) {
    if(sID[nPos] == ')')
      break;

    else if(sID[nPos] == '.')
      return true;

    else if(m_Args.getNumArguments() <= nArg)
      return false;

    nArg++;
    nPos += 2;
  }

  if(m_Args.getNumArguments() > nArg)
    return false;

  return true;
}


// ####################################################################### //
// # Nuclex::CCommand::getArgs()                                            # // 
// ####################################################################### //
/** Retrieves the input argument list. This is the list of arguments that
    gets passed from the caller to the called method.

    @return The input argument list
*/
ArgumentList &CCommand::getArgs(void) {
  return m_Args;
}

// ####################################################################### //
// # Nuclex::CCommand::getArgs()                                            # // 
// ####################################################################### //
/** Retrieves the input argument list. This is the list of arguments that
    gets passed from the caller to the called method.

    @return The input argument list
*/
const ArgumentList &CCommand::getArgs(void) const {
  return m_Args;
}

// ####################################################################### //
// # Nuclex::CCommand::getReturns()                                         # // 
// ####################################################################### //
/** Retrieves the output argument list. This list is filled with the
    return values of the called method and can be used by the caller
    to obtain the method's results.

    @return The output argument list
*/
ArgumentList &CCommand::getReturns(void) {
  return m_Returns;
}

// ####################################################################### //
// # Nuclex::CCommand::getReturns()                                         # // 
// ####################################################################### //
/** Retrieves the output argument list. This list is filled with the
    return values of the called method and can be used by the caller
    to obtain the method's results.

    @return The output argument list
*/
const ArgumentList &CCommand::getReturns(void) const {
  return m_Returns;
}

// ####################################################################### //
// # Nuclex::CCommand::loadObject()                                         # // 
// ####################################################################### //
/** Load the command from a Serializer.

    @param  Serializer  Serializer from which to load the command
*/
void CCommand::loadObject(ISerializer &Serializer) {
  m_sName = Serializer.retrieveValue("Name");
  //m_Args = *dynamic_cast<ArgumentList *>(Serializer.retrieveValue("Arguments").getObject());
  //m_Returns = *dynamic_cast<ArgumentList *>(Serializer.retrieveValue("Returns").getObject());

  return;
}

// ####################################################################### //
// # Nuclex::CCommand::saveObject()                                         # // 
// ####################################################################### //
/** Saves the command into a Serializer.

    @param  Serializer  Serializer to use for saving the command
*/
void CCommand::saveObject(ISerializer &Serializer) const {
  Serializer.storeValue("Name", m_sName);
//  Serializer.storeValue("Arguments", Variant(&m_Args));

  return;
}
#endif