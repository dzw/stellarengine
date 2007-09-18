//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## GUIServer.h - GUI management                                              //
// ### # #      ###                                                                            //
// # ### #      ###  Manages a GUI for user interaction                                        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_GUISERVER_H
#define NUCLEX_GUI_GUISERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/GUI/Widget.h"
#include "Nuclex/GUI/DesktopWindow.h"
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Input/InputReceiver.h"
#include <map>

namespace Nuclex { namespace GUI {

//  //
//  Nuclex::GUI::GUIServer                                                                     //
//  //
/// GUI server
/** Manages the default desktop window and a collection of template widgets
    and windows for the GUI, which can be contributed to by plugins to provide
    custom widget types.
*/
class GUIServer :
  public Input::InputReceiver {
  public:
    class WidgetEnumerator;
    class WindowEnumerator;

    /// Constructor
    NUCLEX_API GUIServer();
    /// Destructor
    NUCLEX_API virtual ~GUIServer();

  //
  // GUIServer implementation
  //
  public:
    /// Get desktop window
    NUCLEX_API const shared_ptr<DesktopWindow> &getDesktopWindow() const { return m_spDesktop; }
    /// Set desktop window
    NUCLEX_API void setDesktopWindow(const shared_ptr<DesktopWindow> &spDesktop) { m_spDesktop = spDesktop; }

    /// Create new widget (clones an existing widget)
    NUCLEX_API shared_ptr<Widget> createWidget(const string &sName) const;
    /// Create new widget (clones an existing widget)
    NUCLEX_API shared_ptr<Window> createWindow(const string &sName) const;

    /// Retrieve GUI Widget by name
    NUCLEX_API const shared_ptr<Widget> &getWidget(const string &sName) const;
    /// Add GUI Widget
    NUCLEX_API void addWidget(const string &sName, const shared_ptr<Widget> &spWidget);
    /// Remove GUI Widget
    NUCLEX_API void removeWidget(const string &sName);
    /// Remove all Widgets
    NUCLEX_API void clearWidgets();
    /// Get Widget enumerator
    NUCLEX_API shared_ptr<WidgetEnumerator> enumWidgets() const;

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

    /// Draw the GUI
    NUCLEX_API void draw(Video::VertexDrawer &VD, Theme &T);
    
  //
  // InputReceiver implementation
  //
  public:
    NUCLEX_API bool processInput(const Event &InputEvent);

  private:
    /// A map of GUI Widgets
    typedef std::map<string, shared_ptr<Widget> > WidgetMap;
    /// A map of GUI Windows
    typedef std::map<string, shared_ptr<Window> > WindowMap;

    WidgetMap                 m_Widgets;              ///< Registered widgets
    WindowMap                 m_Windows;              ///< Registered windows
    shared_ptr<DesktopWindow> m_spDesktop;            ///< The desktop window
    Point2<float>             m_Cursor;
};

//  //
//  Nuclex::GUI::WidgetEnumerator                                                              //
//  //
/** Enumerates over a list of GUI Widgets
*/
class GUIServer::WidgetEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of WidgetEnumerator
    */
    NUCLEX_API virtual ~WidgetEnumerator() {}

  //
  // WidgetEnumerator implementation
  //
  public:
    /// Cycle through all Widgets
    /** Returns the current Widget and advances to the next one.
        If no more Widgets are available, returns NULL

        @return The current Widget being enumerated
    */
    NUCLEX_API virtual const shared_ptr<Widget> &cycle() = 0;
};

//  //
//  Nuclex::GUI::WindowEnumerator                                                              //
//  //
/** Enumerates over a list of GUI Windows
*/
class GUIServer::WindowEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of WindowEnumerator
    */
    NUCLEX_API virtual ~WindowEnumerator() {}

  //
  // WindowEnumerator implementation
  //
  public:
    /// Cycle through all Windows
    /** Returns the current Window and advances to the next one.
        If no more Windows are available, returns NULL

        @return The current Window being enumerated
    */
    NUCLEX_API virtual const shared_ptr<Window> &cycle() = 0;
};

}} // namespace Nuclex::GUI

#endif // NUCLEX_GUI_GUISERVER_H
