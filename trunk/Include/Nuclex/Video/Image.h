//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Image.h - Image                                                           //
// ### # #      ###                                                                            //
// # ### #      ###  An image stored as pixels                                                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_IMAGE_H
#define NUCLEX_VIDEO_IMAGE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Box2.h"
#include "Nuclex/Video/Surface.h"
#include <vector>

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Image                                                                       //
//  //
/// Raster image
/** Stores an image as pixels. Differs from Surface in that an image is expected to
    store its pixels in RAM and should be able to blit itself onto other surfaces
    without the requirement of locking the image bits.
*/
class Image :
  public Video::Surface {
  public:
    /// Destructor
    /** Destroys an instance of Image
    */
    NUCLEX_API virtual ~Image() {}

  //
  // Image implementation
  //
  public:
    /// Copy image bits region
    /** Copies a region of the image bits onto the given memory adress

        @param  Destination   Locked destination surface
        @param  Position      Target position on destination surface
        @param  SourceRegion  Region to copy
    */
    NUCLEX_API virtual void blitTo(
      const LockInfo &Destination,
      const Point2<long> &Position = Point2<long>(),
      const Box2<long> &SourceRegion = Box2<long>()
    ) const = 0;

    /// Copy image bits (conveniency method)
    NUCLEX_API virtual void blitTo(
      const shared_ptr<Surface> &spDestSurface,
      const Point2<long> &Position = Point2<long>(),
      const Box2<long> &SourceRegion = Box2<long>()
    ) const;
};

//  //
//  Nuclex::DefaultImage                                                                       //
//  //
/// Default bitmap implementation
/** Implements a bitmap with default behavior
*/
class DefaultImage :
  public Image {
  public:
    /// Constructor
    NUCLEX_API DefaultImage(const Point2<size_t> &Size, PixelFormat eFormat);
    /// Destructor
    NUCLEX_API virtual ~DefaultImage();

  //
  // Image implementation
  //
  public:
    /// Copy image bits region
    NUCLEX_API void blitTo(
      const LockInfo &Destination,
      const Point2<long> &Position = Point2<long>(),
      const Box2<long> &SourceRegion = Box2<long>()
    ) const;

  //
  // Surface implementation
  //
  public:
    /// Get image size
    NUCLEX_API const Point2<size_t> &getSize() const { return m_Size; }

    /// Get image color format
    NUCLEX_API PixelFormat getFormat() const { return m_eFormat; }

    /// Lock surface region
    NUCLEX_API const LockInfo &lock(
      LockMode eLockMode,
      const Box2<long> &Region = Box2<long>()
    );

    /// Unlock surface
    NUCLEX_API void unlock();

  private:
    Point2<size_t>                     m_Size;        ///< Image size
    PixelFormat                        m_eFormat;     ///< Color format
    mutable std::vector<unsigned char> m_Memory;      ///< Image memory
    LockInfo                           m_LockInfo;    ///< Current lock info
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_IMAGE_H
