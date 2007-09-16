//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## Image.cpp - Image                                   //
// ### # #      ###                                                      //
// # ### #      ###  Implements an image                                 //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "CoronaPlugin/Video/CoronaImage.h"
#include "Nuclex/Video/Surface.h"
#include "Nuclex/Video/PixelFormat.h"
#include "Nuclex/Video/Blit.h"
#include "Nuclex/Support/Exception.h"

// Corona's PixelFormat seems to describe the actual layout of the
// bits in memory, while nuclex simply allows the cpu to use any byte
// ordering enforced by the current platform. The latter is easier to
// handle in memory and does not require further precautions.
//
// Anyway, this means we have to flip the bytes in all of corona's
// pixel formats in order to keep the color channels in the right place.
//

using namespace Nuclex;
using namespace Nuclex::Video;

namespace {

typedef PixelFormatDescription<
  unsigned_32,         // 24 bits
  ColorChannel< 0, 8>, // ........ ........ rrrrrrrr
  ColorChannel< 8, 8>, // ........ gggggggg ........
  ColorChannel<16, 8>, // bbbbbbbb ........ ........
  NullChannel,
  24
> BGR_8_8_8;

typedef PixelFormatDescription<
  unsigned_32,         // 32 bits
  ColorChannel< 0, 8>, // ........ ........ ........ rrrrrrrr
  ColorChannel< 8, 8>, // ........ ........ gggggggg ........
  ColorChannel<16, 8>, // ........ bbbbbbbb ........ ........
  ColorChannel<24, 8>  // aaaaaaaa ........ ........ ........
> ABGR_8_8_8_8;

struct PerformBlit {
  /// Blit source to destination with optional alpha blending
  /** With alpha blending: Blends source onto dest by source alpha
      Without alpha blending: Copies source to dest
  */
  inline void operator ()(void *pDestination, long nDestinationPitch, Surface::PixelFormat eDestinationFormat,
                          const void *pSource, long nSourcePitch, corona::PixelFormat eSourceFormat,
                          const Point2<unsigned long> &Size) {
    switch(eDestinationFormat) {
      case Surface::PF_A_8: {
        To<A_8>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_RGB_3_3_2: {
        To<RGB_3_3_2>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_RGB_5_6_5: {
        To<RGB_5_6_5>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_RGB_8_8_8: {
        To<RGB_8_8_8>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_XRGB_1_5_5_5: {
        To<XRGB_1_5_5_5>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_XRGB_8_8_8_8: {
        To<XRGB_8_8_8_8>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_ARGB_1_5_5_5: {
        To<ARGB_1_5_5_5>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_ARGB_4_4_4_4: {
        To<ARGB_4_4_4_4>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      case Surface::PF_ARGB_8_8_8_8: {
        To<ARGB_8_8_8_8>()(
          pDestination, nDestinationPitch, pSource, nSourcePitch, eSourceFormat, Size
        );
        break;
      }
      default:
        throw UnexpectedException("Nuclex::Blit::operator()",
                                  "Invalid destination pixel format");
    }
  }
  
  private:
    template<
      typename DestinationPixelFormatDescription
    >
    struct To {
      /// Blit source to destination with optional alpha blending
      /** With alpha blending: Blends source onto dest by source alpha
          Without alpha blending: Copies source to dest
      */
      inline void operator ()(void *pDestination, long nDestinationPitch,
                              const void *pSource, long nSourcePitch, corona::PixelFormat eSourceFormat,
                              const Point2<unsigned long> &Size) {
        switch(eSourceFormat) {
          case corona::PF_B8G8R8A8: {
            Blit<DestinationPixelFormatDescription, ARGB_8_8_8_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case corona::PF_B8G8R8: {
            Blit<DestinationPixelFormatDescription, RGB_8_8_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case corona::PF_R8G8B8A8: {
            Blit<DestinationPixelFormatDescription, ABGR_8_8_8_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case corona::PF_R8G8B8: {
            Blit<DestinationPixelFormatDescription, BGR_8_8_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          default:
            throw UnexpectedException("Nuclex::To::operator()",
                                      "Invalid source pixel format");
        }
      }
    };
};

} // namespace

// ####################################################################### //
// # Nuclex::Video::CoronaImage::CoronaImage()               Constructor # //
// ####################################################################### //
/** Initializes an instance of CoronaImage

    @param  pVideo  Corona image to use
*/
CoronaImage::CoronaImage(corona::Image *pImage) :
  m_pImage(pImage),
  m_Size(pImage->getWidth(), pImage->getHeight()) {
  m_LockInfo.pMemory = NULL;
}

// ####################################################################### //
// # Nuclex::Video::CoronaImage::~CoronaImage()               Destructor # //
// ####################################################################### //
/** Destroys an instance of CoronaImage
*/
CoronaImage::~CoronaImage() {
  delete m_pImage;
}

// ####################################################################### //
// # Nuclex::Video::CoronaImage::getFormat()                             # //
// ####################################################################### //
/** Retrieves the image's color format

    @return Color format of the image
*/
Surface::PixelFormat CoronaImage::getFormat() const {
  return PixelFormatFromCoronaPixelFormat(m_pImage->getFormat());
}

// ####################################################################### //
// # Nuclex::Video::CoronaImage::blitTo()                                # //
// ####################################################################### //
/** Copies the image bits onto the given memory adress

    @param  DestinationSurface  Destination surface
    @param  Location            Target position on destination surface
    @param  SourceRegion        Region to copy
*/
void CoronaImage::blitTo(const LockInfo &DestinationSurface,
                         const Point2<long> &Location,
                         const Box2<long> &SourceRegion) const {
  LockInfo SourceSurface;

  SourceSurface.Size.set(m_pImage->getWidth(), m_pImage->getHeight());
  SourceSurface.eFormat = PixelFormatFromCoronaPixelFormat(m_pImage->getFormat());
  SourceSurface.nPitch = static_cast<long>(Surface::bppFromFormat(SourceSurface.eFormat) * SourceSurface.Size.X);
  SourceSurface.eMode = LM_READ;
  SourceSurface.pMemory = reinterpret_cast<unsigned char *>(m_pImage->getPixels());

  // Get a valid source region
  Box2<long> ClippedSourceRegion = !SourceRegion ?
    Box2<long>(Point2<long>(), SourceSurface.Size) :
    Box2<long>::getIntersection(SourceRegion, Box2<long>(Point2<long>(), SourceSurface.Size));

  Point2<long> ClippedLocation(
    (Location.X < 0) ? (ClippedSourceRegion.TL.X -= Location.X, 0) : Location.X,
    (Location.Y < 0) ? (ClippedSourceRegion.TL.Y -= Location.Y, 0) : Location.Y
  );

  PerformBlit()(
    static_cast<unsigned char *>(DestinationSurface.pMemory) +
      (ClippedLocation.Y * DestinationSurface.nPitch) +
      (ClippedLocation.X * Surface::bppFromFormat(DestinationSurface.eFormat)),
    DestinationSurface.nPitch,
    DestinationSurface.eFormat,
    static_cast<const unsigned char *>(SourceSurface.pMemory) +
      (ClippedSourceRegion.TL.Y * SourceSurface.nPitch) +
      (ClippedSourceRegion.TL.X * Surface::bppFromFormat(SourceSurface.eFormat)),
    SourceSurface.nPitch,
    m_pImage->getFormat(),
    Point2<size_t>(
      std::min<unsigned long>(DestinationSurface.Size.X - ClippedLocation.X, ClippedSourceRegion.getWidth()),
      std::min<unsigned long>(DestinationSurface.Size.Y - ClippedLocation.Y, ClippedSourceRegion.getHeight())
    )
  );
}

// ####################################################################### //
// # Nuclex::Video::CoronaImage::lock()                                  # //
// ####################################################################### //
/** Locks the specified region on the surface, returning a lockinfo
    structure containing data required to access the surface.

    @param  eLockMode  Lock access mode
    @param  Region     Region to lock
    @return A structure contaning data about the locked region
*/
const Surface::LockInfo &CoronaImage::lock(LockMode eLockMode, const Box2<long> &Region) {
  if(m_LockInfo.pMemory)
    throw FailedException("Nuclex::Video::CoronaImage::lock()",
                          "Tried to lock image multiple times");

  Box2<long> ClippedRegion;
  if(!Region)
    ClippedRegion.BR = m_Size;
  else if(!Region.intersects(Point2<long>(0, 0)) || !Region.intersects(m_Size))
    throw InvalidArgumentException("Nuclex::Video::CoronaImage::lock()",
                                   "The locking region must not leave the image area");

  m_LockInfo.Size = ClippedRegion.getSize();
  m_LockInfo.eFormat = PixelFormatFromCoronaPixelFormat(m_pImage->getFormat());
  m_LockInfo.nPitch = m_Size.X * Surface::bppFromFormat(m_LockInfo.eFormat);
  m_LockInfo.eMode = LM_READWRITE;
  m_LockInfo.pMemory = reinterpret_cast<unsigned char *>(m_pImage->getPixels()) -
                       (ClippedRegion.TL.Y * m_LockInfo.nPitch +
                        ClippedRegion.TL.X * Surface::bppFromFormat(m_LockInfo.eFormat));

  return m_LockInfo;
}

// ####################################################################### //
// # Nuclex::Video::CoronaImage::unlock()                                # //
// ####################################################################### //
/** Unlocks the surface again after it has been locked
*/
void CoronaImage::unlock() {
  if(!m_LockInfo.pMemory)
    throw FailedException(
      "Nuclex::Video::CoronaImage::unlock()", "Tried to unlock non-locked image"
    );

  m_LockInfo.pMemory = NULL;
}
