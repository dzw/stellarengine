//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ButtonWidget.h - Button GUI component                                     //
// ### # #      ###                                                                            //
// # ### #      ###  A pressable button which fires an event when the user clicks on it        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_BUTTONWIDGET_H
#define NUCLEX_GUI_BUTTONWIDGET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/GUI/Widget.h"
#include "SigC++/SigC++.h"

namespace Nuclex {
  namespace Text { class Font; }
}

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::ButtonWidget                                                                  //
//  //
/// Pushable button
/** Simple button which can be clicked to trigger an event. Its default
    behavior matches that of the standard windows button control.
    
    As any widget, the button uses a painter delegate to draw itself, the
    default implementation of which is provided in this header. If you need
    to modify the look of a single button (in contrast to the entire theme),
    you can implement a custom painter. Custom behavior can be realized by
    deriving from the button class and modifying the respective methods.
*/
class ButtonWidget :
  public Widget {
  public:
    typedef SigC::Signal0<void> PressSignal;

    /// Painter for ButtonWidgets
    NUCLEX_API static class Painter {
      public:
        /// Destructor
        virtual ~Painter() {}
      
      //
      // Painter implementation
      //
      public:
        /// Draw the button
        NUCLEX_API virtual void operator ()(Video::VertexDrawer &VD, Theme &T,
                                            ButtonWidget &TheButton,
                                            bool bHover, bool bPressed) {
          string sState = TheButton.isEnabled() ?
            (bHover ? (bPressed ? "pressed" : "hover") : "normal") : "disabled";

          T.drawCell(
            VD, TheButton.getStyle(), TheButton.getRegion(), sState
          );
          T.drawText(
            VD, TheButton.getStyle(), TheButton.getRegion(),
            TheButton.getText(), sState
          );
        }
    } DefaultPainter;

    /// Constructor
    NUCLEX_API ButtonWidget(
      const Box2<real> &Region, const wstring &sText = L"",
      const string sStyle = "button",
      Painter &ThePainter = DefaultPainter
    );
    /// Destructor
    NUCLEX_API virtual ~ButtonWidget() {}

  //
  // ButtonWidget implementation
  //
  public:
    /// Fired when the button is clicked
    PressSignal OnPress;

    /// Check whether the button is enabled or not
    NUCLEX_API bool isEnabled() const { return m_bEnabled; }
    /// Enable or disable the button
    NUCLEX_API virtual void setEnabled(bool bEnabled = true) { m_bEnabled = bEnabled; }

    /// Get the button's text
    NUCLEX_API const wstring &getText() const { return m_sText; }
    /// Set the button's text
    NUCLEX_API virtual void setText(const wstring &sText) { m_sText = sText; }
    
  //
  // Widget implementation
  //
  public:
    /// Clone the widget
    NUCLEX_API virtual shared_ptr<Widget> clone() const;

    /// Returns whether the widget can get the input focus    
    NUCLEX_API virtual bool canGetFocus() const { return m_bEnabled; }
    
    /// Check whether the control was hit
    NUCLEX_API virtual bool hitTest(const Point2<real> &Position) {
      return (m_bEnabled && m_Region.intersects(Position));
    }

    /// Draws the widget
    NUCLEX_API virtual void draw(Video::VertexDrawer &VD, Theme &T) {
      m_Painter(VD, T, *this, m_bMouseHover, m_bPressed);
    }

  //
  // InputReceiver implementation
  //
  public:
    /// Process user input
    NUCLEX_API virtual bool processInput(const Event &InputEvent);

  private:
    bool     m_bEnabled;                              ///< User interaction enabled ?
    wstring  m_sText;                                 ///< Button text

    bool     m_bMouseHover;                           ///< Is mouse hovering over button ?
    bool     m_bPressed;                              ///< Is button pressed ?
    Painter &m_Painter;                               ///< Painter for the button
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_BUTTONWIDGET_H
