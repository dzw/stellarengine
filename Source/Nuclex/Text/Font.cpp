//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## Font.cpp - Font                                     //
// ### # #      ###                                                      //
// # ### #      ###  Implements an font                                  //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Text/Font.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Text;

// ############################################################################################# //
// # Nuclex::Text::Font::drawText()                                                            # //
// ############################################################################################# //
/** Renders the given text string onto a locked surface. The text will
    be blended onto the surface, which means that reads and writes access
    is required on the destination surface
                      
    @param  spSurface   Surface onto which to render
    @param  sText       Text string to render
    @param  Position    Destination position within the surface
    @param  TextColor   Drawing color. May be ignored by color fonts.
    @param  eAlignment  How to align the text to the destination point
*/
void Font::drawText(const shared_ptr<Video::Surface> &spSurface, const wstring &sText,
                    const Point2<long> &Position, const Color &TextColor,
                    Alignment eAlignment) {
  const Surface::LockInfo &LockedSurface = spSurface->lock(Surface::LM_READWRITE);
  { ScopeGuard unlock_Surface = MakeObjGuard(*spSurface.get(), &Surface::unlock);
    drawText(LockedSurface, sText, Position, TextColor, eAlignment);
  }
}

// ############################################################################################# //
// # Nuclex::Text::Font::copyText()                                                            # //
// ############################################################################################# //
/** Copy the given text string onto a locked surface. Instead of blending
    the text against the existing surface pixels, this method will
    copy the text's alpha values to the destination surface.
                      
    @param  spSurface   Surface onto which to render
    @param  sText       Text string to render
    @param  Position    Destination position within the surface
    @param  TextColor   Drawing color. May be ignored by color fonts.
    @param  eAlignment  How to align the text to the destination point
*/
void Font::copyText(const shared_ptr<Video::Surface> &spSurface, const wstring &sText,
                    const Point2<long> &Position, const Color &TextColor,
                    Alignment eAlignment) {
  const Surface::LockInfo &LockedSurface = spSurface->lock(Surface::LM_READWRITE);
  { ScopeGuard unlock_Surface = MakeObjGuard(*spSurface.get(), &Surface::unlock);
    copyText(LockedSurface, sText, Position, TextColor, eAlignment);
  }
}
