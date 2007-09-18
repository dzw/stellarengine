//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Window.h - GUI window                                                     //
// ### # #      ###                                                                            //
// # ### #      ###  A window in a GUI, builds dialogs from groups of widgets                  //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_WINDOW_H
#define NUCLEX_GUI_WINDOW_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/GUI/Widget.h"
#include "Nuclex/Input/InputReceiver.h"
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Support/Exception.h"

#include <map>

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::Window                                                                        //
//  //
/// GUI window
/** 
*/
class Window :
  public Input::InputReceiver {
  public:
    class WidgetEnumerator;
    /// Painter for Windows
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
                                            Window &TheWindow) {
          T.drawCell(
            VD, TheWindow.getStyle(), TheWindow.getRegion()
          );
        }    
    } DefaultPainter;

    /// Constructor
    NUCLEX_API Window(
      const Box2<float> &Region, const string &sStyle ="window",
      Painter &ThePainter = DefaultPainter
    );

    /// Destructor
    NUCLEX_API virtual ~Window() {}

  //
  // Window implementation
  //
  public:
    /// Clone the window
    NUCLEX_API virtual shared_ptr<Window> clone() const;

    /// Returns whether the window can be moved
    NUCLEX_API bool isMoveable() const { return m_bMoveable; }
    /// Sets whether the window may be moved
    NUCLEX_API virtual void setMoveable(bool bMoveable = true) { m_bMoveable = bMoveable; }

    /// Retrieve GUI Widget by name
    NUCLEX_API const shared_ptr<Widget> &getWidget(const string &sName) const;
    /// Add GUI Widget
    NUCLEX_API virtual void addWidget(const string &sName, const shared_ptr<Widget> &spWidget);
    /// Remove GUI Widget
    NUCLEX_API virtual void removeWidget(const string &sName);
    /// Remove all Widgets
    NUCLEX_API virtual void clearWidgets();
    /// Get Widget enumerator
    NUCLEX_API shared_ptr<WidgetEnumerator> enumWidgets() const;

    /// Get widget location
    NUCLEX_API const Box2<real> &getRegion() const { return m_Region; }
    /// Set widget location
    NUCLEX_API virtual void setRegion(const Box2<real> &Region);

    /// Get widget location
    NUCLEX_API const string &getStyle() const { return m_sStyle; }
    /// Set widget location
    NUCLEX_API virtual void setStyle(const string &sStyle) { m_sStyle = sStyle; }

    /// Draw the window
    NUCLEX_API virtual void draw(Video::VertexDrawer &VD, Theme &T);

    /// Check whether window is under specified location
    NUCLEX_API virtual bool hitTest(const Point2<real> &Position) const {
      return m_Region.intersects(Position);
    }

  //
  // InputReceiver implementation
  //
  public:
    NUCLEX_API virtual bool processInput(const Event &InputEvent);

  private:
    /// A map of GUI Widgets
    typedef std::map<string, shared_ptr<Widget> > WidgetMap;

// rename: activewidget -> DragWidget
    weak_ptr<Widget>    m_wpActiveWidget;             ///< Widget held by mouse
    unsigned long       m_nActiveWidgetButton;        ///< Button holding the widget
    weak_ptr<Widget>    m_wpHoverWidget;              ///< Last widget entered by mouse
    weak_ptr<Widget>    m_wpFocusWidget;              ///< Input focus widget
    WidgetMap           m_Widgets;                    ///< Widget widgets
    Box2<real>          m_Region;                     ///< Widget location
    bool                m_bMoveable;                  ///< Is the window moveable ?
    shared_ptr<Widget>  m_spWindowController;         ///< The internal window controller
    Painter            &m_Painter;                    ///< The window's painter
    string              m_sStyle;                     ///< Visual style of the window
};

//  //
//  Nuclex::GUI::Window::WidgetEnumerator                                                      //
//  //
/** Enumerates over a list of GUI Widgets
*/
class Window::WidgetEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of WidgetEnumerator
    */
    NUCLEX_API virtual ~WidgetEnumerator() {}

  //
  // WidgetEnumerator implementation
  //
  public:
    NUCLEX_API virtual const shared_ptr<Widget> &get() const = 0;

    NUCLEX_API virtual bool next() = 0;
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_WINDOW_H
