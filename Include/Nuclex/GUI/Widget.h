//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Widget.h - Interactive GUI component                                      //
// ### # #      ###                                                                            //
// # ### #      ###  A component in a GUI which can be manipulated by the user,                //
// #  ## #   # ## ## such as a button or a text field                                          //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_WIDGET_H
#define NUCLEX_GUI_WIDGET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Math/Box2.h"
#include "Nuclex/Input/InputReceiver.h"
#include "Nuclex/GUI/Theme.h"

namespace Nuclex {
  namespace Video { class VertexDrawer; }
}

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::Widget                                                                        //
//  //
/// GUI widget
/** 
*/
class Widget :
  public Input::InputReceiver {
  public:
    /// Constructor
    NUCLEX_API Widget(const Box2<real> &Region, /*const shared_ptr<Theme> &spTheme,*/
                      const string &sStyle) :
      m_Region(Region),
      /*m_spTheme(spTheme),*/
      m_sStyle(sStyle) {}

    /// Destructor
    NUCLEX_API virtual ~Widget() {}

  //
  // Widget implementation
  //
  public:
    /// Clone the widget
    /** Creates an identical deep copy of the widget
    
        @return The cloned widget
    */
    NUCLEX_API virtual shared_ptr<Widget> clone() const = 0;

    /// Draws the widget
    NUCLEX_API virtual void draw(Video::VertexDrawer &VD, Theme &T) = 0;

    /// Performs a hit check of the specified point
    NUCLEX_API virtual bool hitTest(const Point2<real> &Position) {
      return m_Region.intersects(Position);
    }

    /// Returns whether the widget can get the input focus    
    NUCLEX_API virtual bool canGetFocus() const { return false; }
    /// Notifies the widget when it receives or loses the input focus
    NUCLEX_API virtual void focusChanged(bool bFocused) {}
    
    /// Get widget location
    NUCLEX_API const Box2<real> &getRegion() const { return m_Region; }
    /// Set widget location
    NUCLEX_API virtual void setRegion(const Box2<real> &Region) { m_Region = Region; }

    /// Get widget location
    NUCLEX_API const shared_ptr<Theme> &getTheme() const { return m_spTheme; }
    /// Set widget location
    NUCLEX_API virtual void setTheme(const shared_ptr<Theme> &spTheme) { m_spTheme = spTheme; }
    
    /// Get widget style
    NUCLEX_API const string &getStyle() const { return m_sStyle; }
    /// Set widget style
    NUCLEX_API virtual void setStyle(const string &sStyle) { m_sStyle = sStyle; }

  //
  // InputReceiver implementation
  //
  public:
    /// Process user input
    NUCLEX_API virtual bool processInput(const Event &InputEvent) { return false; }

  protected:
    /// Location of the widget on the GUI area
    Box2<real> m_Region;
    /// Theme used for the widget
    shared_ptr<Theme> m_spTheme;
    /// The style used by the widget
    string m_sStyle;
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_WIDGET_H
