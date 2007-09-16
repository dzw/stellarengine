//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## FreeTypeFont.cpp - FreeType font implementation                           //
// ### # #      ###                                                                            //
// # ### #      ###  Implements a nuclex font using the FreeType library for                   //
// #  ## #   # ## ## antialiased TrueType rendering                                            //
// #   #  ###  #   # R1                              (C)2002-2005 Markus Ewald -> License.txt  //
//  //
#include "FreeTypePlugin/Text/FreeTypeFont.h"
#include "Nuclex/Video/PixelFormat.h"
#include "Nuclex/Video/Blit.h"
#include "Nuclex/Video/AlphaBlend.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Text;

namespace {

//  //
//  BlendAlphamap                                                                              //
//  //
/// Auxiliary structure for blending alpha maps onto pixel buffers
struct BlendAlphaMap {
  /// Perform the alpha map blending
  inline void operator ()(
    void *pDestination, long nDestinationPitch,
    Surface::PixelFormat eDestinationFormat,
    const void *pSource, long nSourcePitch,
    const Point2<size_t> &Size, const Color &PixelColor
  ) {

    // Select the right blending function based on the destination surface's pixel format
    switch(eDestinationFormat) {

      // 8 bits alpha-only surface
      case Surface::PF_A_8: {
        To<A_8>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // 8 bits 3-3-2 RGB surface
      case Surface::PF_RGB_3_3_2: {
        To<RGB_3_3_2>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // 16 bits 5-6-5 RGB surface
      case Surface::PF_RGB_5_6_5: {
        To<RGB_5_6_5>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // 24 bits 8-8-8 RGB surface
      case Surface::PF_RGB_8_8_8: {
        To<RGB_8_8_8>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // 16 bits 5-5-5 RGB surface
      case Surface::PF_XRGB_1_5_5_5: {
        To<XRGB_1_5_5_5>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // 32 bits 8-8-8 RGB surface
      case Surface::PF_XRGB_8_8_8_8: {
        To<XRGB_8_8_8_8>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // 16 bits 1-5-5-5 RGB surface with alpha channel
      case Surface::PF_ARGB_1_5_5_5: {
        To<ARGB_1_5_5_5>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // 32 bits 8-8-8-8 RGB surface with alpha channel
      case Surface::PF_ARGB_8_8_8_8: {
        To<ARGB_8_8_8_8>()(
          reinterpret_cast<unsigned char *>(pDestination), nDestinationPitch,
          reinterpret_cast<const unsigned char *>(pSource), nSourcePitch,
          Size, PixelColor
        );
        break;
      }

      // Unknown destination pixel format
      default:
        throw UnexpectedException("BlendAlphaMap::operator()", "Invalid source pixel format");
    }
  }

  //  //
  //  BlendAlphamap::To                                                                        //
  //  //
  /// Templated structure specialized for each destination pixel format
  template<typename DestinationPixelFormatDescription>
  struct To {

    /// Alpha map blending function
    void operator ()(
      unsigned char *pDestination, long nDestinationPitch,
      const unsigned char *pSource, long nSourcePitch,
      const Point2<size_t> &Size, const Color &PixelColor
    ) {

      // Number of bytes to skip after completing a line in the source bitmap or
      // destination bitmap respectively
      long nSourceLineAdd = nSourcePitch - (Size.X * A_8::BytesPerPixel);
      long nDestinationLineAdd = nDestinationPitch - (Size.X * DestinationPixelFormatDescription::BytesPerPixel);

      // Prepare an alpha blender for the currently selected drawing color
      Video::AlphaBlend<
        DestinationPixelFormatDescription, DestinationPixelFormatDescription, A_8::Alpha::Bits::Count
      > BlendPixel(DestinationPixelFormatDescription::pixelFromColor(PixelColor));

      // Careful, this is an actual tight loop(tm) which needs to run quite fast
      for(size_t nY = 0; nY < Size.Y; ++nY) {
        for(size_t nX = 0; nX < Size.X; ++nX) {
          WritePixel<DestinationPixelFormatDescription>()(
            pDestination,
            BlendPixel(
              ReadPixel<DestinationPixelFormatDescription>()(pDestination),
              ReadPixel<A_8>()(pSource)
            )
          );

          pSource += A_8::BytesPerPixel;
          pDestination += DestinationPixelFormatDescription::BytesPerPixel;
        }

        pSource += nSourceLineAdd;
        pDestination += nDestinationLineAdd;
      }
    }
  };
};

} // namespace

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::FreeTypeFont()                                    Constructor # //
// ############################################################################################# //
/** Initializes an instance of FreeTypeFont

    @param  FTFace  Freetype font face
    @param  nSize   Desired font size
    @check Is the mutex really required around the FT_Set_Pixel_Sizes() call ?
*/
FreeTypeFont::FreeTypeFont(FT_Face FTFace, size_t nSize) :
  m_FTFace(FTFace),
  m_MaxCharSize(nSize, nSize),
  m_Height(nSize) {

  { Mutex::ScopedLock FreeTypeUser(getFreeTypeMutex());
  
    // Set up the character sizes for this font. This magically scales the TrueType font
    // to the exact size in which we want it to be rendered
    FT_Error Error = FT_Set_Pixel_Sizes(FTFace, 0, nSize);
    if(Error)
      throw UnexpectedException(
        "Nuclex::Text::FreeTypeFont::FreeTypeFont()",
        string("FT_Set_Pixel_Sizes() failed: ") + getFreeTypeErrorDescription(Error)
      );

    // Realize all the standard ascii characters
    for(size_t Glyph = 0; Glyph < 128; ++Glyph)
      m_Glyphs[Glyph].create(m_FTFace, Glyph);
  }
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::renderText()                                                  # //
// ############################################################################################# //
/** Renders the given text string onto a locked surface
                      
    @param  LockInfo    Locked surface onto which to render
    @param  sText       Text string to render
    @param  Position    Destination position within the surface
    @param  TextColor   Drawing color for the text
    @param  eAlignment  How to align the text to the destination point
*/
void FreeTypeFont::drawText(
  const Video::Surface::LockInfo &LockInfo, const wstring &sText,
  const Point2<long> &Position, const Color &TextColor, Alignment eAlignment
) {
  // Calculate the region this piece of text will occupy
  Box2<long> Region = measureRegion(sText);

  // Then adjust the position in accordance with the formatting rules given
  Point2<long> AlignedPosition(Position);

  // Horizontal adjustment
  if((eAlignment & A_HCENTER) == A_HCENTER)
    AlignedPosition.X -= (Region.getWidth() / 2) + Region.TL.X;
  else if(eAlignment & A_LEFT)
    AlignedPosition.X -= Region.TL.X;
  else if(eAlignment & A_RIGHT)
    AlignedPosition.X -= Region.BR.X;

  // Vertical adjustment
  if((eAlignment & A_VCENTER) == A_VCENTER)
    AlignedPosition.Y -= (Region.getHeight() / 2) + Region.TL.Y;
  else if(eAlignment & A_TOP)
    AlignedPosition.Y -= Region.TL.Y;
  else if(eAlignment & A_BOTTOM)
    AlignedPosition.Y -= Region.BR.Y;

  // Finally, we can proceed and render the string onto the destination surface
  for(string::size_type Pos = 0; Pos < sText.length(); ++Pos) {
    Glyph &Glyph = getGlyph(sText[Pos]);

    Glyph.blend(LockInfo, (AlignedPosition) - Glyph.Metrics.Hotspot, TextColor);

    AlignedPosition += Glyph.Metrics.Advance;
  }
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::copyText()                                                    # //
// ############################################################################################# //
/** Renders the given text string onto a locked surface
                      
    @param  LockInfo    Locked surface onto which to render
    @param  sText       Text string to render
    @param  Position    Destination position within the surface
    @param  TextColor   Drawing color for the text
    @param  eAlignment  How to align the text to the destination point
*/
void FreeTypeFont::copyText(
  const Video::Surface::LockInfo &LockInfo, const wstring &sText,
  const Point2<long> &Position, const Color &TextColor, Alignment eAlignment
) {
  // Calculate the region this piece of text will occupy
  Box2<long> Region = measureRegion(sText);

  // Then adjust the position in accordance with the formatting rules given
  Point2<long> AlignedPosition(Position);

  // Horizontal adjustment
  if((eAlignment & A_HCENTER) == A_HCENTER)
    AlignedPosition.X -= (Region.getWidth() / 2) + Region.TL.X;
  else if(eAlignment & A_LEFT)
    AlignedPosition.X -= Region.TL.X;
  else if(eAlignment & A_RIGHT)
    AlignedPosition.X -= Region.BR.X;

  // Vertical adjustment
  if((eAlignment & A_VCENTER) == A_VCENTER)
    AlignedPosition.Y -= (Region.getHeight() / 2) + Region.TL.Y;
  else if(eAlignment & A_TOP)
    AlignedPosition.Y -= Region.TL.Y;
  else if(eAlignment & A_BOTTOM)
    AlignedPosition.Y -= Region.BR.Y;

  // Finally, we can proceed and render the string onto the destination surface
  for(string::size_type Pos = 0; Pos < sText.length(); ++Pos) {
    Glyph &Glyph = getGlyph(sText[Pos]);

    Glyph.blit(LockInfo, (AlignedPosition) - Glyph.Metrics.Hotspot, TextColor);

    AlignedPosition += Glyph.Metrics.Advance;
  }
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::Glyph::create()                                               # //
// ############################################################################################# //
/** Realizes the glyph

    @param  FTFace  Freetype font face
    @param  nGlyph  Index of the glyph to realize
*/
void FreeTypeFont::Glyph::create(FT_Face FTFace, wchar_t Glyph) {

  // This mutex reflects the remote possibility that multiple glyphs might have their
  // create() method called at the same time
  { Mutex::ScopedLock Lock(getFreeTypeMutex());

    // Try to render the selected chara
    FT_Error Error = ::FT_Load_Char(FTFace, Glyph, FT_LOAD_RENDER);
    if(Error)
      throw UnexpectedException(
        "Nuclex::FreeTypeFont::Glyph::create()",
        string("FreeType reported an error while rendering the glyph") +
          getFreeTypeErrorDescription(Error)
      );

    // Obtain the glyph's metadata
    Metrics.Advance.set(FTFace->glyph->advance.x, FTFace->glyph->advance.y);
    Metrics.Advance /= 64;
    Metrics.Hotspot.set(-FTFace->glyph->bitmap_left, FTFace->glyph->bitmap_top);
    Metrics.Size.set(FTFace->glyph->bitmap.width, FTFace->glyph->bitmap.rows);

    // Copy the glyph's pixels to our internal pixel buffer
    size_t PixelCount = Metrics.Size.X * Metrics.Size.Y;
    if(PixelCount > 0) {
      Bitmap.resize(PixelCount);

      unsigned char *pSource = FTFace->glyph->bitmap.buffer;
      unsigned char *pDestination = &Bitmap[0];
      for(size_t Line = 0; Line < Metrics.Size.Y; ++Line) {
        ::memcpy(pDestination, pSource, Metrics.Size.X);
        pSource += FTFace->glyph->bitmap.pitch;
        pDestination += Metrics.Size.X;
      }
    }
  }
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::Glyph::blit()                                                 # //
// ############################################################################################# //
/** Blits the glyph bitmap in the specified color and its opacity as
    alpha channel onto the destination surface
    
    @param  Destination  Surface to blit to
    @param  Location     Location at which to blit
    @param  Color        Color to use for the glyph
*/
void FreeTypeFont::Glyph::blit(
  const Video::Surface::LockInfo &Destination, const Point2<long> &Location, const Color &Color
) {

  // Make sure we've got write access to the surface we're supposed to draw on
  if(!(Destination.eMode & Surface::LM_WRITE))
    throw FailedException(
      "Nuclex::Text::FreeTypeFont::Glyph::blend()",
      "The destination surface has to be locked for write access"
    );

  // Prepare a surface LockInfo structure for blitting
  Surface::LockInfo CharacterSurface;
  CharacterSurface.eFormat = Surface::PF_A_8;
  CharacterSurface.eMode = Surface::LM_READ;
  CharacterSurface.nPitch = Metrics.Size.X;
  CharacterSurface.pMemory = &Bitmap[0];
  CharacterSurface.Size = Metrics.Size;

  // Now we can use the standard blitting routine to blit the glyph to the destination surface
  Surface::blit(
    Destination, CharacterSurface,
    Location,
    Box2<long>(0, 0, Metrics.Size.X, Metrics.Size.Y)
  );
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::Glyph::blend()                                                # //
// ############################################################################################# //
/** Alpha-blends the glyph bitmap in the specified color onto the destination surface.
    A read/write-lock is required for this operation to succeed.
    
    @param  Destination  Surface to blit to
    @param  Location     Location at which to blit
    @param  Color        Color to use for the glyph
*/
void FreeTypeFont::Glyph::blend(
  const Video::Surface::LockInfo &Destination, const Point2<long> &Location, const Color &Color
) {

  // Make sure we've got read and write access to the surface we're supposed to draw on
  if((Destination.eMode & Surface::LM_READWRITE) != Surface::LM_READWRITE)
    throw FailedException(
      "Nuclex::Text::FreeTypeFont::Glyph::blend()",
      "The destination surface has to be locked for read+write access"
    );
                                        
  // Get a valid source region
  Box2<long> ClippedSourceRegion(0, 0, Metrics.Size.X, Metrics.Size.Y);

  Point2<long> ClippedLocation(
    (Location.X < 0) ? (ClippedSourceRegion.TL.X -= Location.X, 0) : Location.X,
    (Location.Y < 0) ? (ClippedSourceRegion.TL.Y -= Location.Y, 0) : Location.Y
  );

  BlendAlphaMap()(
    static_cast<unsigned char *>(Destination.pMemory) +
      (ClippedLocation.Y * Destination.nPitch) +
      (ClippedLocation.X * Surface::bppFromFormat(Destination.eFormat)),
    Destination.nPitch,
    Destination.eFormat,
    static_cast<const unsigned char *>(&Bitmap[0]) +
      (ClippedSourceRegion.TL.Y * Metrics.Size.X) +
      (ClippedSourceRegion.TL.X * 1),
    Metrics.Size.X, 
    Point2<size_t>(
      std::min<unsigned long>(Destination.Size.X - ClippedLocation.X, ClippedSourceRegion.getWidth()),
      std::min<unsigned long>(Destination.Size.Y - ClippedLocation.Y, ClippedSourceRegion.getHeight())
    ),
    Color
  );
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::measureRegion()                                               # //
// ############################################################################################# //
/** Measures the size required by a text string

    @param  sText  String to measure
    @return The string's size
*/
Box2<long> FreeTypeFont::measureRegion(const wstring &sText) {
  // This could be done way more effective under the assumption that no characters will have
  // irregular hotspots and advancements, eg. advancements which place the next character to
  // the left of the previous one

  Box2<long> Region;
  Point2<long> Position;

  // Run through the whole string as if rendering it
  for(string::size_type Pos = 0; Pos < sText.length(); ++Pos) {

    // Obtain the next glyph to be rendered
    const Glyph &Glyph = getGlyph(sText[Pos]);

    // Update the position of the leftmost pixel being drawn
    long nX = (Position.X) - Glyph.Metrics.Hotspot.X;
    if((nX < Region.TL.X) || (Pos == 0))
      Region.TL.X = nX;

    // Update the position of the topmost pixel being drawn
    long nY = (Position.Y) - Glyph.Metrics.Hotspot.Y;
    if((nY < Region.TL.Y) || (Pos == 0))
      Region.TL.Y = nY;

    // We assume the glyph will never have a negative size to make things faster
    Position += Glyph.Metrics.Advance;

    // Update the position of the rightmost pixel being drawn
    nX = (Position.X) - Glyph.Metrics.Hotspot.X;
    if((nX > Region.BR.X) || (Pos == 0))
      Region.BR.X = nX;

    // Update the position of the bottommost pixel being drawn
    nY = (Position.Y) - Glyph.Metrics.Hotspot.Y;
    if((nY > Region.BR.Y) || (Pos == 0))
      Region.BR.Y = nY;
  }

  return Region;
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFont::getGlyph()                                                    # //
// ############################################################################################# //
/** Retrieves the glyph depicting the specified UTF-16 character

    @param  Char  UTF-16 Character whose glyph image to retrieve
    @return The specified character's glyph image
*/
FreeTypeFont::Glyph &FreeTypeFont::getGlyph(wchar_t Char) {

  // The first 128 glyphs have been pre-rendered and are always available
  if(Char < 128)
    return m_Glyphs[Char];

  // Other glyphs, especially unicode ones, might need to be realized first
  GlyphMap::iterator GlyphIt = m_WGlyphs.find(Char);
  if(GlyphIt == m_WGlyphs.end()) {
    GlyphIt = m_WGlyphs.insert(GlyphMap::value_type(Char, Glyph())).first;
    GlyphIt->second.create(m_FTFace, Char);
  }

  return GlyphIt->second;
}
