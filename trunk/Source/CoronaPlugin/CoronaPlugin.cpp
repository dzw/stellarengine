//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NCXImage.cpp - Nuclex image extension module        //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "CoronaPlugin/CoronaPlugin.h"
#include "CoronaPlugin/Video/CoronaImageCodec.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Video/VideoServer.h"

using namespace Nuclex;

namespace CoronaPlugin {

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXCORONA_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXCORONA_API*/ void nuclexPluginRegister() {
  Kernel::getInstance().getVideoServer()->addImageCodec(
    "Corona",
    shared_ptr<Video::ImageCodec>(new Video::CoronaImageCodec()
  ));
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXCORONA_API*/ void nuclexPluginUnregister() {
  try {
    Kernel::getInstance().getVideoServer()->removeImageCodec("Corona");
  }
  catch(...) {
    Kernel::logMessage(
      Kernel::MT_ERROR,
      "An exception occured while removing the corona codec"
    );
  }
}

void InitPlugin(){}

IMPLEMENTPACKAGE(CoronaPlugin);
}
