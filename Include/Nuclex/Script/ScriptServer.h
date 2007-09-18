//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## ScriptServer.h - Scripting server                   //
// ### # #      ###                                                      //
// # ### #      ###  Serves all scripting capabilities of flex           //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_SCRIPTSERVER_H
#define NUCLEX_SCRIPT_SCRIPTSERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

#include <map>

namespace Nuclex { namespace Script {

class Language;
class Script;

//  //
//  Nuclex::ScriptServer                                                 //
//  //
/// Nuclex script server
/** The script server is responsible for scripting languages and
    scripts thereof. 
*/
class ScriptServer {
  public:
    /// Constructor
    NUCLEX_API ScriptServer();
    /// Destructor
    NUCLEX_API virtual ~ScriptServer();

  //
  // ScriptServer implementation
  //
  public:
    // ----- Scripting language services -----
    //
    /// Lookup scripting language
    NUCLEX_API const shared_ptr<Language> &getLanguage(const string &sName) const;
    /// Add new scripting language
    NUCLEX_API void addLanguage(const string &sName, const shared_ptr<Language> &spLanguage);
    /// Remove scripting language
    NUCLEX_API void removeLanguage(const string &sName);

    // ----- Script services -----
    //
    /// Create new script
    NUCLEX_API shared_ptr<Script> createScript(const string &sScript) const;
    /// Load script from storage
    NUCLEX_API shared_ptr<Script> loadScript(const string &sSource) const;

    /// Update systems
    NUCLEX_API void Tick();

  private:
    /// Map of scripting languages
    typedef std::map<string, shared_ptr<Language> > LanguageMap;

    LanguageMap  m_Languages;                         ///< Scripting languages
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_SCRIPTSERVER_H
