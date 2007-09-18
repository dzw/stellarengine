//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexDrawer.h - Vertex buffer primitive drawer                           //
// ### # #      ###                                                                            //
// # ### #      ###  Utility class for drawing 2D primitives using a vertex buffer             //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEXDRAWER_H
#define NUCLEX_VIDEO_VERTEXDRAWER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Video.h"
#include "Nuclex/Video/Image.h"
#include "Nuclex/Video/TextureCache.h"
#include "Nuclex/Video/VertexCache.h"
#include "Nuclex/Text/Font.h"
#include "Nuclex/Math/Point2.h"
#include "Nuclex/Math/Box2.h"
#include <vector>
#include <map>
#include <stack>
#include <list>

namespace Nuclex {
  namespace Text { class Font; }
}

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::VertexDrawer                                                                //
//  //
/// Vertex buffer drawer
/** Uses a VertexBuffer to draw simple shapes and text

    @todo Renames ScreenSize to something better
*/
class VertexDrawer {
  public:
    /// Constructor
    NUCLEX_API VertexDrawer(const shared_ptr<VideoDevice> &spVideoDevice);
    /// Destructor
    NUCLEX_API ~VertexDrawer() {}
      
  //
  // VertexDrawer implementation
  //
  public:
    /// Get total screen size
    NUCLEX_API const Point2<float> &getScreenSize() const { 
      //m_ScreenSize = m_Offsets.top().getSize();
      return m_ScreenSize;
    }

    /// Begin drawing
    NUCLEX_API void begin(const shared_ptr<VideoDevice::RenderingContext> &spRC);
    /// End drawing
    NUCLEX_API void end();

    /// Get used vertex count
    NUCLEX_API size_t getVertexCount() const { return 0; }
    /// Get used texture count
    NUCLEX_API size_t getTextureCount() const { return 0; }
    /// Get required drawprimitive operation count
    NUCLEX_API size_t getOperationCount() const { return 0; }

    /// Push drawing offset region
    NUCLEX_API void pushRegion(const Box2<float> &Region);
    /// Pop drawing offset region
    NUCLEX_API void popRegion();

    /// Draw a line
    NUCLEX_API void drawLine(
      const Point2<float> &From, const Point2<float> &To,
      const Color &LineColor = Color::White
    );

    /// Draw a color filled box
    NUCLEX_API inline void drawBox(
      const Box2<float> &Region, const Color &BoxColor = Color::White
    ) {
      drawBox(Region, shared_ptr<Texture>(), Point2<float>(), Point2<float>(), BoxColor);
    }

    /// Draw a textured box
    NUCLEX_API void drawBox(
      const Box2<float> &Region, const shared_ptr<Texture> &spTexture,
      const Point2<float> &TexStart = Point2<float>(0, 0),
      const Point2<float> &TexEnd = Point2<float>(1, 1),
      const Color &BoxColor = Color::White
    );

    /// Draw a bitmap
    NUCLEX_API inline void drawImage(
      const Point2<float> &Location, const shared_ptr<Image> &spImage,
      const Color &ImageColor = Color::White
    ) {
      drawImage(
        Box2<float>(Location, Location + Point2<float>(spImage->getSize(), StaticCastTag())),
        spImage,
        ImageColor
      );
    }

    /// Draw a bitmap
    NUCLEX_API void drawImage(
      const Box2<float> &Region, const shared_ptr<Image> &spImage,
      const Color &ImageColor = Color::White
    ) {
      TextureCache::CacheSlot CachedImage = m_TextureCache.cache(shared_ptr<Surface>(spImage));

      drawBox(
        Region, CachedImage.first,
        CachedImage.second.TL, CachedImage.second.BR, ImageColor
      );
    }

    /// Render text
    NUCLEX_API void drawText(
      const shared_ptr<Text::Font> &spFont, const wstring &sText,
      const Point2<float> &Position,
      const Color &TextColor = Color::White,
      Text::Font::Alignment eAlignment = Text::Font::A_NORMAL
    );

  private:
    /// Stack of float points for storing drawing offsets
    typedef std::stack<Box2<float> > BoxStack;
    
    /// Realized font (rasterized ?)
    struct RealizedFont {
      /// Constructor
      RealizedFont() :
        Characters(128) {}

      /// Realized font character
      struct Character {
        Character() :
          bCached(false) {}

        /// UTF-16 code of the character
        wchar_t Code;
        /// CacheID of the character bitmap
        Cacheable::CacheID CacheID;
        /// Whether the character was cached
        bool bCached;
        /// Character metrix
        Text::Font::Metrics Metrics;
      };

      typedef std::vector<Character> CharacterVector;
      CharacterVector Characters;
      typedef std::map<wchar_t, Character> CharacterMap;
      CharacterMap WCharacters;
    };
    typedef std::map<Cacheable::CacheID, RealizedFont> RealizedFontMap;
    
    static void updateCachedCharacter(
      const Surface::LockInfo &LockedSurface,
      Text::Font *pFont, RealizedFont::Character *pChar
    );

    /// The video device
    shared_ptr<VideoDevice> m_spVideoDevice;
    /// Drawing position offsets
    BoxStack m_Offsets;
    /// Current screen size
    Point2<float> m_ScreenSize;
    /// Render states for drawing
    shared_ptr<RenderStates> m_spRenderStates;
    /// Read cache for textures
    TextureCache m_TextureCache;
    /// Write cache for vertices
    VertexCache<VideoDevice::PretransformedVertex> m_VertexCache;
    /// Current rendering context
    shared_ptr<VideoDevice::RenderingContext> m_spCurrentRC;
    /// Supporting vertices for primitive drawing
    VideoDevice::PretransformedVertex m_Primitive[6];
    /// 
    RealizedFontMap m_RealizedFonts;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VERTEXDRAWER_H
