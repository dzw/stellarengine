//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NCXDirect3D9.cpp - Nuclex Direct3D9 extension       //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R8             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_DIRECT3D9_H
#define NUCLEX_DIRECT3D9_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Kernel.h"

#ifndef NUCLEX_WIN32
#error This plugin only works on the win32 platform
#endif

#include "Nuclex/Video/Surface.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/String.h"

// #include "MemoryTracker/DisableMemoryTracker.h"
#include <comdef.h>
#include "DirectX/D3D9.h"
#include "DirectX/D3DX9.h"
#include "DirectX/DXErr9.h"
// #include "MemoryTracker/MemoryTracker.h"

// Der folgende ifdef-Block zeigt die Standardlösung zur Erstellung von
// Makros, die das Exportieren aus einer DLL vereinfachen. Alle Dateien
// in dieser DLL wurden mit dem in der Befehlszeile definierten Symbol
// NUCLEX_EXPORTS kompiliert. Dieses Symbol sollte für kein Projekt definiert
// werden, das diese DLL verwendet. Auf diese Weise betrachtet jedes andere
// Projekt, dessen Quellcodedateien diese Datei einbeziehen, 
// NUCLEX_API-Funktionen als aus einer DLL importiert, während diese DLL mit
// diesem Makro definierte Symbole als exportiert betrachtet.
#ifdef DIRECT3D9PLUGIN_EXPORTS
  #define NUCLEXDIRECT3D9_API __declspec(dllexport)
#else
  #define NUCLEXDIRECT3D9_API __declspec(dllimport)
#endif

_COM_SMARTPTR_TYPEDEF(ID3DXBuffer, IID_ID3DXBuffer);
_COM_SMARTPTR_TYPEDEF(ID3DXMesh, IID_ID3DXMesh);

namespace Nuclex { namespace Video {

NUCLEXDIRECT3D9_API IDirect3D9 *getDirect3D9();

// ####################################################################### //
// # Nuclex::Video::D3DFORMATFromPixelFormat()                           # //
// ####################################################################### //
/** Converts a nuclex pixel format into a direct3d pixel format

    @param  eFormat  Format to convert
    @return The Direct3D pixel format
*/
static inline D3DFORMAT D3DFORMATFromPixelFormat(Surface::PixelFormat eFormat) {
  switch(eFormat) {
    case Surface::PF_A_8: return D3DFMT_A8;
    case Surface::PF_RGB_3_3_2: return D3DFMT_R3G3B2;
    case Surface::PF_RGB_5_6_5: return D3DFMT_R5G6B5;
    case Surface::PF_RGB_8_8_8: return D3DFMT_R8G8B8;
    case Surface::PF_XRGB_1_5_5_5: return D3DFMT_X1R5G5B5;
    case Surface::PF_XRGB_8_8_8_8: return D3DFMT_X8R8G8B8;
    case Surface::PF_ARGB_1_5_5_5: return D3DFMT_A1R5G5B5;
    case Surface::PF_ARGB_4_4_4_4: return D3DFMT_A4R4G4B4;
    case Surface::PF_ARGB_8_8_8_8: return D3DFMT_A8R8G8B8;
    default: return D3DFMT_UNKNOWN;
  }
}

// ####################################################################### //
// # Nuclex::Video::PixelFormatFromD3DFORMAT()                           # //
// ####################################################################### //
/** Converts a direct3d pixel format into a nuclex pixel format

    @param  eFormat  Format to convert
    @return The nuclex pixel format
*/
static inline Surface::PixelFormat PixelFormatFromD3DFORMAT(D3DFORMAT eFormat) {
  switch(eFormat) {
    case D3DFMT_A8: return Surface::PF_A_8;
    case D3DFMT_R3G3B2: return Surface::PF_RGB_3_3_2;
    case D3DFMT_R5G6B5: return Surface::PF_RGB_5_6_5;
    case D3DFMT_R8G8B8: return Surface::PF_RGB_8_8_8;
    case D3DFMT_X1R5G5B5: return Surface::PF_XRGB_1_5_5_5;
    case D3DFMT_X8R8G8B8: return Surface::PF_XRGB_8_8_8_8;
    case D3DFMT_A1R5G5B5: return Surface::PF_ARGB_1_5_5_5;
    case D3DFMT_A4R4G4B4: return Surface::PF_ARGB_4_4_4_4;
    case D3DFMT_A8R8G8B8: return Surface::PF_ARGB_8_8_8_8;
    default: return Surface::PF_NONE;
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DCheck()                                           # //
// ####################################################################### //
/** Direct3D method call check. Converts errors to exceptions

    @param  pszSource  Source of the call
    @param  pszMethod  Method being called
    @param  hResult    Method result code
*/
static inline void D3DCheck(const char *pszSource, const char *pszMethod,
                            const HRESULT hResult) {
  if(FAILED(hResult))
    throw UnexpectedException(
      pszSource,
      string("Unexpected failure in ") + pszMethod + ": " + DXGetErrorString9(hResult)
    );
}



}} // namespace Nuclex::Video;


using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Scene;

static IDirect3D9 *g_pDirect3D9 = NULL;

//  //
//  Direct3D9InitTerm                                                    //
//  //
/// Direct3D9 global initialization class
/** Initializes global DLL data. Currently includes loading D3D8.dll
    (which is required only once per process) and searching for the
    function pointer to create the main Direct3D9 interface.
*/
class Direct3D9InitTerm {
  public:
    /// Constructor
    /** Initializes global data when the dll enters a process
    */
    Direct3D9InitTerm() :
      m_hD3D9Dll(NULL) {
      typedef IDirect3D9 *WINAPI fnDirect3DCreate9(UINT SDKVersion);

      // Try to load D3D9.dll to get access to the creation method
      m_hD3D9Dll = ::LoadLibrary("D3D9");
      if(!m_hD3D9Dll) {
        Kernel::logMessage(
          Kernel::MT_WARNING,
          "D3D9.dll could not be loaded. Direct3D9 support disabled."
        );

        return;
      }
      
      { ScopeGuard Free_hD3D9Dll = MakeGuard(FreeLibrary, m_hD3D9Dll);

        // Get the function pointer to the Direct3DCreate9() procedure
        fnDirect3DCreate9 *pfnDirect3DCreate9 = reinterpret_cast<fnDirect3DCreate9 *>(
          ::GetProcAddress(m_hD3D9Dll, "Direct3DCreate9")
        );
        if(!pfnDirect3DCreate9) {
          Kernel::logMessage(
            Kernel::MT_WARNING,
            "Direct3DCreate9() procedure not found in D3D9.dll. Direct3D9 support disabled."
          );

          return;
        }

        g_pDirect3D9 = pfnDirect3DCreate9(D3D_SDK_VERSION);
        if(!g_pDirect3D9) {
          Kernel::logMessage(
            Kernel::MT_WARNING,
            "Direct3DCreate9() failed. Direct3D9 support disabled."
          );

          return;
        }
        
        Free_hD3D9Dll.Dismiss();
      }

      Kernel::logMessage(
        Kernel::MT_INFO,
        "Direct3D9 available and successfully initialized."
      );
    }

    /// Destructor
    /** Destroys global data when the dll leaves a process
    */
    ~Direct3D9InitTerm() {
      if(m_hD3D9Dll) {
        if(g_pDirect3D9) {
          // TODO: Releasing D3D here causes an access violation. Probably the destruction time
          //       of the initterm class is too late to do this, construct a dynamic initializer.
          //g_pDirect3D9->Release();
          g_pDirect3D9 = NULL;
        }

        ::FreeLibrary(m_hD3D9Dll);
        m_hD3D9Dll = NULL;
      }
    }

  private:
    HMODULE m_hD3D9Dll;                               ///< The d3d9.dll handle
} ;


DECLARATIONPACKAGE(Direct3D9Plugin);

#endif // NUCLEX_DIRECT3D9_H
