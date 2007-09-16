//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NuclexFreeType.cpp - FreeType plugin impl.          //
// ### # #      ###                                                      //
// # ### #      ###  The plugin implementation for nuclex                //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "FreeTypePlugin/FreeTypePlugin.h"
#include "FreeTypePlugin/Text/FreeTypeFontCodec.h"
#include "Nuclex/Text/TextServer.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;

FreeTypeInitTerm _FreeTypeInitTerm;

// ####################################################################### //
// # Nuclex::Text::getFreeTypeLibrary()                                  # //
// ####################################################################### //
NUCLEXFREETYPE_API FT_Library Nuclex::Text::getFreeTypeLibrary() {
  return _FreeTypeInitTerm.getFTLibrary();
}

// ####################################################################### //
// # Nuclex::Text::getFreeTypeMutex()                                    # //
// ####################################################################### //
NUCLEXFREETYPE_API Mutex &Nuclex::Text::getFreeTypeMutex() {
  static Mutex FreeTypeMutex;
  return FreeTypeMutex;
}

namespace FreeTypePlugin{
// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXFREETYPE_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXFREETYPE_API*/ void nuclexPluginRegister() {
  Kernel::getInstance().getTextServer()->addCodec(
    "FreeType",
    shared_ptr<Text::FontCodec>(new Text::FreeTypeFontCodec())
  );
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXFREETYPE_API*/ void nuclexPluginUnregister() {
  try {
    Kernel::getInstance().getTextServer()->removeCodec("FreeType");
  }
  catch(...) {
    Kernel::logMessage(Kernel::MT_ERROR, "An exception occured while removing the freetype codec");
  }
}

void InitPlugin()
{
}

IMPLEMENTPACKAGE(FreeTypePlugin);
} // namespace
