//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Blit.h - Software blitting routines                                       //
// ### # #      ###                                                                            //
// # ### #      ###  Blitting routines for copying or alpha-blending surfaces                  //
// #  ## #   # ## ## onto other surface                                                        //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_BLIT_H
#define NUCLEX_VIDEO_BLIT_H

#include "Nuclex/Video/PixelFormat.h"
#include "Nuclex/Math/Point2.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Blit                                                                        //
//  //
/// Blit memory block
/** Performs a blit, optionally including color conversion, alpha blending from
    alpha value and/or alpha channel. The best blitting solution is statically
    chosen by a compile-time solvable call graph of template method.
*/
template<
  typename DestinationPixelFormatDescription,
  typename SourcePixelFormatDescription,
  bool bAlphaBlend
>
struct Blit {
  /// Blit source to destination with optional alpha blending
  /** With alpha blending: Blends source onto dest by source alpha
      Without alpha blending: Copies source to dest
  */
  inline void operator ()(void *pDestination, long nDestinationPitch,
                          const void *pSource, long nSourcePitch,
                          const Point2<unsigned long> &Size) {
    blit<bAlphaBlend>(
      reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
      reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
      Size
    );
  }
  /// Alpha blit source to destination with optional alpha blending
  /** With alpha blending: Blends source onto dest by (source alpha x cAlpha)
      Without alpha blending: Copies source to dest, sets dest alpha to cAlpha
  */
  inline void operator ()(void *pDestination, long nDestinationPitch,
                          const void *pSource, long nSourcePitch,
                          const Point2<unsigned long> &Size, unsigned char cAlpha) {
    blit<bAlphaBlend>(
      reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
      reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
      Size, cAlpha
    );
  }
  
  private:
    static inline typename DestinationPixelFormatDescription::PixelType convertAdjustAlpha(
      typename SourcePixelFormatDescription::PixelType Pixel
    ) {
      return convertAdjustAlpha(
        Pixel, Bool2Type<
          (sizeof(SourcePixelFormatDescription::PixelType) > 
           sizeof(DestinationPixelFormatDescription::PixelType))
        >()
      );
    }
    // Required to let the ColorChannelConverter use the larger of two pixel types
    static inline typename DestinationPixelFormatDescription::PixelType convertAdjustAlpha(
      typename SourcePixelFormatDescription::PixelType Pixel, Bool2Type<true>
    ) {
      return static_cast<PixelType>(
        ColorChannelConverter<SourcePixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Red, SourcePixelFormatDescription::Red>()(Pixel) |
        ColorChannelConverter<SourcePixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Green, SourcePixelFormatDescription::Green>()(Pixel) |
        ColorChannelConverter<SourcePixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Blue, SourcePixelFormatDescription::Blue>()(Pixel) |
        ColorChannelConverter<SourcePixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Alpha, SourcePixelFormatDescription::Alpha>()(Pixel)
      );
    }
    static inline typename DestinationPixelFormatDescription::PixelType convertAdjustAlpha(
      typename SourcePixelFormatDescription::PixelType Pixel, Bool2Type<false>
    ) {
      return 
        ColorChannelConverter<DestinationPixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Red, SourcePixelFormatDescription::Red>()(Pixel) |
        ColorChannelConverter<DestinationPixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Green, SourcePixelFormatDescription::Green>()(Pixel) |
        ColorChannelConverter<DestinationPixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Blue, SourcePixelFormatDescription::Blue>()(Pixel) |
        ColorChannelConverter<DestinationPixelFormatDescription::PixelType,
          DestinationPixelFormatDescription::Alpha, SourcePixelFormatDescription::Alpha>()(Pixel);
    }

    template<bool bAlphaBlend>
    inline void blit(unsigned char *pDestination, long nDestinationPitch,
                     const unsigned char *pSource, long nSourcePitch,
                     const Point2<unsigned long> &Size);
    /// Alpha blended blit with optional pixel format conversion
    template<>
    inline void blit<true>(unsigned char *pDestination, long nDestinationPitch,
                           const unsigned char *pSource, long nSourcePitch,
                           const Point2<unsigned long> &Size) {
      // dest = source + (dest - source) * source.a
      // Uses non-alpha blit() if no alpha channel in source
      blend(
        pDestination, nDestinationPitch, 
        pSource, nSourcePitch,
        Size, Bool2Type<SourcePixelFormatDescription::HasAlphaChannel>()
      );
    }
    /// Blit with optional pixel format conversion
    template<>
    inline void blit<false>(unsigned char *pDestination, long nDestinationPitch,
                            const unsigned char *pSource, long nSourcePitch,
                            const Point2<unsigned long> &Size) {
      // dest = source
      // Uses memcpy() if both pixel formats are identical
      copy(
        pDestination, nDestinationPitch, 
        pSource, nSourcePitch,
        Size,
        DestinationPixelFormatDescription(), SourcePixelFormatDescription()
      );
    }

    template<bool bAlphablend>
    inline void blit(unsigned char *pDestination, long nDestinationPitch,
                     const unsigned char *pSource, long nSourcePitch,
                     const Point2<unsigned long> &Size, unsigned char cAlpha);

    /// Alpha blended alpha blit with optional pixel format conversion
    template<>
    inline void blit<true>(unsigned char *pDestination, long nDestinationPitch,
                           const unsigned char *pSource, long nSourcePitch,
                           const Point2<unsigned long> &Size, unsigned char cAlpha) {
      // dest = source + (dest - source) * (cAlpha * source.a)
      long nSourceLineAdd = nSourcePitch - (Size.X * SourcePixelFormatDescription::BytesPerPixel);
      long nDestinationLineAdd = nDestinationPitch -
        (Size.X * DestinationPixelFormatDescription::BytesPerPixel);
      
      for(unsigned long nY = 0; nY < Size.Y; ++nY) {
        for(unsigned long nX = 0; nX < Size.X; ++nX) {
          WritePixel<DestinationPixelFormatDescription>()(
            pDestination,
            AlphaBlend<
              DestinationPixelFormatDescription, SourcePixelFormatDescription,
              SourcePixelFormatDescription::Alpha::Bits::Count, 8
            >(ReadPixel<SourcePixelFormatDescription>()(pSource))(
              ReadPixel<DestinationPixelFormatDescription>()(pDestination),
              static_cast<unsigned char>(
                (ReadPixel<SourcePixelFormatDescription>()(pSource) &
                  SourcePixelFormatDescription::Alpha::Bits::Mask) >>
                  SourcePixelFormatDescription::Alpha::Bits::First
              ),
              cAlpha
            )
          );

          pSource += SourcePixelFormatDescription::BytesPerPixel;
          pDestination += DestinationPixelFormatDescription::BytesPerPixel;
        }

        pSource += nSourceLineAdd;
        pDestination += nDestinationLineAdd;
      }
    }
    /// Alpha blit with optional pixel format conversion
    template<>
    inline void blit<false>(unsigned char *pDestination, long nDestinationPitch,
                            const unsigned char *pSource, long nSourcePitch,
                            const Point2<unsigned long> &Size, unsigned char cAlpha) {
      // dest = source; dest.a = source.a * cAlpha
      long nSourceLineAdd = nSourcePitch - (Size.X * SourcePixelFormatDescription::BytesPerPixel);
      long nDestinationLineAdd = nDestinationPitch -
        (Size.X * DestinationPixelFormatDescription::BytesPerPixel);

      for(unsigned long nY = 0; nY < Size.Y; ++nY) {
        for(unsigned long nX = 0; nX < Size.X; ++nX) {
          DestinationPixelFormatDescription::PixelType Pixel =
            DestinationPixelFormatDescription::convertFrom<SourcePixelFormatDescription>(
            ReadPixel<SourcePixelFormatDescription>()(pSource)
          );
          NativeType Alpha = (Pixel & DestinationPixelFormatDescription::Alpha::Bits::Mask) >>
            DestinationPixelFormatDescription::Alpha::Bits::First;
          Pixel = static_cast<DestinationPixelFormatDescription::PixelType>(
            (Pixel & ~DestinationPixelFormatDescription::Alpha::Bits::Mask) | (Shift<
              NativeType, 8 - DestinationPixelFormatDescription::Alpha::Bits::First
            >()(Alpha * cAlpha) & DestinationPixelFormatDescription::Alpha::Bits::Mask)
          );

          WritePixel<DestinationPixelFormatDescription>()(
            pDestination,
            Pixel
          );

          pSource += SourcePixelFormatDescription::BytesPerPixel;
          pDestination += DestinationPixelFormatDescription::BytesPerPixel;
        }

        pSource += nSourceLineAdd;
        pDestination += nDestinationLineAdd;
      }
    }

    /// Alpha blended blit
    inline void blend(unsigned char *pDestination, long nDestinationPitch,
                      const unsigned char *pSource, long nSourcePitch,
                      const Point2<unsigned long> &Size, Bool2Type<true>) {
      long nSourceLineAdd = nSourcePitch - (Size.X * SourcePixelFormatDescription::BytesPerPixel);
      long nDestinationLineAdd = nDestinationPitch -
        (Size.X * DestinationPixelFormatDescription::BytesPerPixel);
      
      for(unsigned long nY = 0; nY < Size.Y; ++nY) {
        for(unsigned long nX = 0; nX < Size.X; ++nX) {
          WritePixel<DestinationPixelFormatDescription>()(
            pDestination,
            AlphaBlend<
              DestinationPixelFormatDescription, SourcePixelFormatDescription,
              SourcePixelFormatDescription::Alpha::Bits::Count
            >(ReadPixel<SourcePixelFormatDescription>()(pSource))(
              ReadPixel<DestinationPixelFormatDescription>()(pDestination),
              static_cast<unsigned char>(
                (ReadPixel<SourcePixelFormatDescription>()(pSource) &
                  SourcePixelFormatDescription::Alpha::Bits::Mask) >>
                  SourcePixelFormatDescription::Alpha::Bits::First
              )
            )
          );

          pSource += SourcePixelFormatDescription::BytesPerPixel;
          pDestination += DestinationPixelFormatDescription::BytesPerPixel;
        }

        pSource += nSourceLineAdd;
        pDestination += nDestinationLineAdd;
      }
    }    
    /// Alpha blended blit
    inline void blend(unsigned char *pDestination, long nDestinationPitch,
                      const unsigned char *pSource, long nSourcePitch,
                      const Point2<unsigned long> &Size, Bool2Type<false>) {
      blit<false>(
        pDestination, nDestinationPitch,
        pSource, nSourcePitch, Size
      );
    }

    /// Blit with color conversion
    template<typename A, typename B>
    inline void copy(unsigned char *pDestination, long nDestinationPitch,
                     const unsigned char *pSource, long nSourcePitch,
                     const Point2<unsigned long> &Size, A, B) {
      long nSourceLineAdd = nSourcePitch - (Size.X * SourcePixelFormatDescription::BytesPerPixel);
      long nDestinationLineAdd = nDestinationPitch -
        (Size.X * DestinationPixelFormatDescription::BytesPerPixel);
      
      for(unsigned long nY = 0; nY < Size.Y; ++nY) {
        for(unsigned long nX = 0; nX < Size.X; ++nX) {
          WritePixel<DestinationPixelFormatDescription>()(
            pDestination,
            DestinationPixelFormatDescription::convertFrom<SourcePixelFormatDescription>(
              ReadPixel<SourcePixelFormatDescription>()(pSource)
            )
          );

          pSource += SourcePixelFormatDescription::BytesPerPixel;
          pDestination += DestinationPixelFormatDescription::BytesPerPixel;
        }

        pSource += nSourceLineAdd;
        pDestination += nDestinationLineAdd;
      }
    }
    /// Blit without color conversion
    template<typename A>
    inline void copy(unsigned char *pDestination, long nDestinationPitch,
                     const unsigned char *pSource, long nSourcePitch,
                     const Point2<unsigned long> &Size, A, A) {
      for(unsigned long nY = 0; nY < Size.Y; ++nY) {
        memcpy(pDestination, pSource, DestinationPixelFormatDescription::BytesPerPixel * Size.X);

        pSource += nSourcePitch;
        pDestination += nDestinationPitch;
      }
    }
}; 

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_BLIT_H
