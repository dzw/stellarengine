//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AlphaBlend.h - Alpha blending                                             //
// ### # #      ###                                                                            //
// # ### #      ###  Optimized software alpha blender for integer based                        //
// #  ## #   # ## ## pixel formats                                                             //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_ALPHABLEND_H
#define NUCLEX_VIDEO_ALPHABLEND_H

#include "Nuclex/Video/PixelFormat.h"

// All this funky code enables a compiler which is really good to produce the ideal alpha
// blending code including pixel format conversion at the same time and even dual alpha
// blending in a single pass.
// Nice idea, but actually, I haven't yet found a compiler which is *that* good. Even
// MSVC 7.1 likes to make non-inlined function calls instead of just emitting the 10 or so
// asm instructions built from these hundreds of lines of code.
//

namespace Nuclex { namespace Video {

namespace Detail {

//  //
//  Nuclex::Video::MultiBlend                                                                  //
//  //
/// Multi channel blender
/** Tries to blend as many color channels as possible with a single multiplication. By
    padding each color channel with AlphaBitCount bits, a multiplication by a value of up
    to AlphaBitCount bits is guaranteed to not destroy the neighboring channel's contents.
    
    How the color channels are grouped together is decided statically by recursive template
    instantiation, so a decent compiler should be able to create optimal blending code.
    
    Althought this template is made public, it is intended for internal use by the AlphaBlend
    template. The only cause for it being global is to reduce template code bloat by making one
    realization per destination pixel format sufficient.
    
    @param  PixelType             The data type used for a pixel
    @param  OriginalColorChannel  Color channel on which to operate
    @param  AlphaBitCount1        Alpha bits of first blending operation
    @param  AlphaBitCount2        Alpha bits of second blending operation
    @param  Predecessor           Preceding color channel for multichannel blend
    @param  bRoot                 Whether this channel is the root channel (forces blend)
*/
template<
  typename PixelType,
  typename OriginalColorChannel,
  size_t AlphaBitCount1,
  size_t AlphaBitCount2 = 0,
  typename Predecessor = void,
  bool bRoot = false
>
struct MultiBlend {
  private:
    /// The platforms native data type
    typedef size_t NativeType;
    
    /// Checks whether a type was provided
    template<typename Predecessor> struct TypeProvided { enum { Result = true }; };
    template<> struct TypeProvided<void> { enum { Result = false }; };

    /// Returns the number of bits allocated to the predecessor
    template<typename Predecessor> struct PredecessorBits {
      enum { Result = Predecessor::UsedBitCount };
    };
    template<> struct PredecessorBits<void> {
      enum { Result = 0 };
    };

    /// Optional predecessor stack-keeper
    template<typename Predecessor> struct ContainPredecessor {
      /// Constructor
      inline ContainPredecessor(PixelType SourcePixel) : m_Predecessor(SourcePixel) {}
      /// Conversion operator
      inline operator Predecessor() { return m_Predecessor; }
      /// Format rgba to pixel format
      inline static NativeType format(PixelType Pixel) { return Predecessor::format(Pixel); }
      /// Unformat pixel to rgba
      inline static PixelType unformat(NativeType Pixel) { return Predecessor::unformat(Pixel); }
      
      /// Perform alpha blend
      inline PixelType blend(PixelType Pixel, unsigned char cAlpha) {
        return m_Predecessor.operator()(Pixel, cAlpha);
      }
      inline PixelType blend(PixelType Pixel, unsigned char cAlpha1, unsigned char cAlpha2) {
        return m_Predecessor.operator()(Pixel, cAlpha1, cAlpha2);
      }
      private: Predecessor m_Predecessor;                                 ///< The predecessor
    };
    template<> struct ContainPredecessor<void> {
      inline ContainPredecessor(PixelType) {}
      inline static NativeType format(PixelType) { return 0; }
      inline static PixelType unformat(NativeType) { return 0; }
      inline PixelType blend(PixelType, unsigned char) { return 0; }
      inline PixelType blend(PixelType, unsigned char, unsigned char) { return 0; }
    };
    ContainPredecessor<Predecessor> m_Predecessor;
  
  public:
    enum {
      /// total number of bits available for blending
      BitsAvailable = sizeof(NativeType) * BitsPerByte,
      /// Whether the color channel blender has a predecessor
      HasPredecessor = TypeProvided<Predecessor>::Result,
      /// The number of padding bits required to prevent bit leakage
      PaddingBits = StaticMax<AlphaBitCount1, AlphaBitCount2>::Result,
      /// Whether a combined blend has to be performed in this channel
      BlendRequired = (PredecessorBits<Predecessor>::Result +
        OriginalColorChannel::Bits::Count + PaddingBits) > BitsAvailable,
      /// The first bit of the channel blender
      FirstBit = BlendRequired ? 0 : PredecessorBits<Predecessor>::Result,
      /// Total number of bits already used since the last combined blend
      UsedBitCount = FirstBit + OriginalColorChannel::Bits::Count + PaddingBits
    };

    /// The original (unformatted) color channel    
    typedef OriginalColorChannel OriginalChannel;
    /// The formatted channel for blending
    typedef ColorChannel<FirstBit, OriginalColorChannel::Bits::Count> BlendChannel;

    /// Format rgba to blending pixel format
    static inline NativeType format(PixelType Pixel) { 
      // The bits are guaranteed to fit within NativeType after conversion,
      // because we always start a new channel merge if NativeType becomes full
      return (ColorChannelConverter<NativeType, BlendChannel, OriginalChannel>()(Pixel)) |
        (BlendRequired ? 0 : ContainPredecessor<Predecessor>::format(Pixel));
    }
    /// Unformat blending pixel format to rgba
    static inline PixelType unformat(NativeType Pixel) {
      // The bits are guaranteed to fit, ...because they already fitted previously...
      return static_cast<PixelType>(
        ColorChannelConverter<NativeType, OriginalChannel, BlendChannel>()(Pixel)
      ) | (BlendRequired ? 0 : ContainPredecessor<Predecessor>::unformat(Pixel));
    }

    /// Constructor
    inline MultiBlend(PixelType SourcePixel) :
      m_Predecessor(SourcePixel) {

      if(BlendRequired || bRoot) // hey compiler, this if's result is also static
        m_SourcePixel = ContainPredecessor<Predecessor>::format(SourcePixel);
    }

    /// Alpha blend pixel with specified opacity
    inline PixelType operator ()(PixelType Pixel, unsigned char cAlpha) {
      return m_Predecessor.blend(Pixel, cAlpha) |
            blend<(BlendRequired || bRoot)>(Pixel, cAlpha);
    }
    /// Alpha blend pixel with two opacity values
    inline PixelType operator ()(PixelType Pixel, unsigned char cAlpha1, unsigned char cAlpha2) {
      return m_Predecessor.blend(Pixel, cAlpha1, cAlpha2) |
            blend(Pixel, cAlpha1, cAlpha2, Bool2Type<(BlendRequired || bRoot)>());
    }

  private:
    /// Internal single opacity pixel blending method
    template<bool>
    inline PixelType blend(PixelType Pixel, unsigned char cAlpha);

    template<>
    inline PixelType blend<true>(PixelType Pixel, unsigned char cAlpha) { 
      NativeType Formatted = ContainPredecessor<Predecessor>::format(Pixel);
      return ContainPredecessor<Predecessor>::unformat(
               Formatted + (((m_SourcePixel - Formatted) * cAlpha) >> AlphaBitCount1)
             );
    }
    template<>
    inline PixelType blend<false>(PixelType, unsigned char) { return 0; }

    /// Internal dual opacity pixel blending method
    template<bool>
    inline PixelType blend(PixelType Pixel, unsigned char cAlpha1, unsigned char cAlpha2);

    template<>
    inline PixelType blend<true>(PixelType Pixel, unsigned char cAlpha1, unsigned char cAlpha2) { 
      NativeType Formatted = ContainPredecessor<Predecessor>::format(Pixel);
      return ContainPredecessor<Predecessor>::unformat(
        Formatted + (
          (((
            ((m_SourcePixel - Formatted) * cAlpha1) >> AlphaBitCount1
          ) & ContainPredecessor<Predecessor>::format(-1)) * cAlpha2) >> AlphaBitCount2
        )
      );
    }
    template<>
    inline PixelType blend<false>(PixelType, unsigned char, unsigned char) { return 0; }

    NativeType m_SourcePixel; 
};

} // namespace Detail

//  //
//  Nuclex::Video::AlphaBlend                                                                  //
//  //
/** Alpha-blends a single pixel. If multiple pixels with the same color  are to be drawn,
    it is a good idea to create an instance of this class and reuse it, because the byte
    padding of the drawing color will only be performed once for all pixels.
    
    @param  DestinationPixelFormatDescription  The destination pixel format
    @param  SourcePixelFormatDescription       The source pixel format
    @param  AlphaBitCount1                     Number of bits in first opacity value
    @param  AlphaBitCount2                     Number of bits in second opacity value
*/
template<
  typename DestinationPixelFormatDescription,
  typename SourcePixelFormatDescription,
  size_t AlphaBitCount1,
  size_t AlphaBitCount2 = 0
>
struct AlphaBlend {
  typedef typename DestinationPixelFormatDescription::PixelType PixelType;

  // Problem ?
  // Alpha information gets lost if destination pixel format does not have an alpha channel,
  // althought the cAlpha2 argument has been extracted before

  /// Constructor
  inline AlphaBlend(typename SourcePixelFormatDescription::PixelType SourcePixel) :
    m_MultiBlend(
      DestinationPixelFormatDescription::convertFrom<SourcePixelFormatDescription>(SourcePixel)
    ) {}

  /// Perform single opacity value blend
  inline PixelType operator ()(PixelType DestinationPixel, unsigned char cAlpha) {
    return m_MultiBlend(DestinationPixel, cAlpha);
  }
  /// Perform dual opacity value blend
  inline PixelType operator ()(PixelType DestinationPixel,
                               unsigned char cAlpha1, unsigned char cAlpha2) {
    return m_MultiBlend(DestinationPixel, cAlpha1, cAlpha2);
  }

  private:
    /// An RGBA blender
    typedef Detail::MultiBlend<
      PixelType, NullChannel, AlphaBitCount1, AlphaBitCount2,
      Detail::MultiBlend<
        PixelType,
        typename DestinationPixelFormatDescription::Red, AlphaBitCount1, AlphaBitCount2,
        Detail::MultiBlend<
          PixelType,
          typename DestinationPixelFormatDescription::Green, AlphaBitCount1, AlphaBitCount2,
          Detail::MultiBlend<
            PixelType,
            typename DestinationPixelFormatDescription::Blue, AlphaBitCount1, AlphaBitCount2,
            Detail::MultiBlend<
              PixelType,
              typename DestinationPixelFormatDescription::Alpha, AlphaBitCount1, AlphaBitCount2
            >
          >
        >
      >, true
    > MultiBlendType;

    MultiBlendType m_MultiBlend;                      ///< The combined channel blender
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_ALPHABLEND_H
