//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## PixelFormat.h - Pixel format description                                  //
// ### # #      ###                                                                            //
// # ### #      ###  A template-based pixel format description with                            //
// #  ## #   # ## ## color conversion functions                                                //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_PIXELFORMAT_H
#define NUCLEX_VIDEO_PIXELFORMAT_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Color.h"
#include "Nuclex/Support/ColorConversion.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::PixelFormatDescription                                                      //
//  //
/// Pixel format descriptor
/** Describes a pixel format by storing a set of color channels and
    their bit layouts.
    
    @param  PixelType                The data type to use for a pixel
    @param  Red, Green, Blue, Alpha  Color channel layouts. Set to a
                                     valid ColorChannel<> or to NullChannel
    @param  BitsPerPixel             The number of bits per pixel. Will be
                                     deduced from the channels if not supplied
*/
template<
  typename PixelType,
  typename Red = NullChannel, typename Green = NullChannel,
  typename Blue = NullChannel, typename Alpha = NullChannel,
  size_t BitsPerPixel = StaticMax<
    StaticMax<Red::Bits::Last, Green::Bits::Last>::Result,
    StaticMax<Blue::Bits::Last, Alpha::Bits::Last>::Result
  >::Result
>
struct PixelFormatDescription {
  typedef PixelType PixelType;
  typedef Red Red;
  typedef Green Green;
  typedef Blue Blue;
  typedef Alpha Alpha;

  private:
    template<typename T> struct ChannelProvided { enum { Result = true }; };
    template<> struct ChannelProvided<NullChannel> { enum { Result = false }; };
  public:

  enum {
    /// True when an alpha channel is present
    HasAlphaChannel = ChannelProvided<Alpha>::Result,
    /// The number of color channels
    ChannelCount = ChannelProvided<Red>::Result + ChannelProvided<Green>::Result +
                   ChannelProvided<Blue>::Result + ChannelProvided<Alpha>::Result,
    /// The number of bits per pixel
    BitsPerPixel = BitsPerPixel,
    /// The number of bytes per pixel
    BytesPerPixel = BitsPerPixel / BitsPerByte
  };
  
  /// Convert a pixel from another format into this format 
  template<typename OtherPixelFormatDescription>
  static inline PixelType convertFrom(typename OtherPixelFormatDescription::PixelType Pixel) {
    return convertFrom<OtherPixelFormatDescription>(
      Pixel,
      Bool2Type<(sizeof(OtherPixelFormatDescription::PixelType) > sizeof(PixelType))>()
    );
  }

  /// Build a pixel color from a visix color specification
  /** @param  Color  Color to convert to a pixel color
      @return The pixel color of the specified visix color specification
  */
  static inline PixelType pixelFromColor(const Color &Color) {
    return
      Red::fromFloat<PixelType>(Color.R) |
      Green::fromFloat<PixelType>(Color.G) |
      Blue::fromFloat<PixelType>(Color.B) |
      Alpha::fromFloat<PixelType>(Color.A);
  }

  /// Get the visix color specification of a pixel color
  /** @param  Pixel  Pixel color to convert to a color
      @return The visix color specification of the specified pixel color
  */
  static inline Color colorFromPixel(PixelType Pixel) {
    return Color(
      Red::toFloat(Pixel),
      Green::toFloat(Pixel),
      Blue::toFloat(Pixel),
      Alpha::toFloat(Pixel)
    );
  }

  private:
    // Required to let the ColorChannelConverter use the larger of two pixel types
    template<typename OtherPixelFormatDescription>
    static inline PixelType convertFrom(typename OtherPixelFormatDescription::PixelType Pixel,
                                        Bool2Type<true>) {
      return static_cast<PixelType>(
        ColorChannelConverter<
          OtherPixelFormatDescription::PixelType, Red, OtherPixelFormatDescription::Red
        >()(Pixel) |
        ColorChannelConverter<
          OtherPixelFormatDescription::PixelType, Green, OtherPixelFormatDescription::Green
        >()(Pixel) |
        ColorChannelConverter<
          OtherPixelFormatDescription::PixelType, Blue, OtherPixelFormatDescription::Blue
        >()(Pixel) |
        ColorChannelConverter<
          OtherPixelFormatDescription::PixelType, Alpha, OtherPixelFormatDescription::Alpha
        >()(Pixel)
      );
    }

    template<typename OtherPixelFormatDescription>
    static inline PixelType convertFrom(typename OtherPixelFormatDescription::PixelType Pixel,
                                        Bool2Type<false>) {
      return ColorChannelConverter<PixelType, Red, OtherPixelFormatDescription::Red>()(Pixel) |
        ColorChannelConverter<PixelType, Green, OtherPixelFormatDescription::Green>()(Pixel) |
        ColorChannelConverter<PixelType, Blue, OtherPixelFormatDescription::Blue>()(Pixel) |
        ColorChannelConverter<PixelType, Alpha, OtherPixelFormatDescription::Alpha>()(Pixel);
    }
};

/// Read a pixel in the specified pixel format
/** You can specialize this function if the default implementation is not
    adequate for a pixel format (as is the case with rgb-8-8-8)
*/
template<typename PixelFormatDescription>  
struct ReadPixel {
  typedef PixelFormatDescription PixelFormatDescription;
  inline typename PixelFormatDescription::PixelType operator ()(const void *pMemory) {
    return *reinterpret_cast<const PixelFormatDescription::PixelType *>(pMemory);
  }
};

/// Write a pixel in the specified pixel format
/** You can specialize this function if the default implementation is not
    adequate for a pixel format (as is the case with rgb-8-8-8)
*/
template<typename PixelFormatDescription>  
struct WritePixel {
  typedef PixelFormatDescription PixelFormatDescription;
  inline void operator ()(void *pMemory, typename PixelFormatDescription::PixelType Pixel) {
    *reinterpret_cast<PixelFormatDescription::PixelType *>(pMemory) = Pixel;
  }
};

typedef PixelFormatDescription<
  unsigned_8,        // 8 bits
  NullChannel,
  NullChannel,
  NullChannel,
  ColorChannel<0, 8> // aaaaaaaa
> A_8;

typedef PixelFormatDescription<
  unsigned_8,         // 8 bits
  ColorChannel<5, 3>, // rrr.....
  ColorChannel<2, 3>, // ...ggg..
  ColorChannel<0, 2>  // ......bb
> RGB_3_3_2;

typedef PixelFormatDescription<
  unsigned_8,         // 8 bits
  ColorChannel<5, 3>, // rrr.....
  ColorChannel<2, 3>, // ...ggg..
  ColorChannel<0, 2>  // ......bb
> RGB_3_3_2;

typedef PixelFormatDescription<
  unsigned_16,         // 16 bits
  ColorChannel<11, 5>, // rrrrr... ........
  ColorChannel< 5, 6>, // .....ggg ggg.....
  ColorChannel< 0, 5>  // ........ ...bbbbb
> RGB_5_6_5;

typedef PixelFormatDescription<
  unsigned_32,         // 24 bits
  ColorChannel<16, 8>, // rrrrrrrr ........ ........
  ColorChannel< 8, 8>, // ........ gggggggg ........
  ColorChannel< 0, 8>, // ........ ........ bbbbbbbb
  NullChannel,
  24
> RGB_8_8_8;

typedef PixelFormatDescription<
  unsigned_16,         // 16 bits
  ColorChannel<10, 5>, // .rrrrr.. ........
  ColorChannel< 5, 5>, // ......gg ggg.....
  ColorChannel< 0, 5>, // ........ ...bbbbb
  NullChannel,
  16
> XRGB_1_5_5_5;

typedef PixelFormatDescription<
  unsigned_32,         // 32 bits
  ColorChannel<16, 8>, // ........ rrrrrrrr ........ ........
  ColorChannel< 8, 8>, // ........ ........ gggggggg ........
  ColorChannel< 0, 8>, // ........ ........ ........ bbbbbbbb
  NullChannel,
  32
> XRGB_8_8_8_8;

typedef PixelFormatDescription<
  unsigned_16,         // 16 bits
  ColorChannel<10, 5>, // .rrrrr.. ........
  ColorChannel< 5, 5>, // ......gg ggg.....
  ColorChannel< 0, 5>, // ........ ...bbbbb
  ColorChannel<15, 1>  // a....... ........
> ARGB_1_5_5_5;

typedef PixelFormatDescription<
  unsigned_16,         // 16 bits
  ColorChannel< 8, 4>, // ....rrrr ........
  ColorChannel< 4, 4>, // ........ gggg....
  ColorChannel< 0, 4>, // ........ ....bbbb
  ColorChannel<12, 4>  // aaaa.... ........
> ARGB_4_4_4_4;

typedef PixelFormatDescription<
  unsigned_32,         // 32 bits
  ColorChannel<16, 8>, // ........ rrrrrrrr ........ ........
  ColorChannel< 8, 8>, // ........ ........ gggggggg ........
  ColorChannel< 0, 8>, // ........ ........ ........ bbbbbbbb
  ColorChannel<24, 8>  // aaaaaaaa ........ ........ ........
> ARGB_8_8_8_8;

// Specialization because default implementation would touch neighboring pixel
template<> struct ReadPixel<RGB_8_8_8> {
  typedef RGB_8_8_8 PixelFormatDescription;
  inline PixelFormatDescription::PixelType operator ()(const void *pMemory) {
    return *reinterpret_cast<const PixelFormatDescription::PixelType *>(pMemory);
  }
};
template<> struct WritePixel<RGB_8_8_8> {
  typedef RGB_8_8_8 PixelFormatDescription;
  inline void operator ()(void *pMemory, PixelFormatDescription::PixelType Pixel) {
    *reinterpret_cast<PixelFormatDescription::PixelType *>(pMemory) = Pixel;
  }
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_PIXELFORMAT_H
