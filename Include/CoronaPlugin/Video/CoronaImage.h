//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## ImageServer.h - Image server                      //
// ### # #      ###                                                      //
// # ### #      ###  Implements an image loading server                  //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_CORONAIMAGE_H
#define NUCLEX_VIDEO_CORONAIMAGE_H

#include "CoronaPlugin/CoronaPlugin.h"
#include "Nuclex/Video/Image.h"
#include "Corona/Corona.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::CoronaImage                                          //
//  //
/// Corona Image
/** An image using the Corona library
*/
class CoronaImage :
  public Image {
  public:
    /// Constructor
    NUCLEXCORONA_API CoronaImage(corona::Image *pImage);
    /// Destructor
    NUCLEXCORONA_API virtual ~CoronaImage();

  //
  // Image implementation
  //
  public:
    /// Get image width
    NUCLEXCORONA_API const Point2<size_t> &getSize() const { return m_Size; }
    /// Get image color format
    NUCLEXCORONA_API PixelFormat getFormat() const;
    /// Copy image bits region
    NUCLEXCORONA_API void blitTo(
      const LockInfo &Destination,
      const Point2<long> &Position = Point2<long>(),
      const Box2<long> &SourceRegion = Box2<long>()
    ) const;

  //
  // Surface implementation
  //
  public:
    /// Lock surface region
    NUCLEXCORONA_API const LockInfo &lock(LockMode eLockMode, const Box2<long> &Region = Box2<long>());
    /// Unlock surface
    NUCLEXCORONA_API void unlock();

  private:
    const Point2<size_t>  m_Size;                     ///< Image size
    corona::Image        *m_pImage;                   ///< Corona image
    LockInfo              m_LockInfo;                 ///< Surface lock info
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_CORONAIMAGE_H
