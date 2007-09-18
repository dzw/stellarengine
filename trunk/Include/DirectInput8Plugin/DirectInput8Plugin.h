//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## DirectInput8Plugin.h - DirectInput extension        //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R8             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_DIRECTINPUT8PLUGIN_H
#define NUCLEX_DIRECTINPUT8PLUGIN_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Kernel.h"

#ifndef NUCLEX_WIN32
#error This plugin only works on the win32 platform
#endif

#include "Nuclex/Video/Surface.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Input/InputDevice.h"

// #include "MemoryTracker/DisableMemoryTracker.h"
#include <comdef.h>
#define DIRECTINPUT_VERSION 0x0800
#include "DirectX/DInput.h"
#include "DirectX/Dxerr.h"
// #include "MemoryTracker/MemoryTracker.h"

// Der folgende ifdef-Block zeigt die Standardlösung zur Erstellung von
// Makros, die das Exportieren aus einer DLL vereinfachen. Alle Dateien
// in dieser DLL wurden mit dem in der Befehlszeile definierten Symbol
// NUCLEX_EXPORTS kompiliert. Dieses Symbol sollte für kein Projekt definiert
// werden, das diese DLL verwendet. Auf diese Weise betrachtet jedes andere
// Projekt, dessen Quellcodedateien diese Datei einbeziehen, 
// NUCLEX_API-Funktionen als aus einer DLL importiert, während diese DLL mit
// diesem Makro definierte Symbole als exportiert betrachtet.
#ifdef DIRECTINPUT8PLUGIN_EXPORTS
  #define NUCLEXDIRECTINPUT8_API __declspec(dllexport)
#else
  #define NUCLEXDIRECTINPUT8_API __declspec(dllimport)
#endif

_COM_SMARTPTR_TYPEDEF(IDirectInputDevice8, IID_IDirectInputDevice8);
//_COM_SMARTPTR_TYPEDEF(ID3DXMesh, IID_ID3DXMesh);

namespace Nuclex { namespace Input {

NUCLEXDIRECTINPUT8_API IDirectInput8 *getDirectInput8();

// ####################################################################### //
// # Nuclex::Input::DInputCheck()                                        # //
// ####################################################################### //
/** Direct3D method call check. Converts errors to exceptions

    @param  pszSource  Source of the call
    @param  pszMethod  Method being called
    @param  hResult    Method result code
*/
static inline void DInputCheck(const char *pszSource, const char *pszMethod,
                               const HRESULT hResult) {
  if(FAILED(hResult))
    throw UnexpectedException(pszSource,
                              string("Unexpected failure in ") + pszMethod + ": " + DXGetErrorString(hResult));
}

// ####################################################################### //
// # Nuclex::Input::InputDeviceTypeFromDInputDevType()                   # //
// ####################################################################### //
static inline InputDevice::Type InputDeviceTypeFromDInputDevType(DWORD dwDevType) {
  switch(dwDevType) {
    case DI8DEVTYPE_DRIVING: return InputDevice::T_WHEEL;
    case DI8DEVTYPE_GAMEPAD: return InputDevice::T_GAMEPAD;
    case DI8DEVTYPE_JOYSTICK: return InputDevice::T_JOYSTICK;
    case DI8DEVTYPE_KEYBOARD: return InputDevice::T_KEYBOARD;
    case DI8DEVTYPE_MOUSE: return InputDevice::T_MOUSE;
    default: return InputDevice::T_UNKNOWN;
  }
}

}} // namespace Nuclex::Input;


using namespace Nuclex;
using namespace Nuclex::Input;

extern IDirectInput8 *g_pDirectInput8;

//  //
//  DirectInput8InitTerm                                                 //
//  //
/// DirectInput8 global initialization class
/** Initializes global DLL data. Currently includes loading D3D8.dll
    (which is required only once per process) and searching for the
    function pointer to create the main DirectInput8 interface.
*/
class DirectInput8InitTerm {
  public:
    /// Constructor
    /** Initializes global data when the dll enters a process
    */
    DirectInput8InitTerm() {
      HRESULT hResult = DirectInput8Create(
        GetModuleHandle(NULL),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        reinterpret_cast<void **>(&g_pDirectInput8),
        NULL
      );
      if(FAILED(hResult)) {
        Kernel::logMessage(
          Kernel::MT_WARNING,
          "DirectInput8 failed to initialize. DirectInput8 support disabled."
        );
        return;
      }

      Kernel::logMessage(
        Kernel::MT_INFO,
        "DirectInput8 is available."
      );
    }

    /// Destructor
    /** Destroys global data when the dll leaves a process
    */
    ~DirectInput8InitTerm() {
      if(g_pDirectInput8) {
        g_pDirectInput8->Release();
        g_pDirectInput8 = NULL;
      }
    }
} ;



DECLARATIONPACKAGE(DirectInput8Plugin);

#endif // NUCLEX_DIRECTINPUT8PLUGIN_H
