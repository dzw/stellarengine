//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## TextWidget.h - Text display GUI component                                 //
// ### # #      ###                                                                            //
// # ### #      ###  A component which will output a text string on the screen                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_TEXTWIDGET_H
#define NUCLEX_GUI_TEXTWIDGET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/GUI/Widget.h"
#include "Nuclex/Text/Font.h"
#include "SigC++/SigC++.h"

namespace Nuclex {
  namespace Text { class Font; }
}

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::TextWidget                                                                    //
//  //
/// Text
/** Simple text element for displaying non-interactive strings
*/
class TextWidget :
  public Widget {
  public:
    /// Painter for TextWidgets
    NUCLEX_API static class Painter {
      public:
        /// Destructor
        virtual ~Painter() {}
      
      //
      // Painter implementation
      //
      public:
        /// Draw the text
        NUCLEX_API virtual void operator ()(Video::VertexDrawer &VD, Theme &T,
                                            TextWidget &TheText) {
          T.drawText(
            VD, TheText.getStyle(), TheText.getRegion(), TheText.getText()
          );
        }
    } DefaultPainter;

    /// Constructor
    NUCLEX_API TextWidget(
      const Box2<real> &Region, const wstring sText = L"",
      const string sStyle = "text",
      Painter &ThePainter = DefaultPainter
    );
    
    /// Destructor
    NUCLEX_API virtual ~TextWidget() {}

  //
  // TextWidget implementation
  //
  public:
    /// Get the text being displayed
    NUCLEX_API const wstring &getText() const { return m_sText; }
    /// Set the text to display
    NUCLEX_API virtual void setText(const wstring &sText) { m_sText = sText; }

    /// Get the text's alignment
    NUCLEX_API Text::Font::Alignment getAlignment() const { return m_eAlignment; }
    /// Set the text's alignment
    NUCLEX_API virtual void setAlignment(Text::Font::Alignment eAlignment) { m_eAlignment = eAlignment; }

  //
  // Widget implementation
  //
  public:
    /// Clone the widget
    NUCLEX_API shared_ptr<Widget> clone() const;

    /// Returns whether the widget can get the input focus    
    NUCLEX_API virtual bool canGetFocus() const { return false; }
    
    /// Check whether the control was hit
    NUCLEX_API virtual bool hitTest(const Point2<real> &Position) {
      return false;
    }

    /// Draws the widget
    NUCLEX_API virtual void draw(Video::VertexDrawer &VD, Theme &T) {
      m_Painter(VD, T, *this);
    }

  //
  // Receiver implementation
  //
  public:
    /// Process user input
    NUCLEX_API virtual bool processInput(const InputReceiver::Event &InputEvent);

  protected:
    wstring                m_sText;                   ///< Text to display
    Text::Font::Alignment  m_eAlignment;              ///< Text alignment
    Painter               &m_Painter;                 ///< Painter for the button
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_TEXTWIDGET_H
