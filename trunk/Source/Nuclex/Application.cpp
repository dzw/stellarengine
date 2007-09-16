//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Application.cpp - Platform abstraction of an application                  //
// ### # #      ###                                                                            //
// # ### #      ###  Manages interaction of the application with the operating system,         //
// #  ## #   # ## ## eg. settings up and managing a window                                     //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Application.h"
#include "Resource.h"

using namespace Nuclex;

#ifdef NUCLEX_WIN32

/** Stores the number of opened rendering windows. Useful to track when
    the window class needs to be registered and when it can be unregistered
    again.
*/
size_t Application::m_nOpenWindows = 0;

#endif // NUCLEX_WIN32

// ############################################################################################# //
// # Nuclex::Application::Application()                                                        # //
// ############################################################################################# //
/** Initializes an instance of Application
 
    @param  sTitle  Application title (used as window title)
*/
Application::Application(const string &sTitle) :
#ifdef NUCLEX_WIN32
  m_hInstance(::GetModuleHandle(NULL)),
  m_hWindow(NULL),
  m_bOwnWindow(false),
  m_bFocused(false),
  m_bClose(false),
#endif
  m_sTitle(sTitle),
  m_bVisible(false) {
  
  createWindow(sTitle);
}

// ############################################################################################# //
// # Nuclex::Application::~Application()                                                       # //
// ############################################################################################# //
/** Releases an instance of Application
*/
Application::~Application() {
  destroyWindow();
}

// ############################################################################################# //
// # Nuclex::Application::heartBeat()                                                          # //
// ############################################################################################# //
/** Regular update
*/
bool Application::heartBeat() {
#ifdef NUCLEX_WIN32
  MSG Message;
  while(::PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
    ::TranslateMessage(&Message);
    ::DispatchMessage(&Message);
  }
#endif

  while(m_InputEvents.size()) {
    m_InputEvents.front()();
    m_InputEvents.pop_front();
  }
  
  bool bCloseRequested = m_bClose;
  return m_bClose = false, bCloseRequested;
}

// ############################################################################################# //
// # Nuclex::Application::setSize()                                                            # //
// ############################################################################################# //
/** Resizes the application

    @param  Size  Size in pixels to resize the application to
*/
void Application::setSize(const Point2<size_t> &Size) {
#ifdef NUCLEX_WIN32
  RECT Rect = { 0, 0, Size.X, Size.Y };
  if(!::AdjustWindowRectEx(
    &Rect,
    ::GetWindowLong(m_hWindow, GWL_STYLE),
    false,
    0
  ))
    throw UnexpectedException(
      "Nuclex::Application::resize()", "GetWindowLong() failed unexpectedly"
    );
  
  if(!SetWindowPos(
    m_hWindow,
    HWND_TOP,
    Rect.left, Rect.top,
    Rect.right - Rect.left, Rect.bottom - Rect.top,
    SWP_NOMOVE | SWP_NOZORDER
  ))
    throw UnexpectedException(
      "Nuclex::Application::resize()", "SetWindowPos() failed unexpectedly"
    );
#endif // NUCLEX_WIN32
}

// ############################################################################################# //
// # Nuclex::Application::show()                                                               # //
// ############################################################################################# //
/** Shows or hides the application

    @param  bShow  Whether to show the application
*/
void Application::show(bool bShow) {
#ifdef NUCLEX_WIN32
  ::ShowWindow(m_hWindow, bShow ? SW_SHOW : SW_HIDE);
#endif // NUCLEX_WIN32
}

#ifdef NUCLEX_WIN32
// ############################################################################################# //
// # Nuclex::Application::createWindow()                                                       # //
// ############################################################################################# //
/** Creates a new window

    @param  sName  Window name
    @param  hWnd   Existing window handle to use or NULL
*/
void Application::createWindow(const string &sName, HWND hWnd) {
  if(!hWnd) {
    if(m_nOpenWindows == 0) {
      WNDCLASSEX WindowClassEx;

      // Create and register our window class    
      WindowClassEx.cbSize        = sizeof(WindowClassEx);
      WindowClassEx.style         = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
      WindowClassEx.lpfnWndProc   = internalWindowProc;
      WindowClassEx.cbClsExtra    = 0;
      WindowClassEx.cbWndExtra    = 0;
      WindowClassEx.hInstance     = m_hInstance;
      WindowClassEx.hIcon         = ::LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_NUCLEX));
      WindowClassEx.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
      WindowClassEx.hbrBackground = NULL; //reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
      WindowClassEx.lpszMenuName  = NULL;
      WindowClassEx.lpszClassName = "Nuclex";
      WindowClassEx.hIconSm       = NULL;

      if(!::RegisterClassEx(&WindowClassEx))
        throw UnexpectedException("Nuclex::Application::createWindow()",
                                  "Unexpected API failure in RegisterClassEx()");
    }
    ++m_nOpenWindows;

    // Create the window
    hWnd = ::CreateWindowEx(
      WS_EX_APPWINDOW,
      "Nuclex",
      sName.c_str(),
      WS_POPUPWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
        WS_THICKFRAME | (m_bVisible ? WS_VISIBLE : DWORD(0)),
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      0, //::GetDesktopWindow(),
      NULL,
      m_hInstance,
      NULL
    );
    if(!hWnd)
      throw UnexpectedException("Nuclex::Application::createWindow()",
                                "Failed to create window");

    m_bOwnWindow = true;

  } else {
    if(::GetProp(m_hWindow, "Nuclex::Application"))
      throw InvalidArgumentException("Nuclex::Application::createWindow()",
                                     "This window is already used by another instance!");

    m_bOwnWindow = false;
  }

  m_hWindow = hWnd;
  ::SetProp(hWnd, "Nuclex::Application", reinterpret_cast<HANDLE>(this));
}

// ############################################################################################# //
// # Nuclex::Application::destroyWindow()                                                      # //
// ############################################################################################# //
/** Destroys the currently open window
*/
void Application::destroyWindow() {
  if(m_hWindow) {
    ::RemoveProp(m_hWindow, "Nuclex::Application");

    if(m_bOwnWindow) {
      HINSTANCE hInstance = ::GetModuleHandle(NULL);

      ::DestroyWindow(m_hWindow);
      
      --m_nOpenWindows;
      if(m_nOpenWindows == 0)
        ::UnregisterClass("Nuclex", hInstance);
    
      m_bOwnWindow = false;
    }

    m_hWindow = NULL;
  }
}

// ############################################################################################# //
// # Nuclex::Application::internalWindowProc()                                                 # //
// ############################################################################################# //
/** Internal window callback procedure. Delegates to the Application::windowProc() method.

    @param  hWnd      Window handle
    @param  uMessage  Message sent to the window
    @param  wParam    Word argument
    @param  lParam    Long argument
    @return Window message result
*/
LRESULT CALLBACK Application::internalWindowProc(
  HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam
) {
  // Get the pointer to the class owning this Window and give the
  // call on to the class's WindowProc()
  Application *pThis = reinterpret_cast<Application *>(
    ::GetProp(hWnd, "Nuclex::Application")
  );
  if(pThis)
    return pThis->windowProc(uMessage, wParam, lParam);

  // This point should not be reached, all windows should have
  // the Nuclex::Application value set
  return ::DefWindowProc(hWnd, uMessage, wParam, lParam);
}

// ############################################################################################# //
// # Nuclex::Application::windowProc()                                                         # //
// ############################################################################################# //
/** Window callback procedure

    @param  uMessage  Message sent to the window
    @param  wParam    Word argument
    @param  lParam    Long argument
    @return Window message result
*/
LRESULT CALLBACK Application::windowProc(UINT uMessage, WPARAM wParam, LPARAM lParam) {
  switch(uMessage) {

    // Window status changed
    case WM_ACTIVATE: {
      if(LOWORD(wParam) == WA_INACTIVE)
        m_bFocused = false;
      else
        m_bFocused = !HIWORD(wParam);

      break;
    }

    // Application status changed
    case WM_ACTIVATEAPP: {
      m_bFocused = !!wParam;

      break;
    }

    // Window close requested
    case WM_CLOSE: {
      m_bClose = true;

      return 0; // Prevent DefWindowProc from calling DestroyWindow()
    }

    // Window has been resized
    case WM_SIZE: {
      RECT Rect;
      ::GetClientRect(m_hWindow, &Rect);
      
      m_Size.set(Rect.right - Rect.left, Rect.bottom - Rect.top);

      break;
    }
    
    // Window has been moved
    case WM_PAINT: {
      ::ValidateRect(m_hWindow, NULL); // Validate everything

      return 0; // Prevent DefWindowProc from drawing
    }
    
    case WM_ERASEBKGND: {
      return TRUE; // Prevent DefWindowProc from erasing the background
    }

    // Cursor state set requested
    case WM_SETCURSOR: {
      if((m_Size.X > 0) && (m_Size.Y > 0)) {
        RECT Rect;
        POINT CursorPos;

        // All this code just to translate the ClientRect coordinates into
        // Screen coordinates...
        ::GetClientRect(m_hWindow, &Rect);
        CursorPos.x = Rect.left;
        CursorPos.y = Rect.top;
        ::ClientToScreen(m_hWindow, &CursorPos);
        Rect.left = CursorPos.x;
        Rect.top = CursorPos.y;
        CursorPos.x = Rect.right;
        CursorPos.y = Rect.bottom;
        ::ClientToScreen(m_hWindow, &CursorPos);
        Rect.right = CursorPos.x;
        Rect.bottom = CursorPos.y;

        // Obtain cursor position
        ::GetCursorPos(&CursorPos);

        // If it is inside the client area, hide the cursor
        if((CursorPos.x > Rect.left) && (CursorPos.x < Rect.right) &&
           (CursorPos.y > Rect.top)  && (CursorPos.y < Rect.bottom)) {
          ::SetCursor(NULL);

          return TRUE; // Prevent DefWindowProc from assigning the default cursor
        }
      }

      break;
    }

    // Character input
    case WM_CHAR: {
      m_InputEvents.push_back(MakeFunctorInvocation(OnChar, wParam));

      break;
    }

    // Key pressed
    case WM_KEYDOWN: {
      m_InputEvents.push_back(MakeFunctorInvocation(OnKeyDown, LOBYTE(HIWORD(lParam))));

      break;
    }

    // Key released
    case WM_KEYUP: {
      m_InputEvents.push_back(MakeFunctorInvocation(OnKeyUp, LOBYTE(HIWORD(lParam))));

      break;
    }
    
    // Left mouse button pressed
    case WM_LBUTTONDOWN: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonDown,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        0
      ));

      break;
    }

    // Left mouse button released
    case WM_LBUTTONUP: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonUp,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        0
      ));

      break;
    }

    // Right mouse button pressed
    case WM_RBUTTONDOWN: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonDown,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        1
      ));

      break;
    }

    // Right mouse button released
    case WM_RBUTTONUP: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonUp,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        1
      ));

      break;
    }

    // Middle mouse button pressed
    case WM_MBUTTONDOWN: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonDown,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        2
      ));

      break;
    }

    // Middle mouse button released
    case WM_MBUTTONUP: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonUp,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        2
      ));

      break;
    }

    // Extended mouse button pressed
    case WM_XBUTTONDOWN: { 
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonDown,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        GET_XBUTTON_WPARAM(wParam) - XBUTTON1 + 3
      ));
      
      break;
    }

    // Extended mouse button released
    case WM_XBUTTONUP: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseButtonUp,
        Point2<long>(LOWORD(lParam), HIWORD(lParam)),
        GET_XBUTTON_WPARAM(wParam) - XBUTTON1 + 3
      ));

      break;
    }

    // Mouse moved
    case WM_MOUSEMOVE: {
      m_InputEvents.push_back(MakeFunctorInvocation(
        OnMouseMove,
        Point2<long>(LOWORD(lParam), HIWORD(lParam))
      ));

      break;
    }

    // Mouse wheel rotated    
    case WM_MOUSEWHEEL: {
      m_InputEvents.push_back(MakeFunctorInvocation(OnMouseWheel, HIWORD(wParam)));

      break;
    }
  }

  return ::DefWindowProc(m_hWindow, uMessage, wParam, lParam);
}

#endif // NUCLEX_WIN32
