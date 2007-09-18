//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexShader.h - GPU vertex processor                                     //
// ### # #      ###                                                                            //
// # ### #      ###  A small program for the vertex processor which can modify                 //
// #  ## #   # ## ## vertices immediately before rasterization                                 //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEXSHADER_H
#define NUCLEX_VIDEO_VERTEXSHADER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/VertexDeclaration.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::VertexShader                                                                //
//  //
/// Vertex shader
/** Small program used by the graphics adapter or by the cpu to
    perform additional processing of a vertex buffer's contents
    immediately before rendering
*/
class VertexShader {
  public:
    /// Destructor
    NUCLEX_API virtual ~VertexShader() {}

  //
  // VertexShader implementation
  //
  public:
    // :)
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VERTEXSHADER_H
