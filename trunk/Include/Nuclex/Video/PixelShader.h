//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## PixelShader.h - GPU pixel shader                                          //
// ### # #      ###                                                                            //
// # ### #      ###  A small program for the polygon rasterizer which affects                  //
// #  ## #   # ## ## for pixels are colored                                                    //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_PIXELSHADER_H
#define NUCLEX_VIDEO_PIXELSHADER_H

#include "Nuclex/Nuclex.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::PixelShader                                                                 //
//  //
/// Pixel shader
/** A small program, running on the graphics adapter, to calculate
    the color of individual pixels on polygons being rendered.
*/
class PixelShader {
  public:
    /// Destructor
    NUCLEX_API virtual ~PixelShader() {}

  //
  // PixelShader implementation
  //
  public:
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_PIXELSHADER_H
