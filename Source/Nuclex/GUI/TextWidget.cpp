//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## TextWidget.cpp - Text display GUI component                               //
// ### # #      ###                                                                            //
// # ### #      ###  A component which will output a text string on the screen                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/TextWidget.h"
#include "Nuclex/Video/VertexDrawer.h"

#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

TextWidget::Painter TextWidget::DefaultPainter;

// ############################################################################################# //
// # Nuclex::GUI::TextWidget::TextWidget()                                         Constructor # //
// ############################################################################################# //
/** Initializes a TextWidget
*/
TextWidget::TextWidget(const Box2<real> &Region, const wstring sText, const string sStyle,
                       Painter &ThePainter) :
  Widget(Region, sStyle),
  m_Painter(ThePainter),
  m_sText(sText) {}

// ############################################################################################# //
// # Nuclex::GUI::extWidget::clone()                                                           # //
// ############################################################################################# //
/** Creates an exact clone of the TextWidget

    @return The cloned TextWidget
*/
shared_ptr<Widget> TextWidget::clone() const {
  return shared_ptr<TextWidget>(new TextWidget(*this));
}

// ############################################################################################# //
// # Nuclex::GUI::TextWidget::processUserInput()                                               # //
// ############################################################################################# //
/** Processes input sent to the TextWidget

    @param  InputEvent  Input to process
*/
bool TextWidget::processInput(const InputReceiver::Event &InputEvent) {
  return false;
}
