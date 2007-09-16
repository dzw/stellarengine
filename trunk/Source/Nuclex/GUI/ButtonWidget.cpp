//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ButtonWidget.cpp - Button GUI component                                   //
// ### # #      ###                                                                            //
// # ### #      ###  A pressable button which fires an event when the user clicks on it        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/ButtonWidget.h"
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Text/Font.h"

#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

ButtonWidget::Painter ButtonWidget::DefaultPainter;

// ############################################################################################# //
// # Nuclex::GUI::ButtonWidget::ButtonWidget()                                     Constructor # //
// ############################################################################################# //
/** Initializes a ButtonWidget

    @param  Region   Region in which to place the button
    @param  sText    Text to display on the button
    @param  sStyle   The button's visual style
    @param  Painter  Delegate used for drawing the button.
*/
ButtonWidget::ButtonWidget(const Box2<real> &Region, const wstring &sText,
                           const string sStyle, Painter &Painter) :
  Widget(Region, sStyle),
  m_bEnabled(true),
  m_sText(sText),
  m_bMouseHover(false),
  m_bPressed(false),
  m_Painter(Painter) {}

// ############################################################################################# //
// # Nuclex::GUI::ButtonWidget::clone()                                                        # //
// ############################################################################################# //
/** Creates a clone of the button

    @return The cloned button
*/
shared_ptr<Widget> ButtonWidget::clone() const {
  return shared_ptr<ButtonWidget>(new ButtonWidget(*this));
}

// ############################################################################################# //
// # Nuclex::GUI::ButtonWidget::processUserInput()                                             # //
// ############################################################################################# //
/** Processes an input event which has been sent to the button

    @param  InputEvent  The input event's data
*/
bool ButtonWidget::processInput(const Event &InputEvent) {

  switch(InputEvent.eType) {
    
    // Update the button's mouseover state when the mouse is moved
    case InputReceiver::Event::T_MOUSEMOVE: {
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      break;
    }

    // Mouse button pressed down on the button
    case InputReceiver::Event::T_MOUSEBUTTONDOWN: {
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      // Left mouse ? Button pressed.
      if(InputEvent.nButton == 0)
        m_bPressed = true;
        
      break;
    }
    
    // Mouse button released
    case InputReceiver::Event::T_MOUSEBUTTONUP: {
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      if(m_bPressed && (InputEvent.nButton == 0)) {
        m_bPressed = false;

        // Trigger signal if left mouse button released on button
        if(m_bMouseHover)
          OnPress();
      }
      
      break;
    }

  }

  return true;
}
