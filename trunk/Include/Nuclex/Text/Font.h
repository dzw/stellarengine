//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## FontServer.h - Font server                          //
// ### # #      ###                                                      //
// # ### #      ###  Implements an font loading server                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_TEXT_FONT_H
#define NUCLEX_TEXT_FONT_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Surface.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Cacheable.h"

namespace Nuclex { namespace Text {

//  //
//  Nuclex::Text::Font                                                                         //
//  //
/// Interface for fonts
/** Defines a general font interface for bitmap and vector fonts which
    can render themselfes onto surfaces.

    To make it possible for renderers to cache letters on textures,
    a font's style is fixed for its life time. If multiple styles
    of a type face are to be used, multiple font objects must be created.
*/
class Font :
  public Cacheable {
  public:
    /// Text alignment
    /** Controls the text alignment relative to the destination Position
    */
    enum Alignment {
      A_NORMAL = 0,                                   ///< Normal, aligned to base line
      
      A_LEFT = 1,                                     ///< Left end aligned to x position
      A_RIGHT = 2,                                    ///< Right end aligned to x position
      A_HCENTER = A_LEFT | A_RIGHT,                   ///< Horizontally centered to x position

      A_TOP = 4,                                      ///< Top aligned to y position
      A_BOTTOM = 8,                                   ///< Bottom aligned to y position
      A_VCENTER = A_TOP | A_BOTTOM                    ///< Vertically centered to y position
    };

    /// Text metrics
    struct Metrics {
      Point2<size_t> Size;                            ///< The character's total size
      Point2<long>   Advance;                         ///< Advancement to next character
      Point2<long>   Hotspot;                         ///< Hotspot of the character
    };

    /// Destructor
    /** Destroys an instance of a Font
    */
    NUCLEX_API virtual ~Font() {}

  //
  // Font implementation
  //
  public:
    /// Measure text size
    /** Measures the size required by a text string

        @param  sText  String to measure
        @return The size of the string in pixels
    */
    NUCLEX_API virtual Box2<long> measureRegion(const wstring &sText) = 0;

    NUCLEX_API virtual size_t getDescenderHeight() const = 0;

    /// Get text height
    /** Retrieves the height of the typeface

        @return The height of the typeface
    */
    NUCLEX_API virtual size_t getHeight() const = 0;

    /// Get text metrics
    /** Returns the metrics of the specified character

        @param  nChar  Character of which to return the metrics
        @return The character's metrics
    */
    NUCLEX_API virtual Metrics getMetrics(wchar_t nChar) = 0;

    /// Render text to surface
    NUCLEX_API virtual void drawText(const shared_ptr<Video::Surface> &spSurface, const wstring &sText,
                                     const Point2<long> &Position,
                                     const Color &TextColor = Color::White,
                                     Alignment eAlignment = A_NORMAL);

    /// Render to locked surface
    /** Renders the given text string onto a locked surface. The text will
        be blended onto the surface, which means that reads and writes access
        is required on the destination surface
                          
        @param  LockInfo    Locked surface onto which to render
        @param  sText       Text string to render
        @param  Position    Destination position within the surface
        @param  TextColor   Drawing color. May be ignored by color fonts.
        @param  eAlignment  How to align the text to the destination point
    */
    NUCLEX_API virtual void drawText(const Video::Surface::LockInfo &LockInfo, const wstring &sText,
                                     const Point2<long> &Position,
                                     const Color &TextColor = Color::White,
                                     Alignment eAlignment = A_NORMAL) = 0;

    /// Copy text to surface
    NUCLEX_API virtual void copyText(const shared_ptr<Video::Surface> &spSurface, const wstring &sText,
                                     const Point2<long> &Position,
                                     const Color &TextColor = Color::White,
                                     Alignment eAlignment = A_NORMAL);

    /// Copy text to locked surface
    /** Copy the given text string onto a locked surface. Instead of blending
        the text against the existing surface pixels, this method will
        copy the text's alpha values to the destination surface.
                          
        @param  LockInfo    Locked surface onto which to render
        @param  sText       Text string to render
        @param  Position    Destination position within the surface
        @param  TextColor   Drawing color. May be ignored by color fonts.
        @param  eAlignment  How to align the text to the destination point
    */
    NUCLEX_API virtual void copyText(const Video::Surface::LockInfo &LockInfo, const wstring &sText,
                                     const Point2<long> &Position,
                                     const Color &TextColor = Color::White,
                                     Alignment eAlignment = A_NORMAL) = 0;
};

}} // namespace Nuclex::Text

#endif // NUCLEX_TEXT_FONT_H
