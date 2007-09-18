//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## InputWidget.h - Text input GUI component                                  //
// ### # #      ###                                                                            //
// # ### #      ###  A box into which text can be typed by the user                            //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_INPUTWIDGET_H
#define NUCLEX_GUI_INPUTWIDGET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/GUI/Widget.h"
#include "SigC++/SigC++.h"

namespace Nuclex {
  namespace Text { class Font; }
}

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::InputWidget                                                                   //
//  //
/// Input box
/** 
*/
class InputWidget :
  public Widget {
  public:
    typedef SigC::Signal0<void> ChangeSignal;

    /// Painter for InputWidgets
    NUCLEX_API static class Painter {
      public:
        /// Destructor
        virtual ~Painter() {}
      
      //
      // Painter implementation
      //
      public:
        /// Draw the input box
        NUCLEX_API virtual void operator ()(Video::VertexDrawer &VD, Theme &T,
                                            InputWidget &TheInput, bool bFocused) {
          T.drawCell(
            VD, TheInput.getStyle(), TheInput.getRegion()
          );
          T.drawText(
            VD, TheInput.getStyle(), TheInput.getRegion(), TheInput.getText(),
            bFocused ? "focus" : "normal"
          );
          
          if(bFocused)
            T.drawCaret(
              VD, TheInput.getStyle(), TheInput.getRegion(),
              TheInput.getText(), TheInput.getCursorPos(), TheInput.isInsertMode(),
              "focus"
            );
        }
    } DefaultPainter;

    /// Constructor
    NUCLEX_API InputWidget(
      const Box2<real> &Region, const wstring &sText = L"",
      const string sStyle = "input",
      Painter &ThePainter = DefaultPainter
    );
    /// Destructor
    NUCLEX_API virtual ~InputWidget() {}

  //
  // InputWidget implementation
  //
  public:
    /// Fired when the text is modified
    ChangeSignal OnChange;

    /// Check whether the button is enabled or not
    NUCLEX_API bool isEnabled() const { return m_bEnabled; }
    /// Enable or disable the button
    NUCLEX_API void setEnabled(bool bEnabled = true) { m_bEnabled = bEnabled; }

    /// Get the button's text
    NUCLEX_API const wstring &getText() const { return m_sText; }
    /// Set the button's text
    NUCLEX_API virtual void setText(const wstring &sText) { m_sText = sText; }

    /// Get current cursor position
    NUCLEX_API size_t getCursorPos() const { return m_CursorPos; }

    /// Check whether insert mode is active
    NUCLEX_API bool isInsertMode() const { return m_bInsertMode; }
    
  //
  // Widget implementation
  //
  public:
    /// Clone the widget
    NUCLEX_API shared_ptr<Widget> clone() const;

    /// Returns whether the widget can get the input focus    
    NUCLEX_API bool canGetFocus() const { return m_bEnabled; }
    
    /// Notifies the widget when it receives or loses the input focus
    NUCLEX_API void focusChanged(bool bFocused) { m_bHasFocus = bFocused; }

    /// Check whether the control was hit
    NUCLEX_API bool hitTest(const Point2<real> &Position) {
      return (m_bEnabled && m_Region.intersects(Position));
    }

    /// Draws the widget
    NUCLEX_API virtual void draw(Video::VertexDrawer &VD, Theme &T) {
      m_Painter(VD, T, *this, m_bHasFocus);
    }

  //
  // Receiver implementation
  //
  public:
    /// Process user input
    NUCLEX_API bool processInput(const Event &InputEvent);

  private:
    /// Checks whether the character is allowed in the input widget
    virtual bool isAllowedChar(wchar_t wChar) {
      return (wChar >= 0x20); //((wChar >= 0x20) && (wChar < 0x80));
    }
  
    bool     m_bEnabled;                              ///< User interaction enabled ?
    wstring  m_sText;                                 ///< Text in edit box
    size_t   m_CursorPos;                             ///< Current cursor location
    bool     m_bInsertMode;                           ///< Insert or overwrite ?
    bool     m_bHasFocus;                             ///< Whether the input box is selected
    Painter &m_Painter;                               ///< Painter for the button
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_INPUTWIDGET_H
