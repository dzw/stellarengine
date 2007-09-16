//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## DirectInput8Plugin.cpp - DirectInput8 extension     //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R8             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "DirectInput8Plugin/DirectInput8Plugin.h"
#include "DirectInput8Plugin/Input/DirectInput8InputDevice.h"
#include "Nuclex/Input/InputServer.h"
//#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::Input;

//namespace {

IDirectInput8 *g_pDirectInput8 = NULL;
DirectInput8InitTerm _DirectInput8InitTerm;

// ############################################################################################# //
// # addInputDeviceToKernel()                                                                  # //
// ############################################################################################# //
BOOL CALLBACK addInputDeviceToKernel(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
  Kernel &TheKernel = *static_cast<Kernel *>(pvRef);
/*  
  shared_ptr<InputDevice> spDevice(
    new DirectInput8InputDevice(lpddi->guidInstance)
  );
  TheKernel.getInputServer()->addDevice(spDevice->getName(), spDevice);
*/  
  return true;
}

//}

// ############################################################################################# //
// # Nuclex::Input::getDirectInput8()                                                          # //
// ############################################################################################# //
NUCLEXDIRECTINPUT8_API IDirectInput8 *Nuclex::Input::getDirectInput8() { 
  return g_pDirectInput8;
}

namespace DirectInput8Plugin
{

// ############################################################################################# //
// # NuclexVersion()                                                                           # //
// ############################################################################################# //
/** Retrieve the version number of the nuclex framework the plugin
    was compiled against

    @return The framework version against which the plugin was compiled
*/
/*extern "C" NUCLEXDIRECTINPUT8_API*/ int nuclexPluginVersion() {
  return NUCLEX_VERSION;
}

// ############################################################################################# //
// # NuclexRegister()                                                                          # //
// ############################################################################################# //
/** Register the plugin to a nuclex kernel instance

    @param  pKernel  Kernel to register to
*/
/*extern "C" NUCLEXDIRECTINPUT8_API*/ void nuclexPluginRegister() {
  Kernel *pKernel = &Kernel::getInstance();
  HRESULT hResult = getDirectInput8()->EnumDevices(
    DI8DEVCLASS_ALL,
    addInputDeviceToKernel,
    pKernel,
    0
  );
  if(FAILED(hResult))
    Kernel::logMessage(
      Kernel::MT_WARNING,
      string("Device enumeration failed, cause: ") /*+ DXGetErrorString(hResult)*/
    );
}

// ############################################################################################# //
// # NuclexUnregister()                                                                        # //
// ############################################################################################# //
/** Unregister the plugin from a nuclex kernel instance

    @param  pKernel  Kernel to unregister from
*/
/*extern "C" NUCLEXDIRECTINPUT8_API*/ void nuclexPluginUnregister() {
}

void InitPlugin()
{
}

IMPLEMENTPACKAGE(DirectInput8Plugin);
}