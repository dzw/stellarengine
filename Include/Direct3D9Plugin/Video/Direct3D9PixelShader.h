//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9PixelShader.h - PixelShader                //
// ### # #      ###                                                      //
// # ### #      ###  PixelShader class                                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_DIRECT3D9PIXELSHADER_H
#define NUCLEX_VIDEO_DIRECT3D9PIXELSHADER_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Nuclex/Video/PixelShader.h"

namespace Nuclex { namespace Video {

class Direct3D9VideoDevice;

//  //
//  Nuclex::Video::Direct3D9PixelShader                                  //
//  //
/// Direct3D9 PixelShader
/**
*/
class Direct3D9PixelShader :
  public PixelShader {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9PixelShader(Direct3D9VideoDevice &VideoDevice,
                                             const void *pnFunction);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9PixelShader() {}

  //
  // Direct3D9PixelShader implementation
  //
  public:
    /// Retrieve Direct3D Pixel shader
    NUCLEXDIRECT3D9_API const IDirect3DPixelShader9Ptr &getDirect3DPixelShader() const {
      return m_spPixelShader;
    }

  //
  // PixelShader implementation
  //
  public:

  private:
    void onDeviceShutodown(bool);
  
    IDirect3DPixelShader9Ptr m_spPixelShader;         ///< Direct3D Pixel shader
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9PIXELSHADER_H
