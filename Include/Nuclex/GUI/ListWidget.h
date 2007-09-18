//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ListWidget.h - List GUI component                                         //
// ### # #      ###                                                                            //
// # ### #      ###  A pressable button which fires an event when the user clicks on it        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_LISTWIDGET_H
#define NUCLEX_GUI_LISTWIDGET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/GUI/Widget.h"
#include "Nuclex/GUI/SliderWidget.h"
#include "SigC++/SigC++.h"
#include <vector>

namespace Nuclex {
  namespace Text { class Font; }
}

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::ListWidget                                                                    //
//  //
/// GUI widget
/** 
*/
class ListWidget :
  public Widget {
  public:
    typedef SigC::Signal1<void, size_t> SelectSignal;

    /// Painter for ListWidgets
    NUCLEX_API static class Painter {
      public:
        /// Destructor
        virtual ~Painter() {}
      
      //
      // Painter implementation
      //
      public:
        /// Draw the list
        NUCLEX_API virtual void operator ()(Video::VertexDrawer &VD, Theme &T,
                                            ListWidget &TheList) {
          T.drawCell(
            VD, TheList.getStyle(), TheList.getRegion()
          );
          
          Box2<float> Region = TheList.getRegion();
          Region.TL.Y -= TheList.getScrollOffset();
          for(size_t Item = 0; Item < TheList.getItems().size(); ++Item) {
            Region.BR.Y = Region.TL.Y + T.measureRegion(
              TheList.getStyle(), TheList.getItems()[Item]
            ).getHeight() + 2;
            if(Region.BR.Y >= TheList.getRegion().BR.Y)
              break;

            if(Region.TL.Y >= TheList.getRegion().TL.Y) {
              T.drawText(
                VD, TheList.getStyle(), Region, TheList.getItems()[Item]
              );
            }
            
            Region.TL.Y = Region.BR.Y;
          }
          
          Region = TheList.getRegion();
          Region.TL.Y += 1;
          Region.BR.X -= 1;
          Region.BR.Y -= 1;
          Region.TL.X = Region.BR.X - 16;
          TheList.getSlider()->setRegion(Region);
          TheList.getSlider()->draw(VD, T);
        }
    } DefaultPainter;

    /// Constructor
    NUCLEX_API ListWidget(
      const Box2<real> &Region, const std::vector<wstring> &Items = std::vector<wstring>(),
      const string sStyle = "list",
      Painter &ThePainter = DefaultPainter
    );    
    /// Destructor
    NUCLEX_API virtual ~ListWidget() {}

  //
  // ListWidget implementation
  //
  public:
    /// Fired when the button is clicked
    SelectSignal OnSelect;

    /// Check whether the list is enabled or not
    NUCLEX_API bool isEnabled() const { return m_bEnabled; }
    /// Enable or disable the list
    NUCLEX_API virtual void setEnabled(bool bEnabled = true) { m_bEnabled = bEnabled; }

    /// Get the list's text
    NUCLEX_API const std::vector<wstring> &getItems() const { return m_Items; }
    /// Set the list's text
    NUCLEX_API virtual void setItems(const std::vector<wstring> &Items);
    
    NUCLEX_API const shared_ptr<SliderWidget> &getSlider() const { return m_spSlider; }
    NUCLEX_API void setSlider(const shared_ptr<SliderWidget> &spSlider) { m_spSlider = spSlider; }
    
    NUCLEX_API size_t getScrollOffset() const { return m_ScrollOffset; }
    
  //
  // Widget implementation
  //
  public:
    /// Clone the widget
    NUCLEX_API virtual shared_ptr<Widget> clone() const;

    /// Returns whether the widget can get the input focus    
    NUCLEX_API bool canGetFocus() const { return m_bEnabled; }

    /// Check whether the control was hit
    NUCLEX_API virtual bool hitTest(const Point2<real> &Position) {
      return (m_bEnabled && m_Region.intersects(Position));
    }

    /// Draws the widget
    NUCLEX_API virtual void draw(Video::VertexDrawer &VD, Theme &T) {
      m_Painter(VD, T, *this);
    }

  //
  // InputReceiver implementation
  //
  public:
    /// Process user input
    NUCLEX_API virtual bool processInput(const Event &InputEvent);

  protected:
    bool                      m_bEnabled;             ///< User interaction enabled ?
    std::vector<wstring>      m_Items;                ///< List items
    size_t                    m_ScrollOffset;         ///< Scrolling offset
    std::list<size_t>         m_SelectedItems;        ///< Currently selected items
    bool                      m_bMouseHover;          ///< Is mouse hovering over button ?
    bool                      m_bPressed;             ///< Is button pressed ?
    Painter                  &m_Painter;              ///< Painter for the button
    shared_ptr<SliderWidget>  m_spSlider;             ///< The list's slider
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_LISTWIDGET_H
