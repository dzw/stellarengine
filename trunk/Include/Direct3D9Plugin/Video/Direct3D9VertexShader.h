//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9VertexShader.h - VertexShader              //
// ### # #      ###                                                      //
// # ### #      ###  VertexShader class                                  //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_DIRECT3D9VERTEXSHADER_H
#define NUCLEX_VIDEO_DIRECT3D9VERTEXSHADER_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Nuclex/Video/VertexShader.h"

namespace Nuclex { namespace Video {

class Direct3D9VideoDevice;

//  //
//  Nuclex::Video::Direct3D9VertexShader                                 //
//  //
/// Direct3D9 VertexShader
/**
*/
class Direct3D9VertexShader :
  public VertexShader {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9VertexShader(Direct3D9VideoDevice &VideoDevice,
                                              const void *pnFunction);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9VertexShader() {}

  //
  // Direct3D9VertexShader implementation
  //
  public:
    /// Retrieve Direct3D vertex shader
    NUCLEXDIRECT3D9_API const IDirect3DVertexShader9Ptr &getDirect3DVertexShader() const {
      return m_spVertexShader;
    }

  //
  // VertexShader implementation
  //
  public:

  private:
    void onDeviceShutodown(bool);

    IDirect3DVertexShader9Ptr m_spVertexShader;       ///< Direct3D Vertex shader
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9VERTEXSHADER_H
