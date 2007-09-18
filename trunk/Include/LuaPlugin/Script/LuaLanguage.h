//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## LuaLanguage.h - Nuclex Lua language                 //
// ### # #      ###                                                      //
// # ### #      ###  Implements a flex scriting language using the       //
// #  ## #   # ## ## Lua interpreter                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_LUALANGUAGE_H
#define NUCLEX_SCRIPT_LUALANGUAGE_H

#include "LuaPlugin/LuaPlugin.h"
#include "Nuclex/Script/Language.h"

namespace Nuclex { namespace Script {

//  //
//  Nuclex::LuaLanguage                                                  //
//  //
/// Lua scripting language
/** Nuclex scripting language implementation for Lua
*/
class LuaLanguage :
  public Language {
  public:
    /// Constructor
    NUCLEXLUA_API LuaLanguage();
    /// Destructor
    NUCLEXLUA_API virtual ~LuaLanguage();

  //
  // Language implementation
  //
  public:
    // Script services
    //
    /// Create script
    NUCLEXLUA_API shared_ptr<Script> createScript(const string &sScript = "");
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_LUALANGUAGE_H
