//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9PixelShader.cpp - PixelShader              //
// ### # #      ###                                                      //
// # ### #      ###  PixelShader class                                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9PixelShader.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::Direct3D9PixelShader::Direct3D9PixelShader()                   Constructor # //
// ############################################################################################# //
/** Initializes an instance of Direct3D9PixelShader
 
    @param  pPixelShader  The Direct3D PixelShader
*/
Direct3D9PixelShader::Direct3D9PixelShader(Direct3D9VideoDevice &VideoDevice,
                                           const void *pnFunction) {
  D3DCheck(
    "Nuclex::Video::Direct3D9PixelShader::Direct3D9PixelShader()",
    "IDirect3DDevice9::CreatePixelShader()",
    VideoDevice.getDirect3DDevice()->CreatePixelShader(
      reinterpret_cast<const DWORD *>(pnFunction), &m_spPixelShader
    )
  );
}
