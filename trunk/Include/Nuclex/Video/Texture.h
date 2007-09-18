//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Texture.h - Surface for polygon rendering                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Special surface which is to be used for polygon rendering                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_IMAGE_TEXTURE_H
#define NUCLEX_IMAGE_TEXTURE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Surface.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Texture                                                                     //
//  //
/// Texture surface
/**
*/
class Texture :
  public Surface {
  public:
    /// Destructor
    NUCLEX_API virtual ~Texture() {}

  //
  // Texture implementation
  //
  public:
    /// Precache the texture
    /** Tries to precache the texture in the video card's memory.
        This method does not guarantee anything, but provides a hint
        at the renderer to prepare this texture for rendering asap
    */
    NUCLEX_API virtual void precache() = 0;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_TEXTURE_H
