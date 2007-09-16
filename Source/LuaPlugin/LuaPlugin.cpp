//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NuclexLua.cpp - Lua plugin implementation           //
// ### # #      ###                                                      //
// # ### #      ###  The plugin implementation for nuclex                //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "LuaPlugin/LuaPlugin.h"
#include "LuaPlugin/Script/LuaLanguage.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Script/ScriptServer.h"

using namespace Nuclex;
using namespace Nuclex::Script;

namespace LuaPlugin
{

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXLUA_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXLUA_API*/ void nuclexPluginRegister() {
  Kernel::getInstance().getScriptServer()->addLanguage("Lua", shared_ptr<Language>(new LuaLanguage()));
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXLUA_API*/ void nuclexPluginUnregister() {
  try {
    Kernel::getInstance().getScriptServer()->removeLanguage("Lua");
  }
  catch(...) {
    Kernel::logMessage(Kernel::MT_ERROR, "An exception occured while removing the lua language");
  }
}

void InitPlugin()
{
}

IMPLEMENTPACKAGE(LuaPlugin);
}