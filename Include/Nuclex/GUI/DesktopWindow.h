//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## DesktopWindow.h - GUI desktop window                                      //
// ### # #      ###                                                                            //
// # ### #      ###  Manages the root window which contains all other windows                  //
// #  ## #   # ## ## and widget in the GUI                                                     //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_DESKTOPWINDOW_H
#define NUCLEX_GUI_DESKTOPWINDOW_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/GUI/Window.h"

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::DesktopWindow                                                                 //
//  //
/// Root window
/** This is a special window with no graphical representation which serves
    as root container for other windows and widgets. You can have multiple desktops
    simply by using multiple DesktopWindows.

    This window is resposible for dispatching input events and chaining drawing
    operations down the GUI object model. You can disable the GUI by simply not
    forwarding input messages to this window or completely hide the GUI by
    not drawing the DesktopWindow.
*/
class DesktopWindow :
  public Window {
  public:
    class WindowEnumerator;
  
    /// Constructor
    NUCLEX_API DesktopWindow();
    /// Destructor
    NUCLEX_API virtual ~DesktopWindow() {}

  //
  // DesktopWindow implementation
  //
  public:
    // Retrieve widget which owns keyboard focus
    NUCLEX_API const weak_ptr<Window> &getActiveWindow() const { return m_wpActiveWindow; }
    // Set widget which owns keyboard focus
    NUCLEX_API void setActiveWindow(const weak_ptr<Window> &wpActiveWindow) { m_wpActiveWindow = wpActiveWindow; }

    /// Retrieve GUI Window by name
    NUCLEX_API const shared_ptr<Window> &getWindow(const string &sName) const;
    /// Add GUI Window
    NUCLEX_API void addWindow(const string &sName, const shared_ptr<Window> &spWindow);
    /// Remove GUI Window
    NUCLEX_API void removeWindow(const string &sName);
    /// Remove all Windows
    NUCLEX_API void clearWindows();
    /// Get Window enumerator
    NUCLEX_API shared_ptr<WindowEnumerator> enumWindows() const;
    ///
    NUCLEX_API size_t getWindowCount() const { return m_Windows.size(); }
  
  //
  // Window implementation
  //
  public:
    /// Clone the window
    NUCLEX_API virtual shared_ptr<Window> clone() const {
      throw NotSupportedException("Nuclex::GUI::DesktopWindow::setResizable()",
                                  "The desktop cannot be cloned");
    }
    /// Sets whether the window may be moved
    NUCLEX_API void setMoveable(bool bMoveable = true) {
      throw NotSupportedException("Nuclex::GUI::DesktopWindow::setMovable()",
                                  "The desktop does not support moving");
    }

    /// Set widget location
    NUCLEX_API void setRegion(const Box2<real> &Region) {
      throw NotSupportedException("Nuclex::GUI::DesktopWindow::setRegion()",
                                  "The desktop cannot be resized");
    }

    /// Draw the window
    NUCLEX_API virtual void draw(Video::VertexDrawer &VD, Theme &T);

  //
  // InputReceiver implementation
  //
  public:
    NUCLEX_API virtual bool processInput(const Event &InputEvent);
    
  private:
    /// A map of GUI Windows
    typedef std::map<string, shared_ptr<Window> > WindowMap;

    WindowMap        m_Windows;                       ///< Open windows
    weak_ptr<Window> m_wpActiveWindow;                ///< Currently active window
    unsigned long    m_nActiveWindowButton;           ///< Button holding the widget
    weak_ptr<Window> m_wpHoverWindow;                 ///< Last widget entered by mouse
    weak_ptr<Window> m_wpFocusWindow;                 ///< Input focus widget
    bool             m_bDragging;                     ///< Dragging on desktop ?
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_DESKTOPWINDOW_H
