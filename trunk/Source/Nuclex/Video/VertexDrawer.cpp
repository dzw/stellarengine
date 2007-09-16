//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexDrawer.cpp - Vertex buffer primitive drawer                         //
// ### # #      ###                                                                            //
// # ### #      ###  Utility class for drawing 2D primitives using a vertex buffer             //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Video/Surface.h"
#include "Nuclex/Video/Texture.h"
#include "Nuclex/Video/RenderStates.h"
#include "Nuclex/Video/PixelFormat.h"

#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Text;

namespace {

// ############################################################################################# //
// # getVerticesPerPrimitive()                                                                 # //
// ############################################################################################# //
/** Returns the number of vertices required to draw a single
    primitive of the specified type
    
    @param  eType  Primitive type for which to retrieve the number of
                   required vertices
    @return The required number of vertices
*/
inline size_t getVerticesPerPrimitive(VideoDevice::RenderingContext::PrimitiveType eType) {
  switch(eType) {
    case VideoDevice::RenderingContext::PT_POINTLIST: return 1;
    case VideoDevice::RenderingContext::PT_LINELIST: return 2;
    case VideoDevice::RenderingContext::PT_TRIANGLELIST: return 3;
    case VideoDevice::RenderingContext::PT_LINESTRIP:
    case VideoDevice::RenderingContext::PT_TRIANGLESTRIP:
    case VideoDevice::RenderingContext::PT_TRIANGLEFAN: return 0;
    default: throw UnexpectedException("getVerticesPerPrimitive()", "Unknown primitive type");
  }
}

} // namespace

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::VertexDrawer()                                   Constructor # //
// ############################################################################################# //
/** Initializes an instance of VertexDrawer

    @param  spVideoDevice  The VideoDevice on which to operate
*/
VertexDrawer::VertexDrawer(const shared_ptr<VideoDevice> &spVideoDevice) :
  m_spVideoDevice(spVideoDevice),
  m_TextureCache(spVideoDevice),
  m_VertexCache(spVideoDevice),
  m_spRenderStates(spVideoDevice->createRenderStates()) {
  
  for(size_t PrimitiveIndex = 0; PrimitiveIndex < 6; ++PrimitiveIndex) {
    m_Primitive[PrimitiveIndex].Position.Z = 1.0f;
    m_Primitive[PrimitiveIndex].RHW = 1;
  }

  m_spRenderStates->setCullMode(RenderStates::CM_OFF);
  m_spRenderStates->enableDepthBuffer(false);
  m_spRenderStates->enableLighting(false);
  m_spRenderStates->enableAlphaBlend(true);
  m_spRenderStates->setSourceBlendMode(RenderStates::BM_SRCALPHA);
  m_spRenderStates->setDestinationBlendMode(RenderStates::BM_INVSRCALPHA);
  m_spRenderStates->setMinificationFilter(0, RenderStates::TF_NEAREST);
  m_spRenderStates->setMagnificationFilter(0, RenderStates::TF_NEAREST);
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::begin()                                                      # //
// ############################################################################################# //
void VertexDrawer::begin(const shared_ptr<VideoDevice::RenderingContext> &spRC) {
  m_ScreenSize = Point2<float>(m_spVideoDevice->getDisplayMode().Resolution, StaticCastTag());

  while(m_Offsets.size())
    m_Offsets.pop();

  // Required to match the pixel centers to texels
  m_Offsets.push(Box2<float>(Point2<float>(), m_ScreenSize) - Point2<float>(0.5f, 0.5f));

  spRC->getRenderStates() += m_spRenderStates;
  m_VertexCache.begin(spRC);
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::end()                                                        # //
// ############################################################################################# //
void VertexDrawer::end() {
  m_VertexCache.end();
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::pushRegion()                                                 # //
// ############################################################################################# //
/** Pushes the specified region as current clipping region
    onto the stack
    
    @param  Region  Clipping region to push onto the stack
*/
void VertexDrawer::pushRegion(const Box2<float> &Region) {
  //m_Offsets.push(Box2<float>::getIntersection(m_Offsets.top(), Region + m_Offsets.top().TL));
  m_Offsets.push(Region + m_Offsets.top().TL);
  
  m_ScreenSize = Region.getSize();
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::popRegion()                                                  # //
// ############################################################################################# //
/** Pops the topmost clipping region from the stack
*/
void VertexDrawer::popRegion() {
  m_Offsets.pop();
  
  m_ScreenSize = m_Offsets.top().getSize();
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::drawLine()                                                   # //
// ############################################################################################# //
/** Draws a line

    @param  From       Where the line should start
    @param  To         To where the line should be drawn
    @param  LineColor  Color with which to draw the line
*/
void VertexDrawer::drawLine(const Point2<float> &From, const Point2<float> &To,
                            const Color &LineColor) {
  m_Primitive[0].Position.X = From.X + m_Offsets.top().TL.X;
  m_Primitive[0].Position.Y = From.Y + m_Offsets.top().TL.Y;
  m_Primitive[1].Position.X = To.X + m_Offsets.top().TL.X;
  m_Primitive[1].Position.Y = To.Y + m_Offsets.top().TL.Y;

  m_Primitive[0].Color = m_Primitive[1].Color =
    ARGB_8_8_8_8::pixelFromColor(LineColor);
  
  m_VertexCache.addPrimitives(
    m_Primitive, 2,
    VideoDevice::RenderingContext::PT_LINELIST
  );
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::drawBox()                                                    # //
// ############################################################################################# //
/** Draws a box

    @param  Region     Region in which to draw the box
    @param  spTexture  The texture to fill the box with
    @param  TexStart   Coordinates for first box corner within texture
    @param  TexEnd     Coordinates for second box corner within texture
    @param  BoxColor   Color with which to draw the box
*/
void VertexDrawer::drawBox(const Box2<float> &Region, const shared_ptr<Texture> &spTexture,
                           const Point2<float> &TexStart, const Point2<float> &TexEnd,
                           const Color &BoxColor) {
  m_Primitive[0].Position.X = Region.TL.X + m_Offsets.top().TL.X;
  m_Primitive[0].Position.Y = Region.TL.Y + m_Offsets.top().TL.Y;
  m_Primitive[0].TexCoord.X = TexStart.X;
  m_Primitive[0].TexCoord.Y = TexStart.Y;
  m_Primitive[1].Position.X = Region.TL.X + m_Offsets.top().TL.X;
  m_Primitive[1].Position.Y = Region.BR.Y + m_Offsets.top().TL.Y;
  m_Primitive[1].TexCoord.X = TexStart.X;
  m_Primitive[1].TexCoord.Y = TexEnd.Y;
  m_Primitive[2].Position.X = Region.BR.X + m_Offsets.top().TL.X;
  m_Primitive[2].Position.Y = Region.BR.Y + m_Offsets.top().TL.Y;
  m_Primitive[2].TexCoord.X = TexEnd.X;
  m_Primitive[2].TexCoord.Y = TexEnd.Y;

  m_Primitive[3].Position.X = Region.BR.X + m_Offsets.top().TL.X;
  m_Primitive[3].Position.Y = Region.BR.Y + m_Offsets.top().TL.Y;
  m_Primitive[3].TexCoord.X = TexEnd.X;
  m_Primitive[3].TexCoord.Y = TexEnd.Y;
  m_Primitive[4].Position.X = Region.BR.X + m_Offsets.top().TL.X;
  m_Primitive[4].Position.Y = Region.TL.Y + m_Offsets.top().TL.Y;
  m_Primitive[4].TexCoord.X = TexEnd.X;
  m_Primitive[4].TexCoord.Y = TexStart.Y;
  m_Primitive[5].Position.X = Region.TL.X + m_Offsets.top().TL.X;
  m_Primitive[5].Position.Y = Region.TL.Y + m_Offsets.top().TL.Y;
  m_Primitive[5].TexCoord.X = TexStart.X;
  m_Primitive[5].TexCoord.Y = TexStart.Y;

  m_Primitive[0].Color = m_Primitive[1].Color = m_Primitive[2].Color =
  m_Primitive[3].Color = m_Primitive[4].Color = m_Primitive[5].Color =
    ARGB_8_8_8_8::pixelFromColor(BoxColor);

  m_VertexCache.addPrimitives(
    m_Primitive, 6,
    VideoDevice::RenderingContext::PT_TRIANGLELIST,
    spTexture
  );
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::drawText()                                                   # //
// ############################################################################################# //
/** Realizes a font's glyph to a cache surface

    @param  LockedSurface  Cache-Surface onto which to draw
    @param  pFont          Used font
    @param  pChar          Character to be realized
*/
void VertexDrawer::updateCachedCharacter(
  const Surface::LockInfo &LockedSurface, Text::Font *pFont, RealizedFont::Character *pChar
) {
  wchar_t pszChar[] = { pChar->Code, L'\0' };

  pFont->copyText(
    LockedSurface, pszChar, Point2<long>(0, 0),
    Color::White,
    static_cast<Font::Alignment>(Font::A_LEFT | Font::A_TOP)
  );
}

// ############################################################################################# //
// # Nuclex::Video::VertexDrawer::drawText()                                                   # //
// ############################################################################################# //
/** Draws text

    @param  spFont      Font to use for the text
    @param  sText       Text string to draw
    @param  Position    Location at which to draw the text
    @param  TextColor   The text's color
    @param  eAlignment  How to align the font in respect to the drawing location
*/
void VertexDrawer::drawText(const shared_ptr<Text::Font> &spFont,
                            const wstring &sText,
                            const Point2<float> &Position,
                            const Color &TextColor,
                            Text::Font::Alignment eAlignment) {
  RealizedFontMap::iterator FontIt = m_RealizedFonts.find(spFont->getUniqueID());
  if(FontIt == m_RealizedFonts.end()) {
    FontIt = m_RealizedFonts.insert(RealizedFontMap::value_type(
      spFont->getUniqueID(), RealizedFont()
    )).first;
  }
  
  Point2<long> AlignedPosition(Math::round<long>(Position.X), Math::round<long>(Position.Y));
  Box2<long> Region = spFont->measureRegion(sText);

  if((eAlignment & Font::A_HCENTER) == Font::A_HCENTER)
    AlignedPosition.X -= (Region.getWidth() / 2) + Region.TL.X;
  else if(eAlignment & Font::A_LEFT)
    AlignedPosition.X -= Region.TL.X;
  else if(eAlignment & Font::A_RIGHT)
    AlignedPosition.X -= Region.BR.X;

  if((eAlignment & Font::A_VCENTER) == Font::A_VCENTER)
    AlignedPosition.Y -= (Region.getHeight() / 2) + Region.TL.Y;
  else if(eAlignment & Font::A_TOP)
    AlignedPosition.Y -= Region.TL.Y;
  else if(eAlignment & Font::A_BOTTOM)
    AlignedPosition.Y -= Region.BR.Y;

  // Place each character in the vertex buffer
  for(string::size_type Pos = 0; Pos < sText.length(); ++Pos) {
    RealizedFont::Character &Char = FontIt->second.WCharacters[sText[Pos]];
    
    // Look up the character
    if(!Char.bCached) {
      Char.Code = sText[Pos];
      Char.CacheID = Cacheable().getUniqueID();
      Char.Metrics = spFont->getMetrics(sText[Pos]);
      Char.bCached = true;
    }

    if(Char.Metrics.Size.X > 0 || Char.Metrics.Size.Y > 0) {
      TextureCache::CacheSlot CachedCharacter = m_TextureCache.cache(
        Char.CacheID,
        Char.Metrics.Size,
        sigc::bind(
          &VertexDrawer::updateCachedCharacter,
          spFont.get(), &Char
        )
      );
      
      // Draw the character
      drawBox(
        Box2<float>(
          static_cast<float>(AlignedPosition.X - Char.Metrics.Hotspot.X),
          static_cast<float>(AlignedPosition.Y - Char.Metrics.Hotspot.Y),
          static_cast<float>(AlignedPosition.X - Char.Metrics.Hotspot.X + Char.Metrics.Size.X),
          static_cast<float>(AlignedPosition.Y - Char.Metrics.Hotspot.Y + Char.Metrics.Size.Y)
        ),
        CachedCharacter.first,
        CachedCharacter.second.TL,
        CachedCharacter.second.BR,
        TextColor
      );
    }

    // Advance to the next character's position
    AlignedPosition += Char.Metrics.Advance;
  }
}
