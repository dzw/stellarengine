//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## GUIServer.cpp - GUI management                                            //
// ### # #      ###                                                                            //
// # ### #      ###  Manages a GUI for user interaction                                        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/GUIServer.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::GUIServer()                                           Constructor # //
// ############################################################################################# //
/** Initializes the GUI server
*/
GUIServer::GUIServer() :
  m_spDesktop(new DesktopWindow()) {}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::~GUIServer()                                           Destructor # //
// ############################################################################################# //
/** Destroys a GUIServer
*/
GUIServer::~GUIServer() {}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::createWidget()                                                    # //
// ############################################################################################# //
/** Creates a new widget by cloning the widget with the specified name

    @param  sName  Name of the prototype widget which to clone
    @return The new widget
*/
shared_ptr<Widget> GUIServer::createWidget(const string &sName) const {
  WidgetMap::const_iterator WidgetIt = m_Widgets.find(sName);
  if(WidgetIt != m_Widgets.end())
    return WidgetIt->second->clone();
  else
    throw InvalidArgumentException("Nuclex::GUI::GUIServer::createWidget()",
                                   string("Unknown widget type: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::getWidget()                                                       # //
// ############################################################################################# //
/** Retrieves a widget template by name

    @param  sName  Name of the widget template to retrieve
    @return The template widget
*/
const shared_ptr<Widget> &GUIServer::getWidget(const string &sName) const {
  WidgetMap::const_iterator WidgetIt = m_Widgets.find(sName);
  if(WidgetIt != m_Widgets.end())
    return WidgetIt->second;
  else
    throw InvalidArgumentException("Nuclex::GUI::GUIServer::getWidget()",
                                   string("Item not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::addWidget()                                                       # //
// ############################################################################################# //
/** Add a new widget template.

    @param  sName     Name under which to manage the template
    @param  spWidget  The template widget to add
*/
void GUIServer::addWidget(const string &sName, const shared_ptr<Widget> &spWidget) {
  WidgetMap::iterator WidgetIt = m_Widgets.find(sName);
  if(WidgetIt != m_Widgets.end())
    WidgetIt->second = spWidget;
  else
    m_Widgets.insert(WidgetMap::value_type(sName, spWidget));
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::removeWidget()                                                    # //
// ############################################################################################# //
/** Removes a widget template previously added using addWidget()

    @param  sName  Name under which the widget was added
*/
void GUIServer::removeWidget(const string &sName) {
  WidgetMap::iterator WidgetIt = m_Widgets.find(sName);
  if(WidgetIt != m_Widgets.end())
    m_Widgets.erase(WidgetIt);
  else
    throw InvalidArgumentException("Nuclex::GUI::GUIServer::removeWidget()",
                                   string("Item not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::clearWidgets()                                                    # //
// ############################################################################################# //
/** Removes all widget templates from the GUI server
*/
void GUIServer::clearWidgets() {
  m_Widgets.clear();
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::enumWidgets()                                                     # //
// ############################################################################################# //
/** Creates a new enumerator over all templates known to the GUI server

    @return A new enumerator for all registered widget templates
*/
shared_ptr<GUIServer::WidgetEnumerator> GUIServer::enumWidgets() const {
  throw NotSupportedException("Nuclex::GUI::GUIServer::enumWidgets()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::getWindow()                                                       # //
// ############################################################################################# //
/** Retrieves a window template by name

    @param  sName  Name of the window template to retrieve
    @return The template window
*/
const shared_ptr<Window> &GUIServer::getWindow(const string &sName) const {
  WindowMap::const_iterator WindowIt = m_Windows.find(sName);
  if(WindowIt != m_Windows.end())
    return WindowIt->second;
  else
    throw InvalidArgumentException("Nuclex::GUI::GUIServer::getWindow()",
                                   string("Item not found: '") + sName + "'");

}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::addWindow()                                                       # //
// ############################################################################################# //
/** Add a new window template

    @param  sName     Name under which to manage the window template
    @param  spWindow  Template window to be added
*/
void GUIServer::addWindow(const string &sName, const shared_ptr<Window> &spWindow) {
  WindowMap::iterator WindowIt = m_Windows.find(sName);
  if(WindowIt != m_Windows.end())
    WindowIt->second = spWindow;
  else
    m_Windows.insert(WindowMap::value_type(sName, spWindow));
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::removeWindow()                                                    # //
// ############################################################################################# //
/** Removes a template window previously added using addWindow()

    @param  sName  Name under which the window template was added
*/
void GUIServer::removeWindow(const string &sName) {
  WindowMap::iterator WindowIt = m_Windows.find(sName);
  if(WindowIt != m_Windows.end())
    m_Windows.erase(WindowIt);
  else
    throw InvalidArgumentException("Nuclex::GUI::GUIServer::removeWindow()",
                                   string("Item not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::clearWindows()                                                    # //
// ############################################################################################# //
/** Removes all window templates from the GUI server
*/
void GUIServer::clearWindows() {
  m_Windows.clear();
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::enumWindows()                                                     # //
// ############################################################################################# //
/** Create an enumerator for all registered window templates known to
    the GUI server

    @return A new enumerator over all template windows
*/
shared_ptr<GUIServer::WindowEnumerator> GUIServer::enumWindows() const {
  throw NotSupportedException("Nuclex::GUI::GUIServer::enumWindows()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::processInput()                                                    # //
// ############################################################################################# //
/** Processes an input event sent to the desktop window

    @param  InputEvent  The input event to be processed
*/
bool GUIServer::processInput(const Event &InputEvent) {
  if(InputEvent.eType == Event::T_MOUSEMOVE)
    m_Cursor = Point2<float>(InputEvent.Location, StaticCastTag());

  return m_spDesktop->processInput(InputEvent);
}

// ############################################################################################# //
// # Nuclex::GUI::GUIServer::draw()                                                            # //
// ############################################################################################# //
/** Draws the GUI. This call is directly handed to the desktop window's
    draw method.
    
    @param  VD  VertexDrawer to use for drawing
    @param  T   Theme to use for the GUI's look
*/    
void GUIServer::draw(Video::VertexDrawer &VD, Theme &T) {
  m_spDesktop->draw(VD, T);
/*
  T.drawCell(
    VD, "cursor", Box2<float>(m_Cursor, m_Cursor)
  );
*/  
}
