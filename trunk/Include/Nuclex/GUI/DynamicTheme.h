//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## DefaultPainter.h - Default GUI painter                                    //
// ### # #      ###                                                                            //
// # ### #      ###  Default painter for all nuclex GUI elements                               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_DYNAMICTHEME_H
#define NUCLEX_GUI_DYNAMICTHEME_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Serializer.h"
#include "Nuclex/Storage/ResourceSet.h"
#include "Nuclex/GUI/Theme.h"
#include "Nuclex/Support/Color.h"
#include "Nuclex/Video/VertexDrawer.h"
#include <deque>
#include <map>

namespace Nuclex {
  namespace Storage { class StorageServer; }
  namespace Video { class VideoServer; class Image; }
  namespace Text { class TextServer; }
}

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::DynamicTheme                                                                  //
//  //
/// Dynamic GUI theme
/** GUI theme with dynamically loaded layout description. The layout can for example
    be read from an XML description using the XML serializer implementation.
*/
class DynamicTheme :
  public Theme {
  public:
    /// Constructor
    NUCLEX_API DynamicTheme(
      const shared_ptr<Storage::StorageServer> &spStorageServer,
      const shared_ptr<Video::VideoServer> &spVideoServer,
      const shared_ptr<Text::TextServer> &spTextServer
    );
    
    /// Destructor
    NUCLEX_API virtual ~DynamicTheme();

  //
  // DynamicTheme implementation
  //
  public:
    /// Load object from serializer
    NUCLEX_API void load(const shared_ptr<Storage::Serializer> &spSerializer);
  
  //
  // Theme implementation
  //
  public:
    /// Draw skinned cell
    NUCLEX_API void drawCell(
      Video::VertexDrawer &VD,
      const string &sStyle,
      const Box2<float> &Region,
      const string &sState = "normal"
    );
    
    /// Draw text
    NUCLEX_API void drawText(
      Video::VertexDrawer &VD,
      const string &sStyle,
      const Box2<float> &Region,
      const wstring &sText,
      const string &sState = "normal"
    );

    /// Measure text region
    NUCLEX_API Box2<float> measureRegion(
      const string &sStyle,
      const wstring &sText,
      const string &sState = "normal"
    );

    /// Draw cursor
    NUCLEX_API void drawCaret(
      Video::VertexDrawer &VD,
      const string &sStyle,
      const Box2<float> &Region,
      const wstring &sText,
      size_t CursorPos,
      bool bInsertMode,
      const string &sState = "normal"
    );

  private:
    /// A deque of strings
    typedef std::deque<string> ResourceDeque;

    /// Stores the layout of a widget
    struct Style {
      /// State which a widget can be in
      struct State {
        /// Image used for the widget's visualization
        struct Image {
          /// How to place the bitmap
          enum Placement {
            P_TOPLEFT,                                ///< Place in top left corner
            P_TOP,                                    ///< Stretch over top border
            P_TOPRIGHT,                               ///< Place in top right corner
            P_LEFT,                                   ///< Stretch over left border
            P_CENTER,                                 ///< Stretch in center
            P_RIGHT,                                  ///< Stretch over right border
            P_BOTTOMLEFT,                             ///< Place in bottom left corner
            P_BOTTOM,                                 ///< Stretch over bottom border
            P_BOTTOMRIGHT                             ///< Place in bottom right corner
          };

          shared_ptr<Video::Image> spImage;           ///< The bitmap
          Color                    ImageColor;        ///< The bitmap's color
        };
        
        /// A map of bitmaps
        typedef std::map<Image::Placement, Image> ImageMap;

        ImageMap              Images;                 ///< Images for this state
        shared_ptr<Text::Font> spFont;                ///< Font to use for text
        Text::Font::Alignment  eTextAlignment;        ///< How to align text
        Color                  TextColor;             ///< Color for the text
        Point2<float>          TextOffset;            ///< Offset of text from center
      };

      /// A map of states      
      typedef std::map<string, State> StateMap;
      
      StateMap States;                                ///< States of the widget
    };
    /// A map of widget layouts
    typedef std::map<string, Style> StyleMap;

    /// Get bitmap placement from string
    static Style::State::Image::Placement placementFromString(const string &sPlacement);    
    /// Get alignment of font from string
    static Text::Font::Alignment alignmentFromString(const string &sAlignment);

    string                             m_sName;       ///< The theme's name
    shared_ptr<Video::VideoServer>     m_spVideoServer; ///< Image server to store bitmaps in
    Storage::ResourceSet               m_Resources;   ///< Resources of the theme
    StyleMap                           m_Styles;      ///< The theme's styles
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_DYNAMICTHEME_H
