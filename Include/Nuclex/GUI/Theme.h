//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Theme.h - GUI theme                                                       //
// ### # #      ###                                                                            //
// # ### #      ###  Control how GUI elements are drawn                                        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_THEME_H
#define NUCLEX_GUI_THEME_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Variant.h"
#include "Nuclex/Math/Box2.h"
#include "Nuclex/Text/Font.h"
#include "Nuclex/Video/VertexDrawer.h"
#include <map>

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::Theme                                                                         //
//  //
/// GUI theme
/** All drawing operations of the nuclex GUI are performed through a theme,
    which controls the look of the entire GUI. Even special types of widgets,
    customized for a specific application, can use the theme interface to
    make their style customizable.
*/
class Theme {
  public:
    /// Destructor
    NUCLEX_API virtual ~Theme() {}
    
  //
  // Theme implementation
  //
  public:
    /// Draw skinned cell
    /** Draws a GUI cell. This could for example draw a single bitmap
        which is either fixed-size or will be scaled to match the cell's size
        or a cell made up from tiled edge and corner bitmaps to provide a crisp
        look in all sizes and resolutions. However, a cell is not required to
        be rectangular.
        
        @param  VD      VertexDrawer to use for drawing the cell
        @param  sStyle  Cell style. Typically the GUI item's name
        @param  Region  Region to cover
        @param  sState  State (kind of sub-style) of cell
    */
    NUCLEX_API virtual void drawCell(
      Video::VertexDrawer &VD,
      const string &sStyle,
      const Box2<float> &Region,
      const string &sState = "normal"
    ) = 0;
    
    /// Draw text
    /** Simply draws text using a font defined by the theme implementation
    
        @param  VD      VertexDrawer to use for drawing
        @param  sStyle  Text style. Typically the GUI item's name
        @param  Region  Region to cover
        @param  sText   Text to be drawn
        @param  sState  State (kind of sub-style) of text
    */
    NUCLEX_API virtual void drawText(
      Video::VertexDrawer &VD,
      const string &sStyle,
      const Box2<float> &Region,
      const wstring &sText,
      const string &sState = "normal"
    ) = 0;

    /// Measure text region
    /** Measures the region a rendered text will occupy
    
        @param  sStyle  Text style. Typically the GUI item's name
        @param  sText   Text to be measured
        @param  sState  State (kind of sub-style) of text
        @return The region which will be covered by the text
    */
    NUCLEX_API virtual Box2<float> measureRegion(
      const string &sStyle,
      const wstring &sText,
      const string &sState = "normal"
    ) = 0;

    /// Draw cursor
    /** Draws a cursor, covering both the text input caret as well as the
        mouse arrow in different states.
        
        @param  VD        VertexDrawer to use for drawing
        @param  sStyle    Text style. Typically the kind (caret or cursor)
        @param  Location  Region to cover
        @param  sState    State (kind of sub-style) of cursor
    */
    NUCLEX_API virtual void drawCaret(
      Video::VertexDrawer &VD,
      const string &sStyle,
      const Box2<float> &Region,
      const wstring &sText,
      size_t CursorPos,
      bool bInsertMode,
      const string &sState = "normal"
    ) = 0;
};

}} // namespace Nuclex::GUI;

#endif // NUCLEX_GUI_THEME_H
