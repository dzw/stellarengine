//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## ScriptServer.cpp - Scripting server                 //
// ### # #      ###                                                      //
// # ### #      ###  Serves all scripting capabilities of flex           //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Script/ScriptServer.h"
#include "Nuclex/Script/Scriptlet.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::Script;

// ####################################################################### //
// # Nuclex::ScriptServer::ScriptServer()                       Constructor # // 
// ####################################################################### //
/** Creates an instance of ScriptServer
*/
ScriptServer::ScriptServer() {
  return;
}

// ####################################################################### //
// # Nuclex::ScriptServer::~ScriptServer()                       Destructor # // 
// ####################################################################### //
/** Destroys an instance of ScriptServer. 
*/
ScriptServer::~ScriptServer(void) {
  return;
}

// ####################################################################### //
// # Nuclex::ScriptServer::getLanguage()                                    # // 
// ####################################################################### //
/** Retrieves a Language class by name

    @param  sName     Name of the Language class to retrieve
    @return Language class or NULL if not found
*/
const shared_ptr<Language> &ScriptServer::getLanguage(const string &sName) const {
  LanguageMap::const_iterator LanguageIt = m_Languages.find(sName);
  if(LanguageIt == m_Languages.end())
    throw InvalidArgumentException("Nuclex::ScriptServer::getLanguage()",
                                   string("Item not found: '") + sName + "'");

  return LanguageIt->second;
}

// ####################################################################### //
// # Nuclex::ScriptServer::addLanguage()                                    # // 
// ####################################################################### //
/** Add a new Language class. Calls to the NewScript() method can use
    the Language name to specify which Language class to use to create
    the new Script.

    @param  sName       Name of the Language class
    @param  spLanguage  Pointer to a Language object
*/
void ScriptServer::addLanguage(const string &sName, const shared_ptr<Language> &spLanguage) {
  m_Languages.insert(LanguageMap::value_type(sName, spLanguage));

  return;
}

// ####################################################################### //
// # Nuclex::ScriptServer::removeLanguage()                                 # // 
// ####################################################################### //
/** Removes a Language class previously added using AddLanguage()

    @param  sName  Name of the Language class to remove
*/
void ScriptServer::removeLanguage(const string &sName) {
  LanguageMap::iterator LanguageIt = m_Languages.find(sName);
  if(LanguageIt == m_Languages.end()) {
    //m_pKernel->emitMessage(CKernel::MESSAGETYPE::MT_WARNING,
    //                       string("Tried to remove nonexisting scripting language '") + sName + "'");
    return;
  }

  m_Languages.erase(LanguageIt);

  return;
}

// ####################################################################### //
// # Nuclex::ScriptServer::createScript()                                   # // 
// ####################################################################### //
/** Create a new Script. The string is made up of a pair describing
    which Language class to use to open which Script, seperated by ':'.
    If no Language name is specified, the file system will be accessed.

    @param  sScript  Language class and Script seperated by ':'
    @return A new Script object
*/
shared_ptr<Nuclex::Script::Script> ScriptServer::createScript(const string &sScript) const {
/*
  IScript *pScript;
  char    *pszTemp;

  // Look for the language class delimiter
  string::size_type nPos = sScript.find(':');

  // Create temporary string to modify
  try {
    pszTemp = new char[sScript.length() + 1];
    if(!pszTemp)
      throw std::bad_alloc();
  }
  catch(const std::bad_alloc &) {
    throw OutOfMemoryException("Nuclex::ScriptServer::createScript()",
                               "Out of memory!");
  }
  try {
    ::strcpy(pszTemp, sScript.c_str());

    // Look up the language class
    if(nPos != string::npos)
      pszTemp[nPos] = 0;

    LanguageMap::const_iterator LanguageIt = m_Languages.find(pszTemp);
    if(LanguageIt == m_Languages.end())
      throw InvalidArgumentException("Nuclex::ScriptServer::createScript()", 
                                     string("Unknown language class: '") + pszTemp + "'");

    ILanguage *pLanguage = LanguageIt->second;

    // Use the Language class to create a new Script
    if(nPos == string::npos)
      pScript = pLanguage->createScript();
    else
      pScript = pLanguage->createScript(&pszTemp[nPos + 1]);
  }
  catch(const Exception &) {
    delete pszTemp;
    throw;
  }
  delete pszTemp;

  return pScript;
*/
  throw 1;
}

// ####################################################################### //
// # Nuclex::ScriptServer::loadScript()                                     # // 
// ####################################################################### //
/** Create a new Script. The string is made up of a pair describing
    which Language class to use to open which Script, seperated by ':'.
    If no Language name is specified, the file system will be accessed.

    @param  sSource  Source storage from which to load the script
    @return A new Script object
    @todo Not implemented yet
*/
shared_ptr<Nuclex::Script::Script> ScriptServer::loadScript(const string &sSource) const {
  throw NotSupportedException("Nuclex::ScriptServer::loadScript()",
                              "Not implemented yet");
}

// ####################################################################### //
// # Nuclex::ScriptServer::Tick()                                           # //
// ####################################################################### //
/** Regularly call to update all subsystems, execute the message pump,
    etc.
*/
void ScriptServer::Tick() {
  return;
}
