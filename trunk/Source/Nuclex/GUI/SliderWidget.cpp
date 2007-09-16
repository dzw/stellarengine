//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## SliderWidget.cpp - Slider GUI component                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Slider which can be moved within a limited range                          //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/SliderWidget.h"
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Input/KeyboardInputDevice.h"
//#include "DirectX/DInput.h" //!!Temporary

#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

SliderWidget::Painter SliderWidget::DefaultPainter;

namespace {

float clamp(float f) {
  return min(max(f, 0.0f), 1.0f);
}

}

// ############################################################################################# //
// # Nuclex::GUI::SliderWidget::SliderWidget()                                     Constructor # //
// ############################################################################################# //
/** Initializes a new slider widget

    @param  Region     Region in which to place the slider
    @param  fPosition  Initial position of the slider, from 0.0 to 1.0
    @param  fSize      The slider element's size
    @param  Steps      Number of steps from the left side to the right side.
                       A number of 0 will disable stepping.
    @param  sStyle     The slider's visual style
    @param  Painter    Delegate used for drawing the slider.
*/
SliderWidget::SliderWidget(const Box2<real> &Region,
                           float fPosition, float fSize, size_t Steps,
                           Orientation eOrientation, const string sStyle,
                           Painter &ThePainter) :
  Widget(Region, sStyle),
  m_eOrientation(eOrientation),
  m_bEnabled(true),
  m_Painter(ThePainter),
  m_bDragging(false),
  m_bHover(false),
  m_fPosition(fPosition),
  m_fWidth(fSize),
  m_Steps(Steps) {
  // Make sure the user provided position is valid when using steps
  snapToStep();
}

// ############################################################################################# //
// # Nuclex::GUI::SliderWidget::clone()                                                        # //
// ############################################################################################# //
/** Creates a clone of the slider

    @return The cloned slider
*/
shared_ptr<Widget> SliderWidget::clone() const {
  return shared_ptr<SliderWidget>(new SliderWidget(*this));
}

// ############################################################################################# //
// # Nuclex::GUI::SliderWidget::setEnabled()                                                   # //
// ############################################################################################# //
/** Enables or disables the slider

    @param  bEnabled  Whether the slider should be enabled or disabled
*/
void SliderWidget::setEnabled(bool bEnabled) {
  m_bEnabled = bEnabled;
  
  if(!bEnabled)
    m_bDragging = false;
}

// ############################################################################################# //
// # Nuclex::GUI::SliderWidget::processUserInput()                                             # //
// ############################################################################################# //
/** Processes an input event which has been sent to the button

    @param  InputEvent  The input event's data
*/
bool SliderWidget::processInput(const InputReceiver::Event &InputEvent) {
  switch(InputEvent.eType) {

    // Update the slider when the mouse is moved
    case InputReceiver::Event::T_MOUSEMOVE: {
      m_bHover = getSliderRegion().intersects(Point2<float>(InputEvent.Location, StaticCastTag()));

      // Is the slider element being dragged ?
      if(m_bDragging) {
        float fOldPosition = m_fPosition;

        // Adjust the slider elements position
        Point2<float> Location = Point2<float>(InputEvent.Location, StaticCastTag()) - m_DragPoint;
        if(m_eOrientation == O_HORIZONTAL)
          m_fPosition = clamp((Location.X - m_Region.TL.X) / (m_Region.getWidth() - getSliderWidth()));
        else
          m_fPosition = clamp((Location.Y - m_Region.TL.Y) / (m_Region.getHeight() - getSliderWidth()));
        snapToStep();

        // Fire an event if the position has been changed
        if(m_fPosition != fOldPosition)
          OnSlide(m_fPosition);
      }

      break;
    }
    
    // Mouse button pressed within slider region
    case InputReceiver::Event::T_MOUSEBUTTONDOWN: {
      // Calculate slider region
      Box2<float> SliderRegion(getSliderRegion());

      // Left mouse ? Move or drag slider.
      if(InputEvent.nButton == 0) {
        long nMovement;

        if(m_eOrientation == O_HORIZONTAL)
          nMovement = (InputEvent.Location.X < SliderRegion.TL.X) ?
            -1 : ((InputEvent.Location.X >= SliderRegion.BR.X) ? +1 : 0);
        else
          nMovement = (InputEvent.Location.Y < SliderRegion.TL.Y) ?
            -1 : ((InputEvent.Location.Y >= SliderRegion.BR.Y) ? +1 : 0);

        // Move (if clicked beside the slider element) or begin dragging
        if(nMovement) {
          move(nMovement);
        } else {
          m_DragPoint = Point2<float>(InputEvent.Location, StaticCastTag());        
          if(m_eOrientation == O_HORIZONTAL)
            m_DragPoint.X -= m_Region.TL.X + (m_Region.getWidth() - getSliderWidth()) * m_fPosition;
          else
            m_DragPoint.Y -= m_Region.TL.Y + (m_Region.getHeight() - getSliderWidth()) * m_fPosition;

          m_bDragging = true;
        }
      }

      m_bHover = getSliderRegion().intersects(Point2<float>(InputEvent.Location, StaticCastTag()));

      break;
    }
    
    // Mouse button released, stop dragging
    case InputReceiver::Event::T_MOUSEBUTTONUP: {
      m_bHover = getSliderRegion().intersects(Point2<float>(InputEvent.Location, StaticCastTag()));

      if(m_bDragging && (InputEvent.nButton == 0))
        m_bDragging = false;

      break;
    }

    // Key pressed while slider widget was focused
    case InputReceiver::Event::T_KEYDOWN: {
      long nMovement;
      
      if((InputEvent.cKey == Input::KeyboardInputDevice::getScancode("Left")) ||
         (InputEvent.cKey == Input::KeyboardInputDevice::getScancode("Up")))
        nMovement = -1;
      else if((InputEvent.cKey == Input::KeyboardInputDevice::getScancode("Right")) ||
         (InputEvent.cKey == Input::KeyboardInputDevice::getScancode("Down")))
        nMovement = +1;
      else
        nMovement = 0;

      move(nMovement);

      break;  
    }

  }

  return true;
}

// ############################################################################################# //
// # Nuclex::GUI::SliderWidget::getSliderRegion()                                              # //
// ############################################################################################# //
Box2<float> SliderWidget::getSliderRegion() const {
  float fSliderWidth = getSliderWidth();
  float fScrollWidth = (m_eOrientation == O_HORIZONTAL) ?
    (m_Region.BR.X - m_Region.TL.X) - fSliderWidth:
    (m_Region.BR.Y - m_Region.TL.Y) - fSliderWidth;

  if(m_eOrientation == O_HORIZONTAL)
    return Box2<float>(
      static_cast<float>(round<size_t>(m_Region.TL.X + fScrollWidth * m_fPosition)),
      m_Region.TL.Y,
      static_cast<float>(round<size_t>(m_Region.TL.X + fScrollWidth * m_fPosition + fSliderWidth)),
      m_Region.BR.Y
    );
  else
    return Box2<float>(
      m_Region.TL.X,
      static_cast<float>(round<size_t>(m_Region.TL.Y + fScrollWidth * m_fPosition)),
      m_Region.BR.X,
      static_cast<float>(round<size_t>(m_Region.TL.Y + fScrollWidth * m_fPosition + fSliderWidth))
    );
}

// ############################################################################################# //
// # Nuclex::GUI::SliderWidget::snapToStep()                                                   # //
// ############################################################################################# //
void SliderWidget::snapToStep() {
  if(m_Steps > 0) {
    float fPosition = 0;
    float fMinDistance = 1.0f;
    
    for(size_t Step = 0; Step < m_Steps; ++Step) {
      float fDistance = fabs((static_cast<float>(Step) / (m_Steps - 1)) - m_fPosition);
      if(fDistance < fMinDistance) {
        fPosition = static_cast<float>(Step) / (m_Steps - 1);
        fMinDistance = fDistance;
      }
    }
    
    m_fPosition = fPosition;
  }
}

void SliderWidget::move(long nClicks) {
  float fOldPosition = m_fPosition;

  if(m_Steps > 0)
    m_fPosition += static_cast<float>(nClicks) / (m_Steps - 1);
  else
    m_fPosition += m_fWidth * static_cast<float>(nClicks) / 2;
      
  if(m_fPosition != fOldPosition)
    OnSlide(m_fPosition);
}