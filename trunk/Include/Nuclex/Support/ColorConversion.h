//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## ColorConversion.h - Color conversion functions      //
// ### # #      ###                                                      //
// # ### #      ###  Converts colors between different pixel formats     //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_COLORCONVERSION_H
#define NUCLEX_SUPPORT_COLORCONVERSION_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/BitManipulation.h"

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::ColorChannel                                        //
//  //
/// Pixel formatted color channel
/** Carries informations about an individual color channel in a
    pixel format. 
   
    The offset specifies the bit index of the lowest bit used in the
    channel (so it is right-based), while the BitCount contains the
    number of bits used, going towards higher bit indices.

    I thought long about how to best specify a range within a
    pixel format. This solution may not seem like the most intuitive,
    but it avoids many problems and is quite logical once you compare it
    to its alternatives.
*/
template<size_t Offset, size_t BitCount>
struct ColorChannel {
  typedef BitRange<Offset, Offset + BitCount> Bits;
  
  /// Convert float (0..1) to color channel bits
  template<typename PixelType>
  static inline PixelType fromFloat(float f) {
    return static_cast<PixelType>(f * Bits::Mask) & Bits::Mask;
  }
  
  /// Convert color channel bits to float (0..1)
  template<typename PixelType>
  static inline float toFloat(PixelType Pixel) {
    return toFloat(Pixel, Bool2Type<(Bits::Mask != 0)>());
  }
  
  private:
    template<typename PixelType>
    static inline float toFloat(PixelType Pixel, Bool2Type<true>) {
      return static_cast<float>(Pixel & Bits::Mask) / Bits::Mask;
    }
    template<typename PixelType>
    static inline float toFloat(PixelType, Bool2Type<false>) {
      return 1;
    }
/* With PTS

    template<typename PixelType, bool bChannelExists> 
    static inline float toFloat(PixelType Pixel);
    
    template<typename PixelType> static inline float toFloat<PixelType, bChannelExists>(PixelType Pixel) {
      return static_cast<float>(Pixel & Bits::Mask) / Bits::Mask;
    }
    template<typename PixelType>
    static inline float toFloat(PixelType, Bool2Type<false>) {
      return 1;
    }
*/    
}; 
typedef ColorChannel<0, 0> NullChannel;

//  //
//  Nuclex::Support::ColorChannelConverter                               //
//  //
/// Single color channel converter
/** Converts a single color channel to a higher or a lower number of bits

    - Decreasing color depth
      We simply cut off the unused bits and shift the result into the
      correct position
     
        RRRrrrrr ........ ........ (888)
              >> 16       RRR..... (332)

    - Increasing color depth
      If a color is converted from a lower color depth to a higher depth,
      you could simply shift the individual bits like this
               
              << 16       RRR..... (332)
        RRRrrrrr ........ ........ (888)
      
      But the result would leave the bits marked with lowercase letters
      blank. So if the 332 color was white for example, the 888 color would
      look somewhat gray, because five bits of the color are not set.

      Instead of performing a performance costly float multiplication, we
      simply repeat the color bits until we reach the required length:
      
                          421..... (332)
        42142142 ........ ........ (888)
      
      Thus, a 011 in 3 bit depth translates to 01101101 in 8 bit depth. This
      method provides even higher accuracy than floats and is by far faster
      than performing a float multiplication.
      
      
    @param  PixelType    The data type of the pixel color values.
                         Required to be an integer type.
    @param  Destination  Destination position of the channel as bit index
                         Indicates the *highest* bit of the channel range
    @param  Source         Number of bits in the destination color channel
    @param  DestinationPosition  Source position of the channel as bit index
                                 Indicates the *highest* bit of the channel range
    @param  SourceBitCount       Number of bits in the source color channel
    @todo Make a more general implementation and use this as a specialization
          for conversion between integer pixel formats
    @bug Somehow the color channel bits are not used correctly... Bits::Last
         sometimes is the same as Bits::First or even lower. Must be happening
         within MultiBlend or AlphaBlend. Somehow the resulting color is correct.
*/
template<
  typename PixelType, // Integer type
  typename Destination, // ColorChannel
  typename Source // ColorChannel
>
struct ColorChannelConverter {
  /// Converts a color into the specified destination format
  inline PixelType operator ()(PixelType Pixel) {
    return operator()<Source::Bits::Count != 0>(Pixel);
  }

  private:
    /// Alters bit depth or fill with 1s if channel not available in source
    template<bool LengthNotZero> inline PixelType operator ()(PixelType Pixel);
    template<> inline PixelType operator ()<true>(PixelType Pixel) {
      return convert<(Destination::Bits::Count > Source::Bits::Count)>(Pixel);
    }
    template<> inline PixelType operator ()<false>(PixelType Pixel) {
      // (Repeat<PixelType, 1, Destination::Bits::Count>()(1) <<
      //  Destination::Bits::First) & static_cast<PixelType>(-1) & Destination::Bits::Mask;
      return static_cast<PixelType>(-1) & Destination::Bits::Mask;
    }

    /// Increases or decreases bit depth to match destination channel
    template<bool Lengthen> inline PixelType convert(PixelType Pixel);
    template<> inline PixelType convert<true>(PixelType Pixel) {
      // Lengthen the color channel (log(DestinationBitCount / SourceBitCount) * 2 + 3)
      return Repeat<PixelType, Source::Bits::Count, Destination::Bits::Count>()(
               Shift<PixelType, Source::Bits::Last - Destination::Bits::Last>()(
                 Pixel & Source::Bits::Mask
               )
             ) & Destination::Bits::Mask;
    }
    template<> inline PixelType convert<false>(PixelType Pixel) {
      // Shorten the color channel (expands to 2 or less cpu instructions)
      return Shift<PixelType, Source::Bits::Last - Destination::Bits::Last>()(
               Pixel
             ) & Destination::Bits::Mask;
    }
};

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_COLORCONVERSION_H
