//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Surface.cpp - Bitmap surface                                              //
// ### # #      ###                                                                            //
// # ### #      ###  A two-dimensional array of pixels which forms an image,                   //
// #  ## #   # ## ## supporting multiple pixel formats                                         //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/Surface.h"
#include "Nuclex/Video/Blit.h"
#include "Nuclex/Support/Exception.h"
#include "ScopeGuard/ScopeGuard.h"
#include <memory>

using namespace Nuclex;
using namespace Nuclex::Video;

namespace {

//  //
//  PerformBlit                                                                                //
//  //
/// Color format solver for blitting
struct PerformBlit {
  /// Blit source to destination with optional alpha blending
  /** With alpha blending: Blends source onto dest by source alpha
      Without alpha blending: Copies source to dest
  */
  inline void operator ()(
    void *pDestination, long nDestinationPitch, Surface::PixelFormat eDestinationFormat,
    const void *pSource, long nSourcePitch, Surface::PixelFormat eSourceFormat,
    const Point2<size_t> &Size
  ) {
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
        throw UnexpectedException("PerformBlit::operator()",
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
      inline void operator ()(
        void *pDestination, long nDestinationPitch,
        const void *pSource, long nSourcePitch, Surface::PixelFormat eSourceFormat,
        const Point2<unsigned long> &Size
      ) {
        switch(eSourceFormat) {
          case Surface::PF_A_8: {
            Blit<DestinationPixelFormatDescription, A_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_RGB_3_3_2: {
            Blit<DestinationPixelFormatDescription, RGB_3_3_2, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_RGB_5_6_5: {
            Blit<DestinationPixelFormatDescription, RGB_5_6_5, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_RGB_8_8_8: {
            Blit<DestinationPixelFormatDescription, RGB_8_8_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_XRGB_1_5_5_5: {
            Blit<DestinationPixelFormatDescription, XRGB_1_5_5_5, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_XRGB_8_8_8_8: {
            Blit<DestinationPixelFormatDescription, ARGB_8_8_8_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_ARGB_1_5_5_5: {
            Blit<DestinationPixelFormatDescription, ARGB_1_5_5_5, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_ARGB_4_4_4_4: {
            Blit<DestinationPixelFormatDescription, ARGB_4_4_4_4, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          case Surface::PF_ARGB_8_8_8_8: {
            Blit<DestinationPixelFormatDescription, ARGB_8_8_8_8, false>()(
              pDestination, nDestinationPitch, pSource, nSourcePitch, Size
            );
            break;
          }
          default:
            throw UnexpectedException("PerformBlit::To::operator()",
                                      "Invalid source pixel format");
        }
      }
    };
};

} // namespace

// ############################################################################################# //
// # Nuclex::Video::Surface::pixelFromColor()                                                  # //
// ############################################################################################# //
string Surface::stringFromPixelFormat(PixelFormat ePixelFormat) {
  switch(ePixelFormat) {
    case PF_A_8: return "A-8";
    case PF_RGB_3_3_2: return "RGB-3-3-2";
    case PF_RGB_5_6_5: return "RGB-5-6-5";
    case PF_RGB_8_8_8: return "RGB-8-8-8";
    case PF_XRGB_1_5_5_5: return "XRGB-1-5-5-5";
    case PF_XRGB_8_8_8_8: return "XRGB-8-8-8-8";
    case PF_ARGB_1_5_5_5: return "ARGB-1-5-5-5";
    case PF_ARGB_4_4_4_4: return "ARGB-4-4-4-4";
    case PF_ARGB_8_8_8_8: return "ARGB-8-8-8-8";
    default: return "Unknown";
  }
}

// ############################################################################################# //
// # Nuclex::Video::Surface::pixelFormatFromString()                                           # //
// ############################################################################################# //
Surface::PixelFormat Surface::pixelFormatFromString(const string &sPixelFormat) {
  if(sPixelFormat == "A-8")
    return PF_A_8;
  if(sPixelFormat == "RGB-3-3-2")
    return PF_RGB_3_3_2;
  if(sPixelFormat == "RGB-5-6-5")
    return PF_RGB_5_6_5;
  if(sPixelFormat == "RGB-8-8-8")
    return PF_RGB_8_8_8;
  if(sPixelFormat == "XRGB-1-5-5-5")
    return PF_XRGB_1_5_5_5;
  if(sPixelFormat == "XRGB-8-8-8-8")
    return PF_XRGB_8_8_8_8;
  if(sPixelFormat == "ARGB-1-5-5-5")
    return PF_ARGB_1_5_5_5;
  if(sPixelFormat == "ARGB-4-4-4-4")
    return PF_ARGB_4_4_4_4;
  if(sPixelFormat == "ARGB-8-8-8-8")
    return PF_ARGB_8_8_8_8;
  
  return PF_NONE;
}

// ############################################################################################# //
// # Nuclex::Video::Surface::pixelFromColor()                                                  # //
// ############################################################################################# //
/** Converts an RGBA color into a pixel color, whose accuracy is
    dependend of the pixel format specified.

    @param  Color        Color to be converted
    @param  eDestFormat  Destination pixel format
    @return The converted pixel color
*/
inline Surface::PixelType Surface::pixelFromColor(const Color &Color, PixelFormat eDestFormat) {
  switch(eDestFormat) {
    case Surface::PF_RGB_3_3_2:    return RGB_3_3_2::pixelFromColor(Color);
    case Surface::PF_RGB_5_6_5:    return RGB_5_6_5::pixelFromColor(Color);
    case Surface::PF_RGB_8_8_8:    return RGB_8_8_8::pixelFromColor(Color);
    case Surface::PF_XRGB_1_5_5_5: return XRGB_1_5_5_5::pixelFromColor(Color);
    case Surface::PF_XRGB_8_8_8_8: return XRGB_8_8_8_8::pixelFromColor(Color);
    case Surface::PF_ARGB_1_5_5_5: return ARGB_1_5_5_5::pixelFromColor(Color);
    case Surface::PF_ARGB_4_4_4_4: return ARGB_4_4_4_4::pixelFromColor(Color);
    case Surface::PF_ARGB_8_8_8_8: return ARGB_8_8_8_8::pixelFromColor(Color);
    default:                                    return 0;
  }
}

// ############################################################################################# //
// # Nuclex::Video::Surface::colorFromPixel()                                                  # //
// ############################################################################################# //
/** Converts a pixel color into an RGBA color, interpreting the pixel
    color using the specified pixel format.

    @param  nPixel         Pixel color to be converted
    @param  eSourceFormat  Format of the pixel color
    @return The converted color value
*/
inline Color Surface::colorFromPixel(PixelType nPixel, PixelFormat eSourceFormat) {
  switch(eSourceFormat) {
    case Surface::PF_RGB_3_3_2: return RGB_3_3_2::colorFromPixel(nPixel);
    case Surface::PF_RGB_5_6_5: return RGB_5_6_5::colorFromPixel(nPixel);
    case Surface::PF_RGB_8_8_8: return RGB_8_8_8::colorFromPixel(nPixel);
    case Surface::PF_XRGB_1_5_5_5: return XRGB_1_5_5_5::colorFromPixel(nPixel);
    case Surface::PF_XRGB_8_8_8_8: return XRGB_8_8_8_8::colorFromPixel(nPixel);
    case Surface::PF_ARGB_1_5_5_5: return ARGB_1_5_5_5::colorFromPixel(nPixel);
    case Surface::PF_ARGB_4_4_4_4: return ARGB_4_4_4_4::colorFromPixel(nPixel);
    case Surface::PF_ARGB_8_8_8_8: return ARGB_8_8_8_8::colorFromPixel(nPixel);
    default: return Color();
  }
}

// ############################################################################################# //
// # Nuclex::Video::Surface::blit()                                                            # //
// ############################################################################################# //
/** Blits a surface, optionally performing pixel format conversion

    @param  SourceSurface       from which to blit
    @param  DestinationSurface  Surface onto which to blit
    @param  Location            Destination position
    @param  SourceRegion        Region of source surface to copy
*/
void Surface::blit(const LockInfo &DestinationSurface, const LockInfo &SourceSurface,
                   const Point2<long> &Location,
                   const Box2<long> &SourceRegion) {
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
    SourceSurface.eFormat,
    Point2<size_t>(
      std::min<unsigned long>(
        DestinationSurface.Size.X - ClippedLocation.X, ClippedSourceRegion.getWidth()
      ),
      std::min<unsigned long>(
        DestinationSurface.Size.Y - ClippedLocation.Y, ClippedSourceRegion.getHeight()
      )
    )
  );
}

// ####################################################################### //
// # Nuclex::Video::CoronaImage::blitTo()                                # //
// ####################################################################### //
/** Copies the image bits onto the given memory adress

    @param  DestinationSurface  Destination surface
    @param  Location            Target position on destination surface
    @param  SourceRegion        Region to copy
*/
void Surface::blitTo(const LockInfo &DestinationSurface, const Point2<long> &Location,
                     const Box2<long> &SourceRegion) {
  // Get a valid source region
  Box2<long> ClippedSourceRegion = !SourceRegion ?
    Box2<long>(Point2<long>(), getSize()) :
    Box2<long>::getIntersection(SourceRegion, Box2<long>(Point2<long>(), getSize()));

  LockInfo SourceSurface = lock(LM_READ, ClippedSourceRegion);
  { ScopeGuard unlock_SourceSurface = MakeObjGuard(*this, &Surface::unlock);

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
      SourceSurface.eFormat,
      Point2<size_t>(
        std::min<unsigned long>(
          DestinationSurface.Size.X - ClippedLocation.X, ClippedSourceRegion.getWidth()
        ),
        std::min<unsigned long>(
          DestinationSurface.Size.Y - ClippedLocation.Y, ClippedSourceRegion.getHeight()
        )
      )
    );
  }
}

// ############################################################################################# //
// # Nuclex::Video::Image::blitTo()                                                            # //
// ############################################################################################# //
/** Copies the image bits onto the given surface

    @param  spDestSurface  Destination surface
    @param  Position       Target position on destination surface
    @param  SourceRegion   Region to copy
*/
void Surface::blitTo(const shared_ptr<Surface> &spDestSurface,
                     const Point2<long> &Position,
                     const Box2<long> &SourceRegion) {
  const Surface::LockInfo &LockedSurface = spDestSurface->lock(LM_WRITE);
  { ScopeGuard unlock_Surface = MakeObjGuard(*spDestSurface.get(), &Surface::unlock);
  
    blitTo(LockedSurface, Position, SourceRegion);
  }
}
