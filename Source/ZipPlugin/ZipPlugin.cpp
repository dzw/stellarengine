//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ZipPlugin.cpp - Zip archive reader plugin                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Provides the required dll function to register the                        //
// #  ## #   # ## ## zip archive reader plugin to nuclex                                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "ZipPlugin/ZipPlugin.h"
#include "ZipPlugin/Storage/ZipArchive.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;

namespace ZipPlugin
{

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXZIP_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXZIP_API*/ void nuclexPluginRegister() {
  Kernel::getInstance().getStorageServer()->addArchiveFactory(
    "Zip",
    shared_ptr<Storage::StorageServer::ArchiveFactory>(new Storage::ZipArchiveFactory())
  );
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXZIP_API*/ void nuclexPluginUnregister() {
  try {
    Kernel::getInstance().getStorageServer()->removeArchiveFactory("Zip");
  }
  catch(...) {
    Kernel::logMessage(
      Kernel::MT_ERROR,
      "An exception occured while removing the Zip codec"
    );
  }
}

void InitPlugin()
{
}

IMPLEMENTPACKAGE(ZipPlugin);

}