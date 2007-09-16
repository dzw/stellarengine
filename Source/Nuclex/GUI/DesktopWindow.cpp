//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## DesktopWindow.cpp - GUI desktop window                                    //
// ### # #      ###                                                                            //
// # ### #      ###  Manages the root window which contains all other windows                  //
// #  ## #   # ## ## and widget in the GUI                                                     //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/DesktopWindow.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Input;
using namespace Nuclex::GUI;

// ############################################################################################# //
// # Nuclex::GUI::DesktopWindow::DesktopWindow()                                   Constructor # //
// ############################################################################################# //
/** Initializes a DesktopWindow
*/
DesktopWindow::DesktopWindow() :
  Window(Box2<float>(), "desktop"),
  m_nActiveWindowButton(0),
  m_bDragging(false) {
  Window::setMoveable(false);
}

// ############################################################################################# //
// # Nuclex::GUI::DesktopWindow::addWindow()                                                   # //
// ############################################################################################# //
/** Add a new child window to the desktop

    @param  sName     Name under which to manage the child window
    @param  spWindow  The Window to be added
*/
void DesktopWindow::addWindow(const string &sName, const shared_ptr<Window> &spWindow) {
  WindowMap::iterator WindowIt = m_Windows.find(sName);
  if(WindowIt != m_Windows.end())
    WindowIt->second = spWindow;
  else
    m_Windows.insert(WindowMap::value_type(sName, spWindow));
}

// ############################################################################################# //
// # Nuclex::GUI::DesktopWindow::removeWindow()                                                # //
// ############################################################################################# //
/** Removes a Window that was previously added using addWindow()

    @param  sName  Name under which the window was added
*/
void DesktopWindow::removeWindow(const string &sName) {
  WindowMap::iterator WindowIt = m_Windows.find(sName);
  if(WindowIt != m_Windows.end())
    m_Windows.erase(WindowIt);
  else
    throw InvalidArgumentException("Nuclex::GUI::DesktopWindow::removeWindow()",
                                   string("Item not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::DesktopWindow::clearWindows()                                                # //
// ############################################################################################# //
/** Removes all windows from the desktop
*/
void DesktopWindow::clearWindows() {
  m_Windows.clear();
}

// ############################################################################################# //
// # Nuclex::GUI::DesktopWindow::enumWindows()                                                 # //
// ############################################################################################# //
/** Creates a new enumerator to enumerate over all windows added to
    the desktop.
    
    @return A new enumerator over all windows of the desktop
*/
shared_ptr<DesktopWindow::WindowEnumerator> DesktopWindow::enumWindows() const {
  throw NotSupportedException("Nuclex::GUI::DesktopWindow::enumWindows()",
                              "Not implemented yet");
}
          
// ############################################################################################# //
// # Nuclex::GUI::DesktopWindow::draw()                                                        # //
// ############################################################################################# //
/** Draws the window

    @param  VD  VertexDrawer to use for drawing
    @param  T   The GUI theme to be used
*/
void DesktopWindow::draw(Video::VertexDrawer &VD, Theme &T) {
  // Draw all widgets positioned directly on the desktop
  shared_ptr<Window::WidgetEnumerator> spWidgetEnum = enumWidgets();
  while(spWidgetEnum->next())
    spWidgetEnum->get()->draw(VD, T);

  // Draw all child windows of the desktop
  for(WindowMap::iterator WindowIt = m_Windows.begin();
      WindowIt != m_Windows.end();
      ++WindowIt) {
      
    WindowIt->second->draw(VD, T);
  }
}

// ############################################################################################# //
// # Nuclex::GUI::DesktopWindow::processInput()                                                # //
// ############################################################################################# //
/** Processes an input event sent to the desktop window

    @param  InputEvent  The input event to be processed
    @todo Fat ugly method. Somehow decompose into behavioral traits.
*/
bool DesktopWindow::processInput(const Event &InputEvent) {
  // If this is an event containing mouse coordinates
  if(hasMouseCoordinates(InputEvent)) {
    shared_ptr<Window> spWindowUnderMouse;
    
    WindowMap::iterator WindowEnd = m_Windows.end();
    for(WindowMap::iterator WindowIt = m_Windows.begin();
        WindowIt != WindowEnd;
        ++WindowIt)
      if(WindowIt->second->hitTest(Point2<float>(InputEvent.Location, StaticCastTag())))
        spWindowUnderMouse = WindowIt->second;

    // If the mouse isn't dragging a widget, process the message normally
    if(!m_bDragging) {
      if(m_wpActiveWindow.expired()) {
      
        // When a mouse button is pressed, the widget under the mouse will be in
        // drag mode until the mouse button is released again
        if(InputEvent.eType == Event::T_MOUSEBUTTONDOWN) {
          if(spWindowUnderMouse) {
            m_nActiveWindowButton = InputEvent.nButton;
            m_wpActiveWindow = spWindowUnderMouse;

            // The left mouse button actives widgets
            if(InputEvent.nButton == 0)
              m_wpFocusWindow = spWindowUnderMouse;

            // Let the widget handle the mousebuttondown message
            return spWindowUnderMouse->processInput(InputEvent);
          } else {
            bool bWidgetUnderMouse = false;

            shared_ptr<Window::WidgetEnumerator> spWidgetEnum = enumWidgets();
            while(spWidgetEnum->next())
              if(spWidgetEnum->get()->hitTest(Point2<float>(InputEvent.Location, StaticCastTag())))
                bWidgetUnderMouse = true;
                
            if(bWidgetUnderMouse) {
              m_wpFocusWindow = shared_ptr<Window>();
            } else {
              m_nActiveWindowButton = InputEvent.nButton;
              m_bDragging = true;
              
              return true;
            }
          }       
          
        // When the mouse is moved, the widget under the mouse is notified of it
        // Also, the widget the mouse was hovering about previously receives one final message
        } else if(InputEvent.eType == Event::T_MOUSEMOVE) {
          // If there is a current hover widget, send it the mouse move message
          if(!m_wpHoverWindow.expired()) {
            shared_ptr<Window>(m_wpHoverWindow)->processInput(InputEvent);
          
            // Assign the new hover widget, or possibly nothing
            if(spWindowUnderMouse && (spWindowUnderMouse != shared_ptr<Window>(m_wpHoverWindow)))
              spWindowUnderMouse->processInput(InputEvent);
          } else if(spWindowUnderMouse) {
            spWindowUnderMouse->processInput(InputEvent);
          }
          
          m_wpHoverWindow = spWindowUnderMouse;
          
        // Mouse was pressed on empty location, but released on a widget
        } else if(InputEvent.eType == Event::T_MOUSEBUTTONUP) {
          // Will be discarded, logically seen as drag operation on the window itself
          //return false;
        }

      // The mouse is dragging a window
      } else {
        // Send all mouse messages to the widget being dragged until released
        { ScopeGuard Reset_ActiveWindow = MakeObjGuard(m_wpActiveWindow, &weak_ptr<Window>::reset);
          if((InputEvent.eType != Event::T_MOUSEBUTTONUP) ||
            (InputEvent.nButton != m_nActiveWindowButton))
            Reset_ActiveWindow.Dismiss();

          return shared_ptr<Window>(m_wpActiveWindow)->processInput(InputEvent);
        }
      }
    } else {
      if((InputEvent.eType == Event::T_MOUSEBUTTONUP) &&
         (InputEvent.nButton == m_nActiveWindowButton)) {
        m_bDragging = false;
        
        return DesktopWindow::processInput(
          Event(Event::T_MOUSEMOVE, Event::V_NONE, InputEvent.Location)
        );
      } else {
        return true;
      }
    }
  } else if(isKeyboardEvent(InputEvent)) {
    if(!m_wpFocusWindow.expired())
      return shared_ptr<Window>(m_wpFocusWindow)->processInput(InputEvent);
  }
  
  return Window::processInput(InputEvent);
}
