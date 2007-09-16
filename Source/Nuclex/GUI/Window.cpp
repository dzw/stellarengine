//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Window.cpp - GUI window                                                   //
// ### # #      ###                                                                            //
// # ### #      ###  A window in a GUI, builds dialogs from groups of widgets                  //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/Window.h"
#include "Nuclex/Kernel.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Input;
using namespace Nuclex::GUI;

Window::Painter Window::DefaultPainter;

namespace {

//  //
//  WindowWidgetEnumerator                                                                     //
//  //
/// Widget enumerator implementation
class WindowWidgetEnumerator :
  public Window::WidgetEnumerator {
  public:
    WindowWidgetEnumerator(const std::map<string, shared_ptr<Widget> > &WidgetMap) :
      m_WidgetMap(WidgetMap),
      m_WidgetIt(WidgetMap.end()),
      m_WidgetEnd(WidgetMap.end()) {}
  
  //
  // WidgetEnumerator
  //
  public:
    const shared_ptr<Widget> &get() const {
      if(m_WidgetIt == m_WidgetEnd)
        throw UnexpectedException("WindowWidgetEnumerator::get()", 
                                 "Enumerator is in empty cycle position");

      return m_WidgetIt->second;
    }

    virtual bool next() {
      if(m_WidgetIt == m_WidgetEnd)
        m_WidgetIt = m_WidgetMap.begin();
      else
        ++m_WidgetIt;
        
      return (m_WidgetIt != m_WidgetEnd);
    }
  
  private:
    const std::map<string, shared_ptr<Widget> > &m_WidgetMap;
    std::map<string, shared_ptr<Widget> >::const_iterator m_WidgetIt, m_WidgetEnd;
};

//  //
//  WindowControllerWidget                                                                     //
//  //
/// Window controller widget
class WindowControllerWidget :
  public Widget {
  public:
    /// Constructor
    WindowControllerWidget(Window &TheWindow) :
      Widget(Box2<float>(0,0,0,0), ""),
      m_bDragging(false),
      m_Window(TheWindow) {}

    /// Destructor
    ~WindowControllerWidget() {}

  //
  // Widget implementation
  //
  public:
    /// Clone the widget
    shared_ptr<Widget> clone() const {
      throw UnexpectedException(
        "WindowControllerWidget::clone()",
        "The window controller cannot be cloned"
      );
    }

    /// Draws the widget
    void draw(Video::VertexDrawer &VD, Theme &T) {}
    
  //
  // InputReceiver implementation
  //
  public:
    /// Process user input
    bool processInput(const Event &InputEvent) {
      if(m_Window.isMoveable()) {
        if((InputEvent.eType == Event::T_MOUSEBUTTONDOWN) &&
           (InputEvent.nButton == 0)) {
            
          //if(m_Window.getLocation().intersects(Point2<float>(InputEvent.Location, StaticCastTag()))) {
            m_DragSpot = Point2<float>(InputEvent.Location, StaticCastTag());
            m_bDragging = true;
          //}
        } else if(InputEvent.eType == Event::T_MOUSEBUTTONUP) {
          if(InputEvent.nButton == 0)
            m_bDragging = false;
        
        } else if(InputEvent.eType == Event::T_MOUSEMOVE) {
          if(m_bDragging) {
            Point2<float> Location = m_Window.getRegion().TL + 
              Point2<float>(InputEvent.Location, StaticCastTag()) -
              m_DragSpot;

            m_Window.setRegion(Box2<float>(
              Location,
              Location + m_Window.getRegion().getSize()
            ));
          }
        }
      }
      
      return true;
    }

  private:  
    Window        &m_Window;
    bool           m_bDragging;                     ///< Being dragged ?
    Point2<float>  m_DragSpot;                      ///< Dragging location
};

} // namespace                   

// ############################################################################################# //
// # Nuclex::GUI::Window::Window()                                                 Constructor # //
// ############################################################################################# //
/** Creates a new instance of Window
*/
Window::Window(const Box2<float> &Region, const string &sStyle, Painter &ThePainter) :
  m_nActiveWidgetButton(0),
  m_Region(Region),
  m_bMoveable(true),
  m_Painter(ThePainter),
  m_sStyle(sStyle) {

  m_spWindowController = shared_ptr<Widget>(
    new WindowControllerWidget(*this)
  );
}

// ############################################################################################# //
// # Nuclex::GUI::Window::clone()                                                              # //
// ############################################################################################# //
/** Clones the window

    @return A clone of the window
*/
shared_ptr<Window> Window::clone() const {
  return shared_ptr<Window>(new Window(*this));
}

// ############################################################################################# //
// # Nuclex::GUI::Window::getWidget()                                                          # //
// ############################################################################################# //
/** Retrieves a Window class by name

    @param  sName  Name of the Loader class to retrieve
    @return Loader class or NULL if not found
*/
const shared_ptr<Widget> &Window::getWidget(const string &sName) const {
  WidgetMap::const_iterator WidgetIt = m_Widgets.find(sName);
  if(WidgetIt != m_Widgets.end())
    return WidgetIt->second;
  else
    throw InvalidArgumentException("Nuclex::GUI::Window::getWidget()",
                                   string("Item not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::Window::addWidget()                                                          # //
// ############################################################################################# //
/** Add a new Window as Widget to the Window.

    @param  sName    Name of the window class
    @param  spWidget  The Window to add
*/
void Window::addWidget(const string &sName, const shared_ptr<Widget> &spWidget) {
  WidgetMap::iterator WidgetIt = m_Widgets.find(sName);
  if(WidgetIt != m_Widgets.end())
    WidgetIt->second = spWidget;
  else
    m_Widgets.insert(WidgetMap::value_type(sName, spWidget));
}

// ############################################################################################# //
// # Nuclex::GUI::Window::removeWidget()                                                       # //
// ############################################################################################# //
/** Removes a Window that was previously added using addWidget()

    @param  sName  Name of the Window to remove
*/
void Window::removeWidget(const string &sName) {
  WidgetMap::iterator WidgetIt = m_Widgets.find(sName);
  if(WidgetIt != m_Widgets.end())
    m_Widgets.erase(WidgetIt);
  else
    throw InvalidArgumentException("Nuclex::GUI::Window::removeWidget()",
                                   string("Item not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::Window::clearWidgets()                                                       # //
// ############################################################################################# //
/** Removes all Loader class currently added to the graphics server
*/
void Window::clearWidgets() {
  m_Widgets.clear();
}

// ############################################################################################# //
// # Nuclex::GUI::Window::enumWidgets()                                                        # //
// ############################################################################################# //
/** Returns a new enumerator over all Loaders registered to
    the video server.

    @return A new enumerator for all registered loaders
*/
shared_ptr<Window::WidgetEnumerator> Window::enumWidgets() const {
  return shared_ptr<WidgetEnumerator>(
    new WindowWidgetEnumerator(m_Widgets)
  );
}

// ############################################################################################# //
// # Nuclex::GUI::Window::setRegion()                                                          # //
// ############################################################################################# //
/** Sets the region in which the window is placed

    @param  Region  New region in which to place the window
*/
void Window::setRegion(const Box2<float> &Region) {
  m_Region = Region;
}

// ############################################################################################# //
// # Nuclex::GUI::Window::processInput()                                                       # //
// ############################################################################################# //
/** 
*/
bool Window::processInput(const Event &InputEvent_) {
  /* 
      Regel:
      
        - Wenn Maustaste auf Widget gedrückt, wird dieser zum FocusWidget

        - Wenn Maustaste gedrückt, Widget unter Maus wird ActiveWidget
        - Solange gesetzt, erhält ActiveWidget sämtliche Eingaben
        - ActiveWidget wird gelöst, sobald Maustaste losgelassen, mit welcher aktiviert
        
        - Wenn HoverWidget gesetzt, erhält dieser die Mausposition
        - Wenn HoverWidget verlassen, HoverWidget lösen
        - Wenn Maus über Widget, wird dieser zum HoverWidget
        
        - Falls kein ActiveWidget, alle Eingaben zum FocusWidget senden
  */

  Event InputEvent(InputEvent_);
  if(hasMouseCoordinates(InputEvent))
    InputEvent.Location -= Point2<long>(m_Region.TL, StaticCastTag());

  // Special processing for mouse events
  shared_ptr<Widget> spWidgetUnderMouse;
  if(hasMouseCoordinates(InputEvent)) {
    // Locate the widget beneath the mouse cursor
    WidgetMap::iterator WidgetEnd = m_Widgets.end();
    for(WidgetMap::iterator WidgetIt = m_Widgets.begin();
        WidgetIt != WidgetEnd;
        ++WidgetIt)
      if(WidgetIt->second->hitTest(Point2<float>(InputEvent.Location, StaticCastTag())))
        spWidgetUnderMouse = WidgetIt->second;
        
    if(!spWidgetUnderMouse)
      spWidgetUnderMouse = m_spWindowController;
    
    // If the mouse isn't dragging a widget, process the message normally
    if(m_wpActiveWidget.expired()) {
    
      // When a mouse button is pressed, the widget under the mouse will be in
      // drag mode until the mouse button is released again
      if(InputEvent.eType == Event::T_MOUSEBUTTONDOWN) {
        m_nActiveWidgetButton = InputEvent.nButton;
        m_wpActiveWidget = spWidgetUnderMouse;

        // The left mouse button actives widgets
        if(InputEvent.nButton == 0) {
          if(spWidgetUnderMouse->canGetFocus()) {
            if(m_wpFocusWidget.expired() || shared_ptr<Widget>(m_wpFocusWidget) != spWidgetUnderMouse) {
              if(!m_wpFocusWidget.expired())
                shared_ptr<Widget>(m_wpFocusWidget)->focusChanged(false);
            
              m_wpFocusWidget = spWidgetUnderMouse;
              spWidgetUnderMouse->focusChanged(true);
            }
          }
        }

        // Let the widget handle the mousebuttondown message
        return spWidgetUnderMouse->processInput(InputEvent);
        
      // When the mouse is moved, the widget under the mouse is notified of it
      // Also, the widget the mouse was hovering about previously receives one final message
      } else if(InputEvent.eType == Event::T_MOUSEMOVE) {
        // If there is a current hover widget, send it the mouse move message
        if(!m_wpHoverWidget.expired()) {
        
          // problem: anderer widget über widget, maus immernoch bei
          // hittest auf altem hoverwidget, bleibt deshalb in hover-status
          shared_ptr<Widget>(m_wpHoverWidget)->processInput(InputEvent);
        
          // Assign the new hover widget, or possibly nothing
          if(spWidgetUnderMouse && (spWidgetUnderMouse != shared_ptr<Widget>(m_wpHoverWidget)))
            spWidgetUnderMouse->processInput(InputEvent);
        } else if(spWidgetUnderMouse) {
          spWidgetUnderMouse->processInput(InputEvent);
        }

        m_wpHoverWidget = spWidgetUnderMouse;
        
      // Mouse was pressed on empty location, but released on a widget
      } else if(InputEvent.eType == Event::T_MOUSEBUTTONUP) {
        // Will be discarded, logically seen as drag operation on the window itself
        return false;
      }

    // The mouse is dragging a widget
    } else {
      // Send all mouse messages to the widget being dragged until released
      if((InputEvent.eType == Event::T_MOUSEBUTTONUP) &&
          (InputEvent.nButton == m_nActiveWidgetButton)) {
        shared_ptr<Widget>(m_wpActiveWidget)->processInput(InputEvent);                
        m_wpActiveWidget.reset();

        return Window::processInput(
          Event(Event::T_MOUSEMOVE, Event::V_NONE, InputEvent_.Location)
        );
      } else {
        return shared_ptr<Widget>(m_wpActiveWidget)->processInput(InputEvent);
      }
    }
  } else if(isKeyboardEvent(InputEvent)) {
    if(!m_wpFocusWidget.expired())
      return shared_ptr<Widget>(m_wpFocusWidget)->processInput(InputEvent);
  }

  return true;
}

// ############################################################################################# //
// # Nuclex::GUI::Window::draw()                                                               # //
// ############################################################################################# //
void Window::draw(Video::VertexDrawer &VD, Theme &T) {
  m_Painter(VD, T, *this);

  VD.pushRegion(m_Region);
  { ScopeGuard Pop_Region = MakeObjGuard(VD, &Video::VertexDrawer::popRegion);

    for(WidgetMap::iterator WidgetIt = m_Widgets.begin();
        WidgetIt != m_Widgets.end();
        ++WidgetIt) {
      
        WidgetIt->second->draw(VD, T);
    }
  }
}
