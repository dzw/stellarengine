//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NuclexPython.cpp - Rar plugin implementation        //
// ### # #      ###                                                      //
// # ### #      ###  The plugin implementation for nuclex                //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "PythonPlugin/PythonPlugin.h"
#include "PythonPlugin/Script/PythonLanguage.h"
using namespace Nuclex;
using namespace Nuclex::Script;

PythonInitTerm _PythonInitTerm;

namespace PythonPlugin{

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXPYTHON_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXPYTHON_API*/ void nuclexPluginRegister() {
  Kernel::getInstance().getScriptServer()->addLanguage("Python", shared_ptr<Language>(new PythonLanguage()));
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXPYTHON_API*/ void nuclexPluginUnregister() {
  Kernel::getInstance().getScriptServer()->removeLanguage("Python");
}

void InitPlugin()
{
}

IMPLEMENTPACKAGE(PythonPlugin);

} // namespace
