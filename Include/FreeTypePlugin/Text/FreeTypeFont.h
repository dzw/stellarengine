//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## FreeTypeFont.h - FreeType font implementation                             //
// ### # #      ###                                                                            //
// # ### #      ###  Implements a nuclex font using the FreeType library for                   //
// #  ## #   # ## ## antialiased TrueType rendering                                            //
// #   #  ###  #   # R1                              (C)2002-2005 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_TEXT_FREETYPEFONT_H
#define NUCLEX_TEXT_FREETYPEFONT_H

#include "FreeTypePlugin/FreeTypePlugin.h"
#include "Nuclex/Text/Font.h"
#include <vector>
#include <map>

namespace Nuclex { namespace Text {

//  //
//  Nuclex::Text::FreeTypeFont                                                                 //
//  //
/// FreeType Font
/** A font using the FreeType library
*/
class FreeTypeFont :
  public Font {
  public:
    /// Constructor
    NUCLEXFREETYPE_API FreeTypeFont(FT_Face FTFace, size_t Size);
    /// Destructor
    NUCLEXFREETYPE_API virtual ~FreeTypeFont() {}

  //
  // Font implementation
  //
  public:
    /// Get text metrics
    NUCLEXFREETYPE_API Metrics getMetrics(wchar_t Char) {
      return getGlyph(Char).Metrics;
    }
    
    /// Measure text region
    NUCLEXFREETYPE_API Box2<long> measureRegion(const wstring &sText);
    /// Get font height
    NUCLEXFREETYPE_API size_t getHeight() const { return m_Height; }
    /// Got font descender height
    NUCLEXFREETYPE_API size_t getDescenderHeight() const { return m_FTFace->descender / -64; }

    /// Blend to locked surface
    NUCLEXFREETYPE_API void drawText(
      const Video::Surface::LockInfo &LockInfo, 
      const wstring &sText,
      const Point2<long> &Position,
      const Color &TextColor = Color::White,
      Alignment eAlignment = A_NORMAL
    );

    /// Copy to locked surface
    NUCLEXFREETYPE_API void copyText(
      const Video::Surface::LockInfo &LockInfo,
      const wstring &sText,
      const Point2<long> &Position,
      const Color &TextColor = Color::White,
      Alignment eAlignment = A_NORMAL
    );

  private:
    /// A single glyph
    /** Stores informations for a single glyph
    */
    struct Glyph {
      public:
        /// Set up and render the glyph
        void create(FT_Face FTFace, wchar_t Glyph);
        /// Blit the glyph, including its alpha-channel, onto a surface
        void blit(
          const Video::Surface::LockInfo &LockInfo,
          const Point2<long> &Location, const Color &Color
        );
        /// Blend the glyph onto a surface
        void blend(
          const Video::Surface::LockInfo &LockInfo,
          const Point2<long> &Location, const Color &Color
        );

        /// The metrics of this glyph
        Metrics Metrics;
        /// Glyph bitmap pixels
        std::vector<unsigned char> Bitmap;
    };

    /// Array containing all the bitmaps of standard ascii characters
    typedef std::vector<Glyph> GlyphVector;
    /// Map for extended characters, built when needed
    typedef std::map<wchar_t, Glyph> GlyphMap;

    /// Access a glyph by its UTF-16 character code  
    Glyph &getGlyph(wchar_t Char);

    /// The FreeType font face handle
    FT_Face m_FTFace;
    /// Array containing all the bitmaps of standard ascii characters
    Glyph m_Glyphs[128];
    /// Map of extended characters (any unicode characters and such), filled on-demand
    GlyphMap m_WGlyphs;
    /// Maximum character size this font will have
    Point2<size_t> m_MaxCharSize;
    /// The font's default line height
    size_t m_Height;
};

}} // namespace Nuclex::Text

#endif // NUCLEX_TEXT_FREETYPEFONT_H
