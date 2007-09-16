//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NCXAudiere.cpp - Nuclex Audiere extension module    //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "AudierePlugin/AudierePlugin.h"
#include "AudierePlugin/Audio/AudiereAudioDriver.h"
#include "Nuclex/Audio/AudioServer.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Support/Thread.h"
#include "Audiere/Audiere.h"

using namespace Nuclex;
using namespace Nuclex::Audio;

namespace AudierePlugin {

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXAUDIERE_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXAUDIERE_API*/ void nuclexPluginRegister() {
  std::vector<audiere::AudioDeviceDesc> AudioDevices;
  audiere::GetSupportedAudioDevices(AudioDevices);
  
  for(std::vector<audiere::AudioDeviceDesc>::iterator i = AudioDevices.begin();
      i != AudioDevices.end();
      ++i) {
    shared_ptr<AudiereAudioDriver> spDriver(new AudiereAudioDriver(i->name, i->description));
    Kernel::getInstance().getAudioServer()->addDriver(spDriver->getName(), spDriver);
  }
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXAUDIERE_API*/ void nuclexPluginUnregister() {}

void InitPlugin(){}

IMPLEMENTPACKAGE(AudierePlugin);

}

// ############################################################################################# //
// # getAudiereMutex()                                                                         # //
// ############################################################################################# //
NUCLEXAUDIERE_API Mutex &getAudiereMutex() {
  static Mutex AudiereMutex;
  return AudiereMutex;
}

