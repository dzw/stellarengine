//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ToggleWidget.h - ToggleBox GUI component                                  //
// ### # #      ###                                                                            //
// # ### #      ###  A box which can be in checked or unchecked state                          //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_TOGGLEWIDGET_H
#define NUCLEX_GUI_TOGGLEWIDGET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/GUI/Widget.h"
#include "Nuclex/GUI/ButtonWidget.h"
#include "SigC++/SigC++.h"

namespace Nuclex {
  namespace Text { class Font; }
}

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::ToggleWidget                                                                  //
//  //
/// ToggleButton widget
/** 
*/
class ToggleWidget :
  public Widget {
  public:
    typedef SigC::Signal0<void> PressSignal;

    /// Painter for ToggleWidgets
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
                                            ToggleWidget &TheToggle,
                                            bool bHover, bool bPressed) {
          string sState = TheToggle.isEnabled() ?
            (bHover ? (bPressed ? "pressed" : "hover") : "normal") : "disabled";
            
          if(TheToggle.getStatus())
            sState += "_selected";

          Box2<float> ToggleRegion(TheToggle.getRegion());
          ToggleRegion.BR.X = ToggleRegion.TL.X + ToggleRegion.BR.Y - ToggleRegion.TL.Y;
          T.drawCell(
            VD, TheToggle.getStyle(), ToggleRegion, sState
          );
          
          Box2<float> TextRegion(TheToggle.getRegion());
          TextRegion.TL.X += TextRegion.BR.Y - TextRegion.TL.Y;
          
          T.drawText(
            VD, TheToggle.getStyle(), TextRegion, TheToggle.getText(), sState
          );
        }

        /// Toggle whether the button is hit by the cursor
        NUCLEX_API virtual bool hitTest(const Box2<float> &Region, const Point2<float> &Location) {
          return Region.intersects(Location);
        }
    } DefaultPainter;

    /// Constructor
    NUCLEX_API ToggleWidget(
      const Box2<real> &Region, 
      const wstring &sText, bool bStatus = false,
      const string sStyle = "check",
      Painter &ThePainter = DefaultPainter
    );
    /// Destructor
    NUCLEX_API virtual ~ToggleWidget() {}

  //
  // ToggleWidget implementation
  //
  public:
    /// Fired when the check is toggled
    PressSignal OnToggle;

    /// Toggle whether the button is enabled or not
    NUCLEX_API bool isEnabled() const { return m_bEnabled; }
    /// Enable or disable the button
    NUCLEX_API virtual void setEnabled(bool bEnabled = true);

    /// Get the button's text
    NUCLEX_API bool getStatus() const { return m_bStatus; }
    /// Set the button's text
    NUCLEX_API virtual void setStatus(bool bStatus) { m_bStatus = bStatus; }

    /// Get the text being displayed
    NUCLEX_API const wstring &getText() const { return m_sText; }
    /// Set the text to display
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

  protected:
    wstring  m_sText;                                 ///< Text to display
    bool     m_bEnabled;                              ///< User interaction enabled ?
    bool     m_bStatus;                               ///< Toggle status
    bool     m_bMouseHover;                           ///< Is mouse hovering over button ?
    bool     m_bPressed;                              ///< Is button pressed ?
    Painter &m_Painter;                               ///< Painter for the button
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_TOGGLEWIDGET_H
