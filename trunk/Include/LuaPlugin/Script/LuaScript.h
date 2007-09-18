//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## LuaScript.h - Nuclex Lua script                     //
// ### # #      ###                                                      //
// # ### #      ###                                                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_LUASCRIPT_H
#define NUCLEX_SCRIPT_LUASCRIPT_H

#include "LuaPlugin/LuaPlugin.h"
#include "Nuclex/Script/Script.h"
#include "Nuclex/Support/String.h"

extern "C" {
  #include "Lua/Lua.h"
}

namespace Nuclex { namespace Script {

//  //
//  Nuclex::LuaScript                                                    //
//  //
/// Lua script
/** A script in the Lua scripting language
*/
class LuaScript :
  public Script {
  public:
    /// Constructor
    NUCLEXLUA_API LuaScript(const string &sScript = "");
    /// Destructor
    NUCLEXLUA_API virtual ~LuaScript();

  //
  // LuaScript implementation
  //
  public:

  //
  // Script implementation
  //
  public:
    /// Parse script
    NUCLEXLUA_API void parseScript(const string &sScript);
    /// Call procedure
    NUCLEXLUA_API Variant callProcedure(const string &sProc, const ArgumentList &Args);
    /// Execute command
    NUCLEXLUA_API Variant executeCommand(const string &sCommand = "");

  private:
    lua_State *m_pLuaState;
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_LUASCRIPT_H
