//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ListWidget.cpp - List GUI component                                       //
// ### # #      ###                                                                            //
// # ### #      ###  A pressable button which fires an event when the user clicks on it        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/ListWidget.h"
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Text/Font.h"

#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

ListWidget::Painter ListWidget::DefaultPainter;

// ############################################################################################# //
// # Nuclex::GUI::ListWidget::ListWidget()                                         Constructor # //
// ############################################################################################# //
/** Creates a new instance of ListWidget
*/
ListWidget::ListWidget(const Box2<real> &Region, const std::vector<wstring> &Items,
                       const string sStyle, Painter &ThePainter) :
  Widget(Region, sStyle),
  m_bEnabled(true),
  m_bMouseHover(false),
  m_bPressed(false),
  m_Painter(ThePainter),
  m_Items(Items),
  m_ScrollOffset(0),
  m_spSlider(new SliderWidget(
    Box2<float>(Region.BR.X - 20, Region.TL.Y + 1, Region.BR.X - 1, Region.BR.Y - 1),
    0, 0.5, 0, SliderWidget::O_VERTICAL
  )) {}

// ############################################################################################# //
// # Nuclex::GUI::ListWidget::clone()                                                          # //
// ############################################################################################# //
/** Creates a clone of the button

    @return The cloned button
*/
shared_ptr<Widget> ListWidget::clone() const {
  return shared_ptr<ListWidget>(new ListWidget(*this));
}

// ############################################################################################# //
// # Nuclex::GUI::ListWidget::setText()                                                        # //
// ############################################################################################# //
/** 
*/
void ListWidget::setItems(const std::vector<wstring> &Items) {
  m_Items = Items;
}

// ############################################################################################# //
// # Nuclex::GUI::ListWidget::processUserInput()                                               # //
// ############################################################################################# //
/** 
*/
bool ListWidget::processInput(const Event &InputEvent) {
  
  switch(InputEvent.eType) {
    // Update the button's mouseover state when the mouse is moved
    case InputReceiver::Event::T_MOUSEMOVE: {
      
/*    
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));
*/      
      break;
    }

    case InputReceiver::Event::T_MOUSEBUTTONDOWN: {
/*    
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      if(InputEvent.nList == 0)
        m_bPressed = true;
*/        
      break;
    }
    case InputReceiver::Event::T_MOUSEBUTTONUP: {
/*    
      m_bMouseHover = hitTest(Point2<real>(InputEvent.Location, StaticCastTag()));

      if(m_bPressed && (InputEvent.nList == 0)) {
        m_bPressed = false;
        
        if(m_bMouseHover)
          OnPress();
      }
*/      
      break;
    }
  }

  return m_spSlider->processInput(InputEvent);
}
