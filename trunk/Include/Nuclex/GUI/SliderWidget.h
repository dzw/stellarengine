//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## SliderWidget.h - Slider GUI component                                     //
// ### # #      ###                                                                            //
// # ### #      ###  Slider which can be moved within a limited range                          //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_SLIDERWIDGET_H
#define NUCLEX_GUI_SLIDERWIDGET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/GUI/Widget.h"
#include "SigC++/SigC++.h"

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::SliderWidget                                                                  //
//  //
/// Slider
/** Basic slider widget, usable as a traditional analogue slider or as scrollbar.
    The slider element's size can be adjusted to reflect the area of a scrollable
    document which can be seen on screen as known from the GUI environments of
    modern operating systems.
*/
class SliderWidget :
  public Widget {
  public:
    typedef SigC::Signal1<void, float> SlideSignal;
    
    enum Orientation {
      O_HORIZONTAL,
      O_VERTICAL
    };

    /// Painter for SliderWidgets
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
                                            SliderWidget &TheSlider,
                                            bool bHover, bool bPressed) {
          string sStyle(TheSlider.getStyle());
          if(TheSlider.getOrientation() == O_HORIZONTAL)
            sStyle += "_horizontal";
          else
            sStyle += "_vertical";

          T.drawCell(
            VD, sStyle, TheSlider.getRegion(), "background"
          );
          
          T.drawCell(
            VD, sStyle, TheSlider.getSliderRegion(),
            bPressed ? "pressed" : (bHover ? "hover" : "normal")
          );
        }
    } DefaultPainter;
    
    /// Constructor
    NUCLEX_API SliderWidget(
      const Box2<real> &Region,
      float fPosition = 0.0f, float fSize = 1.0f, size_t Steps = 0,
      Orientation eOrientation = O_HORIZONTAL, const string sStyle = "slider",
      Painter &ThePainter = DefaultPainter
    );
    /// Destructor
    NUCLEX_API virtual ~SliderWidget() {}

  //
  // SliderWidget implementation
  //
  public:
    /// Fired when the slider is moved
    SlideSignal OnSlide;

    /// Get orientation of slider widget
    NUCLEX_API Orientation getOrientation() const { return m_eOrientation; }
    /// Set orientation of slider widget
    NUCLEX_API void setOrientation(Orientation eOrientation) { m_eOrientation = eOrientation; }

    /// Check whether the slider is enabled or not
    NUCLEX_API bool isEnabled() const { return m_bEnabled; }
    /// Enable or disable the slider
    NUCLEX_API virtual void setEnabled(bool bEnabled = true);

    /// Get the button's text
    NUCLEX_API float getPosition() const { return m_fPosition; }
    /// Set the button's text
    NUCLEX_API virtual void setPosition(float fPosition) { m_fPosition = fPosition; }

    /// Get size of slider element
    NUCLEX_API float getSize() const { return m_fWidth; }
    /// Set size of slider element, from 0.0 to 1.0
    NUCLEX_API virtual void setSize(float fSize) { m_fWidth = fSize; }

    /// Get number of steps
    NUCLEX_API size_t getSteps() const { return m_Steps; }
    /// Set number of steps
    NUCLEX_API virtual void setSteps(size_t Steps) { m_Steps = Steps; }

    /// Move the slider element by a number of steps    
    NUCLEX_API void move(long nSteps);

    /// Calculate size of slider element in pixels
    NUCLEX_API float getSliderWidth() const {
      return m_fWidth * static_cast<float>(
        (m_eOrientation == O_HORIZONTAL) ? m_Region.getWidth() : m_Region.getHeight()
      );
    }

    /// Get region of slider element
    NUCLEX_API Box2<float> getSliderRegion() const;

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
      m_Painter(VD, T, *this, m_bHover, m_bDragging);
    }

  //
  // Receiver implementation
  //
  public:
    /// Process user input
    NUCLEX_API virtual bool processInput(const InputReceiver::Event &InputEvent);

  private:
    /// Snap current position to steps
    void snapToStep();
    
    Orientation    m_eOrientation;                    ///< The slider's orientation
    bool           m_bEnabled;                        ///< User interaction enabled ?
    float          m_fPosition;                       ///< Slider position
    float          m_fWidth;                          ///< Slider width
    size_t         m_Steps;                           ///< Number of slider steps
    Painter       &m_Painter;                         ///< Painter for the button
    bool           m_bHover;                          ///< Mouse hovers over slider element ?
    bool           m_bDragging;                       ///< Slider element is being dragged ?
    Point2<float>  m_DragPoint;                       ///< Relative dragging location
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_SLIDERWIDGET_H
