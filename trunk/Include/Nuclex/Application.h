//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Application.h - Platform abstraction of an application                    //
// ### # #      ###                                                                            //
// # ### #      ###  Manages interaction of the application with the operating system,         //
// #  ## #   # ## ## eg. settings up and managing a window                                     //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_APPLICATION_H
#define NUCLEX_APPLICATION_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Invocation.h"
#include "Nuclex/Math/Point2.h"
#include "SigC++/SigC++.h"
#include <deque>

#ifdef NUCLEX_WIN32
  #define WIN32_MEAN_AND_LEAN
  #include <windows.h>
#endif

namespace Nuclex {

//  //
//  Nuclex::Application                                                                        //
//  //
/// Application representation
/** On desktop systems, this class typically controls a window which is
    required to properly support task switching amongst other things.
    
    You will not interact with this class under normal circumstances,
    except maybe for setting a custom application title (window title).
    It is not at all suited as an application class from which you derive
    your own application class and should only be seen as a mean of
    hacking into nuclex for usage on custom dialogs and such.
*/
class Application {
  public:
    /// Signal for when application focus changes
    typedef SigC::Signal1<void, bool> FocusSignal;
    /// Signal for when a key is pressed or released
    typedef SigC::Signal1<void, unsigned char> KeySignal;
    /// Signal for when text is entered
    typedef SigC::Signal1<void, wchar_t> TextSignal;
    /// Signal for when the mouse wheel is rotated
    typedef SigC::Signal1<void, long> WheelSignal;
    /// Signal for when the mouse is moved
    typedef SigC::Signal1<void, const Point2<long> &> MoveSignal;
    /// Signal for when a mouse button is pressed or released
    typedef SigC::Signal2<void, const Point2<long> &, unsigned char> ButtonSignal;

    /// Constructor
    NUCLEX_API Application(const string &sTitle = "Nuclex");
    /// Destructor
    NUCLEX_API virtual ~Application();

  //
  // Application implementation
  //
  public:
    /// Perform regular update and message loop
    NUCLEX_API bool heartBeat();

#ifdef NUCLEX_WIN32
    /// Get application instance handle
    NUCLEX_API HINSTANCE getInstanceHandle() const { return m_hInstance; }
    /// Retrieve the application's HWND under Win32 systems
    NUCLEX_API HWND getWindowHandle() const { return m_hWindow; }
#endif

    /// Retrieve the application's title, where applicable
    NUCLEX_API const string &getTitle() const { return m_sTitle; }
    /// Set the application's title, where applicable
    NUCLEX_API void setTitle(const string &sTitle);
    
    /// Check whether the application is currently visible
    NUCLEX_API bool isVisible() const { return m_bVisible; }
    /// Show or hide the application
    NUCLEX_API void show(bool bShow = true);

    /// Retrieve the application's size
    NUCLEX_API const Point2<size_t> &getSize() { return m_Size; }
    /// Resize the application
    NUCLEX_API void setSize(const Point2<size_t> &Size);

    /// Check whether the application is in focus
    NUCLEX_API bool isActive() const { return m_bFocused; }

    /// Application has gotten or lost focus
    FocusSignal OnFocusChanged;

    /// Signalled when a key is pressed
    KeySignal OnKeyDown;
    /// Signalled when a key is released
    KeySignal OnKeyUp;
    /// OS supported text input
    TextSignal OnChar;
    
    /// Signalled when mouse button is pressed
    ButtonSignal OnMouseButtonDown;
    /// Signalled when mouse button is released
    ButtonSignal OnMouseButtonUp;
    /// Signalled when mouse is moved
    MoveSignal OnMouseMove;
    /// Signalled when mouse wheel is rotated
    WheelSignal OnMouseWheel;

  private:
#ifdef NUCLEX_WIN32
    /// Queue of signals that have to be delivered
    typedef std::deque<Invocation> InvocationDeque;

    /// Create a new win32 window
    void createWindow(const string &sName, HWND hWnd = NULL);
    /// Destroy the opened win32 window
    void destroyWindow();

    /// Internal window procedure
    static LRESULT CALLBACK internalWindowProc(
      HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam
    );
    /// Class member window procedure
    LRESULT CALLBACK windowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

    static size_t   m_nOpenWindows;                   ///< Number of open rendering windows

    HINSTANCE       m_hInstance;                      ///< Application instance handle
    HWND            m_hWindow;                        ///< Win32 window handle
    bool            m_bOwnWindow;                     ///< Owns the window ?
    bool            m_bFocused;                       ///< Application has focus ?
    bool            m_bClose;                         ///< Window close requested ?
    InvocationDeque m_InputEvents;                    ///< Queued input events
#endif
    Point2<size_t>  m_Size;                           ///< Application display size
    bool            m_bVisible;                       ///< Application is visible ?
    string          m_sTitle;                         ///< Application title
};

} // namespace Nuclex

#endif // NUCLEX_APPLICATION_H
