//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9VertexShader.cpp - VertexShader            //
// ### # #      ###                                                      //
// # ### #      ###  VertexShader class                                  //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9VertexShader.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VertexShader::Direct3D9VertexShader()                 Constructor # //
// ############################################################################################# //
/** Initializes an instance of Direct3D9VertexShader
 
    @param  pVertexShader  The Direct3D VertexShader
*/
Direct3D9VertexShader::Direct3D9VertexShader(Direct3D9VideoDevice &VideoDevice,
                                             const void *pnFunction) {
  D3DCheck(
    "Nuclex::Video::Direct3D9VertexShader::Direct3D9VertexShader()",
    "IDirect3DDevice9::CreateVertexShader()",
    VideoDevice.getDirect3DDevice()->CreateVertexShader(
      reinterpret_cast<const DWORD *>(pnFunction), &m_spVertexShader
    )
  );
}
