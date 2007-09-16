//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RarPlugin.cpp - Rar archive reader plugin                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Provides the required dll function to register the                        //
// #  ## #   # ## ## rar archive reader plugin to nuclex                                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "RarPlugin/RarPlugin.h"
#include "RarPlugin/Storage/RarArchive.h"
#include "Nuclex/Storage/StorageServer.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::Storage;

namespace RarPlugin{
// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXRAR_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXRAR_API*/ void nuclexPluginRegister() {
  Kernel *pKernel = &Kernel::getInstance();

  pKernel->getStorageServer()->addArchiveFactory(
    "Rar",
    shared_ptr<StorageServer::ArchiveFactory>(new RarArchiveFactory())
  );
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXRAR_API*/ void nuclexPluginUnregister() {
  try {
    Kernel *pKernel = &Kernel::getInstance();

    pKernel->getStorageServer()->removeArchiveFactory("Rar");
  }
  catch(...) {
    Kernel::logMessage(
      Kernel::MT_ERROR,
      "An exception occured while removing the Rar codec"
    );
  }
}

void InitPlugin()
{
}

IMPLEMENTPACKAGE(RarPlugin);
}

// ############################################################################################# //
// # Nuclex::getRarMutex()                                                                     # //
// ############################################################################################# //
NUCLEXRAR_API Mutex &Nuclex::getRarMutex() {
  static Mutex RarMutex;
  return RarMutex;
}
