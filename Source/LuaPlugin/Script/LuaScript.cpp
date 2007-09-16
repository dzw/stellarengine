//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## LuaScript.cpp - Nuclex Lua script                   //
// ### # #      ###                                                      //
// # ### #      ###                                                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "LuaPlugin/Script/LuaScript.h"

using namespace Nuclex;
using namespace Nuclex::Script;

extern "C" {
  #include "Lua/LAuxLib.h"
}

namespace {

// ####################################################################### //
// # getLuaResult()                                                      # //
// ####################################################################### //
/** Retrieves the result of a function call of script execution from lua

    @param  pLuaState  State on which to operate
*/
inline Variant getLuaResult(lua_State *pLuaState) {
  switch(lua_type(pLuaState, -1)) {
    case LUA_TNONE: case LUA_TNIL: {
      return lua_pop(pLuaState, 1), Variant();
    }
      
    case LUA_TNUMBER: {
      Variant Result = lua_tonumber(pLuaState, -1);
      return lua_pop(pLuaState, 1), Result;
    }

    case LUA_TBOOLEAN: {
      Variant Result = (lua_toboolean(pLuaState, -1) != 0);
      return lua_pop(pLuaState, 1), Result;
    }
    
    case LUA_TSTRING: {
      Variant Result = string(lua_tostring(pLuaState, -1), lua_strlen(pLuaState, -1));
      return lua_pop(pLuaState, 1), Result;
    }
    
    case LUA_TTABLE: case LUA_TFUNCTION:
    case LUA_TUSERDATA: case LUA_TTHREAD: case LUA_TLIGHTUSERDATA: {
      Variant Result = string("<") + lua_typename(pLuaState, -1) + ">";
      return lua_pop(pLuaState, 1), Result;
    }
    
    default: {
      throw UnexpectedException("Nuclex::LuaScript::callProcedure()",
                                "The type returned by lua is not valid");
    }
  }
}

} // namespace

// ####################################################################### //
// # Nuclex::LuaScript::LuaScript()                          Constructor # // 
// ####################################################################### //
/** Initializes an instance of LuaScript
*/
LuaScript::LuaScript(const string &sScript) :
  m_pLuaState(lua_open()) {
  if(!m_pLuaState)
    throw UnexpectedException("Nuclex::LuaScript::LuaScript()",
                              "lua_open() failed unexpectedly. Too many scripts ?");
                              
  if(sScript.length() > 0)
    parseScript(sScript);
}

// ####################################################################### //
// # Nuclex::LuaScript::~LuaScript()                          Destructor # // 
// ####################################################################### //
/** Destroys an instance of LuaScript
*/
LuaScript::~LuaScript() {
  if(m_pLuaState)
    lua_close(m_pLuaState);
}

// ####################################################################### //
// # Nuclex::LuaScript::parseScript()                                    # // 
// ####################################################################### //
/** Parses a script

    @param  sScript  Script to parse
*/
void LuaScript::parseScript(const string &sScript) {
  luaL_loadbuffer(m_pLuaState, sScript.c_str(), sScript.length(), "Nuclex script");
  //lua_dobuffer(m_pLuaState, sScript.c_str(), sScript.length(), "Nuclex script");

}

// ####################################################################### //
// # Nuclex::LuaScript::callProcedure()                                  # // 
// ####################################################################### //
/** Calls a procedure in the script

    @param  sProc  Procedure to call
    @param  Args   Arguments to pass to the procedure
*/
Variant LuaScript::callProcedure(const string &sProc, const ArgumentList &Args) {
  lua_pushstring(m_pLuaState, sProc.c_str());
  lua_gettable(m_pLuaState, LUA_GLOBALSINDEX);

  for(unsigned long nArgument = 0; nArgument < Args.getNumArguments(); ++nArgument) {
    switch(Args.getArgument(nArgument).getType()) {
      case Variant::T_NONE: {
        lua_pushnil(m_pLuaState);
        break;
      }
      case Variant::T_BOOL: {
        lua_pushboolean(m_pLuaState, Args.getArgument(nArgument).to<bool>());
        break;
      }
      case Variant::T_INT:
      case Variant::T_SIZE:
      case Variant::T_DOUBLE: {
        lua_pushnumber(m_pLuaState, Args.getArgument(nArgument).to<double>());
        break;
      }
      case Variant::T_STRING: {
        lua_pushlstring(m_pLuaState, Args.getArgument(nArgument).to<string>().c_str(),
                                     Args.getArgument(nArgument).to<string>().length());
        break;
      }
    }
  }
  
  switch(lua_pcall(m_pLuaState, Args.getNumArguments(), 1, 0)) {
    case LUA_ERRRUN:
      throw UnexpectedException("Nuclex::LuaScript::callProcedure()",
                                "A runtime error occured in the script");
    case LUA_ERRMEM:
      throw UnexpectedException("Nuclex::LuaScript::callProcedure()",
                                "A memory allocation error occured in the script");
    case LUA_ERRERR:
      throw UnexpectedException("Nuclex::LuaScript::callProcedure()",
                                "An error occured within the script's error handler");
  }
  
  return getLuaResult(m_pLuaState);
}

// ####################################################################### //
// # Nuclex::LuaScript::executeCommand()                                 # // 
// ####################################################################### //
/** Executes a command in the script

    @param  sCommand  Command to execute
    @return The command's result
*/
Variant LuaScript::executeCommand(const string &sCommand) {
  luaL_dostring(m_pLuaState, sCommand.c_str());
  return getLuaResult(m_pLuaState);
}
