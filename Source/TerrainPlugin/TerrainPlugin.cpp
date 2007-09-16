//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NCXImage.cpp - Nuclex image extension module        //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "TerrainPlugin/TerrainPlugin.h"
#include "TerrainPlugin/Scene/TerrainModelCodec.h"
#include "Nuclex/Scene/SceneServer.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::Scene;

namespace TerrainPlugin
{

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
extern "C" NUCLEXTERRAIN_API int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
extern "C" NUCLEXTERRAIN_API void nuclexPluginRegister(/*Kernel *pKernel*/) {
  Kernel *pKernel = &Kernel::getInstance();

  pKernel->getSceneServer()->addModelCodec(
    "Terrain", 
    shared_ptr<ModelCodec>(new TerrainModelCodec())
  );
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
extern "C" NUCLEXTERRAIN_API void nuclexPluginUnregister(/*Kernel *pKernel*/) {
}

void InitPlugin()
{
}

IMPLEMENTPACKAGE(TerrainPlugin);

}