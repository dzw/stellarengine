//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NCXDirect3D9.cpp - Nuclex Direct3D9 extension       //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R8             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDriver.h"
#include "Direct3D9Plugin/Scene/XModelCodec.h"
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Scene/SceneServer.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Scene;

static Direct3D9InitTerm _Direct3D9InitTerm;


// ####################################################################### //
// # Nuclex::Video::getDirect3D9()                                       # //
// ####################################################################### //
NUCLEXDIRECT3D9_API IDirect3D9 *Nuclex::Video::getDirect3D9() { 
  return g_pDirect3D9;
}


namespace Direct3D9Plugin
{

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXDIRECT3D9_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXDIRECT3D9_API*/ void nuclexPluginRegister() {
  Kernel *pKernel = &Kernel::getInstance();
  const shared_ptr<VideoServer> &spVideoServer = pKernel->getVideoServer();

  IDirect3D9 *pDirect3D9 = getDirect3D9();
  if(pDirect3D9) {
    unsigned long nAdapters = pDirect3D9->GetAdapterCount();

    for(unsigned long nAdapter = 0; nAdapter < nAdapters; nAdapter++) {
      shared_ptr<Direct3D9VideoDriver> spD3D9VD(new Direct3D9VideoDriver(pKernel, nAdapter));
      spVideoServer->addDriver(spD3D9VD->getName(), spD3D9VD);
    }
  }
  
  pKernel->getSceneServer()->addModelCodec(
    "X", 
    shared_ptr<ModelCodec>(new XModelCodec(pKernel->getVideoServer()))
  );
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXDIRECT3D9_API*/ void nuclexPluginUnregister() {
  //pKernel->getSceneServer()->removeModelCodec("X");
}

void InitPlugin()
{}

IMPLEMENTPACKAGE(Direct3D9Plugin);
}