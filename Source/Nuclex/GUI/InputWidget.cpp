//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## InputWidget.cpp - Text input GUI component                                //
// ### # #      ###                                                                            //
// # ### #      ###  A box into which text can be typed by the user                            //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/InputWidget.h"
#include "Nuclex/Video/VertexDrawer.h"

#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

InputWidget::Painter InputWidget::DefaultPainter;

// ############################################################################################# //
// # Nuclex::GUI::InputWidget::InputWidget()                                       Constructor # //
// ############################################################################################# //
/** Creates a new instance of InputWidget
*/
InputWidget::InputWidget(const Box2<real> &Region, const wstring &sText, const string sStyle,
                         Painter &ThePainter) :
  Widget(Region, sStyle),
  m_bEnabled(true),
  m_sText(sText),
  m_CursorPos(sText.length()),
  m_bHasFocus(false),
  m_bInsertMode(true),
  m_Painter(ThePainter) {}

// ############################################################################################# //
// # Nuclex::GUI::InputWidget::clone()                                                         # //
// ############################################################################################# //
/** Creates a clone of the button

    @return The cloned button
*/
shared_ptr<Widget> InputWidget::clone() const {
  return shared_ptr<InputWidget>(new InputWidget(*this));
}

// ############################################################################################# //
// # Nuclex::GUI::InputWidget::processUserInput()                                              # //
// ############################################################################################# //
/** 
*/
bool InputWidget::processInput(const Event &InputEvent) {

  switch(InputEvent.eType) {
    case Event::T_CHAR: {
      if(m_bEnabled) {
        if(isAllowedChar(InputEvent.wChar)) {
          if(m_bInsertMode || (m_CursorPos == m_sText.length()))
            m_sText.insert(m_sText.begin() + m_CursorPos, static_cast<char>(InputEvent.wChar));
          else
            m_sText[m_CursorPos] = static_cast<char>(InputEvent.wChar);

          ++m_CursorPos;
        }
      }
      break;
    }
    case Event::T_KEYDOWN: {
      if(m_bEnabled) {
        switch(InputEvent.cKey) {
          case 14: { // Backspace
            if(m_CursorPos > 0) {
              m_sText.erase(m_sText.begin() + (m_CursorPos - 1));
              --m_CursorPos;
            }
            
            break;
          }
          
          case 83: { // Delete
            if(m_CursorPos < m_sText.length())
              m_sText.erase(m_sText.begin() + m_CursorPos);

            break;
          }
          
          case 75: { // Left
            if(m_CursorPos > 0)
              --m_CursorPos;

            break;
          }
          
          case 77: { // Right
            if(m_CursorPos < m_sText.length())
              ++m_CursorPos;

            break;
          }
          
          case 71: { // Home
            m_CursorPos = 0;
            break;
          }
          
          case 79: { // End
            m_CursorPos = m_sText.length();
            break;
          }
          
          case 82: { // Insert
            m_bInsertMode = !m_bInsertMode;
            break;
          }
        }
      } // m_bEnabled
      break;
    }
  }

  return true;
}
