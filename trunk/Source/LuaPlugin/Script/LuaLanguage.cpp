//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## LuaLanguage.cpp - Nuclex Lua language               //
// ### # #      ###                                                      //
// # ### #      ###  Lua scripting language for nuclex                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "LuaPlugin/Script/LuaLanguage.h"
#include "LuaPlugin/Script/LuaScript.h"

using namespace Nuclex;
using namespace Nuclex::Script;

// ####################################################################### //
// # Nuclex::LuaLanguage::LuaLanguage()                      Constructor # // 
// ####################################################################### //
/** Initializes an instance of CLuaLanguage
*/
LuaLanguage::LuaLanguage() {
  return;
}

// ####################################################################### //
// # Nuclex::LuaLanguage::~LuaLanguage()                      Destructor # // 
// ####################################################################### //
/** Destroys an instance of CLuaLanguage
*/
LuaLanguage::~LuaLanguage() {
  return;
}

// ####################################################################### //
// # Nuclex::LuaLanguage::createScript()                                 # // 
// ####################################################################### //
/** Creates a new script of the implemented language

    @param  sScript  Initial script contents
*/
shared_ptr<Nuclex::Script::Script> LuaLanguage::createScript(const string &sScript) {
  return shared_ptr<Nuclex::Script::Script>(new LuaScript(sScript));
}
