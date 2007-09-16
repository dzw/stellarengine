//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ToggleWidget.cpp - Toggle GUI component                                   //
// ### # #      ###                                                                            //
// # ### #      ###  A pressable button which fires an event when the user clicks on it        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/ToggleWidget.h"
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Text/Font.h"

#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

ToggleWidget::Painter ToggleWidget::DefaultPainter;

// ############################################################################################# //
// # Nuclex::GUI::ToggleWidget::ToggleWidget()                                     Constructor # //
// ############################################################################################# //
/** Creates a new instance of ToggleWidget
*/
ToggleWidget::ToggleWidget(const Box2<real> &Region,
                           const wstring &sText, bool bStatus,
                           const string sStyle, Painter &ThePainter) :
  Widget(Region, sStyle),
  m_sText(sText),
  m_bEnabled(true),
  m_bMouseHover(false),
  m_bPressed(false),
  m_Painter(ThePainter),
  m_bStatus(bStatus) {}

// ############################################################################################# //
// # Nuclex::GUI::ToggleWidget::clone()                                                        # //
// ############################################################################################# //
/** Creates a clone of the button

    @return The cloned button
*/
shared_ptr<Widget> ToggleWidget::clone() const {
  return shared_ptr<ToggleWidget>(new ToggleWidget(*this));
}

// ############################################################################################# //
// # Nuclex::GUI::ToggleWidget::setEnabled()                                                   # //
// ############################################################################################# //
/** 
*/
void ToggleWidget::setEnabled(bool bEnabled) {
  m_bEnabled = bEnabled;
}

// ############################################################################################# //
// # Nuclex::GUI::ToggleWidget::processUserInput()                                             # //
// ############################################################################################# //
/** 
*/
bool ToggleWidget::processInput(const Event &InputEvent) {
  switch(InputEvent.eType) {

    // Update the button's mouseover state when the mouse is moved
    case InputReceiver::Event::T_MOUSEMOVE: {
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      break;
    }

    case InputReceiver::Event::T_MOUSEBUTTONDOWN: {
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      if(InputEvent.nButton == 0)
        m_bPressed = true;
        
      break;
    }
    
    case InputReceiver::Event::T_MOUSEBUTTONUP: {
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      if(m_bPressed && (InputEvent.nButton == 0)) {
        m_bPressed = false;
        
        if(m_bMouseHover) {
          m_bStatus = !m_bStatus;
          OnToggle();
        }
      }
      
      break;
    }
  }

  return true;
}
