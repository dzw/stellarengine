//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## MouseInputDevice.cpp - Mouse input device                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Default implementation of an input device which uses the                  //
// #  ## #   # ## ## operating system's mouse support routines                                 //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Input/MouseInputDevice.h"
#include "Nuclex/Input/InputReceiver.h"
#include "Nuclex/Input/InputServer.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Application.h"
#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace Nuclex;
using namespace Nuclex::Input;

namespace {

//  //
//  MouseTriggerEnumerator                                                                     //
//  //
class MouseTriggerEnumerator :
  public InputDevice::TriggerEnumerator {
  public:
    MouseTriggerEnumerator(unsigned long nNumMouseButtons, bool bWheelPresent) :
      m_nNumMouseButtons(nNumMouseButtons),
      m_bWheelPresent(bWheelPresent),
      m_nCurrentTrigger(0) {}

    /// Destructor
    virtual ~MouseTriggerEnumerator() {}

  //
  // TriggerEnumerator implementation
  //
  public:
    /// Cycle through triggers
    const InputDevice::Trigger *cycle() {
      static        InputDevice::Trigger T;
      unsigned long nCurrentTrigger = m_nCurrentTrigger++;

      if(nCurrentTrigger == 0) {
        T.eKind = InputDevice::Trigger::K_AXIS;
        T.sName = "X Axis";
        return &T;
        
      } else if(nCurrentTrigger == 1) {
        T.eKind = InputDevice::Trigger::K_AXIS;
        T.sName = "Y Axis";
        return &T;
      } else {
        nCurrentTrigger -= 2;
      }

      if(nCurrentTrigger < m_nNumMouseButtons) {
        T.eKind = InputDevice::Trigger::K_BUTTON;
        T.sName = string("Button ") + lexical_cast<string>(nCurrentTrigger + 1);
        return &T;
      }
      
      nCurrentTrigger -= m_nNumMouseButtons;      
      
      if(m_bWheelPresent && (nCurrentTrigger == 0)) {
        T.eKind = InputDevice::Trigger::K_AXIS;
        T.sName = "Wheel";
        return &T;
      }
      
      // End reached
      return NULL;
    }

  private:                    
    unsigned long m_nNumMouseButtons;
    unsigned long m_nCurrentTrigger;
    bool          m_bWheelPresent;
};

} // namespace

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::MouseInputDevice()                                       # //
// ############################################################################################# //
/** Initializes an instance of MouseInputDevice

    @param  TheKernel  Kernel to which the input device belongs
*/
MouseInputDevice::MouseInputDevice(Kernel &TheKernel) :
  m_Kernel(TheKernel),
#ifdef NUCLEX_WIN32
  m_bMouseWheelPresent(::GetSystemMetrics(SM_MOUSEWHEELPRESENT) == TRUE),
  m_nNumMouseButtons(::GetSystemMetrics(SM_CMOUSEBUTTONS)),
#endif
  m_sName("Default Mouse") {

  Application &App = TheKernel.getApplication();
  App.OnMouseButtonDown.connect(sigc::mem_fun(this, &MouseInputDevice::onMouseButtonDown));
  App.OnMouseButtonUp.connect(sigc::mem_fun(this, &MouseInputDevice::onMouseButtonUp));
  App.OnMouseMove.connect(sigc::mem_fun(this, &MouseInputDevice::onMouseMove));
  App.OnMouseWheel.connect(sigc::mem_fun(this, &MouseInputDevice::onMouseWheel));
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::~MouseInputDevice()                                      # //
// ############################################################################################# //
/** Releases an instance of MouseInputDevice
*/
MouseInputDevice::~MouseInputDevice() {}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::available()                                              # //
// ############################################################################################# //
/** Returns true if a mouse is installed

    @return True if a mouse is installed
*/
bool MouseInputDevice::available() {
  return ::GetSystemMetrics(SM_MOUSEPRESENT) != FALSE;
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::getType()                                                # //
// ############################################################################################# //
/** Returns the type of the input device. This is only intended for
    informational purposes

    @return The device's type
*/
InputDevice::Type MouseInputDevice::getType() const {
  return InputDevice::T_MOUSE;
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::enumTriggers()                                           # //
// ############################################################################################# //
/** Returns a new enumerator over all triggers of the input device.
    A trigger is an abstract representation of an axis, a key or
    any other kind of button on an input device.

    @return A new enumerator over all triggers
*/
shared_ptr<InputDevice::TriggerEnumerator> MouseInputDevice::enumTriggers() const {
  return shared_ptr<InputDevice::TriggerEnumerator>(
    new MouseTriggerEnumerator(m_nNumMouseButtons, m_bMouseWheelPresent)
  ); 
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::onMouseButtonDown()                                      # //
// ############################################################################################# //
void MouseInputDevice::onMouseButtonDown(const Point2<long> &Location, unsigned char cButton) {
  m_Kernel.getInputServer()->generateInput(InputReceiver::Event(
    InputReceiver::Event::T_MOUSEBUTTONDOWN,
    InputReceiver::Event::V_NONE,
    cButton,
    Location
  ));
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::onMouseButtonUp()                                        # //
// ############################################################################################# //
void MouseInputDevice::onMouseButtonUp(const Point2<long> &Location, unsigned char cButton) {
  m_Kernel.getInputServer()->generateInput(InputReceiver::Event(
    InputReceiver::Event::T_MOUSEBUTTONUP,
    InputReceiver::Event::V_NONE,
    cButton,
    Location
  ));
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::onMouseMove()                                            # //
// ############################################################################################# //
void MouseInputDevice::onMouseMove(const Point2<long> &Location) {
  m_Kernel.getInputServer()->generateInput(InputReceiver::Event(
    InputReceiver::Event::T_MOUSEMOVE,
    InputReceiver::Event::V_NONE,
    Location
  ));
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::onMouseWheel()                                           # //
// ############################################################################################# //
void MouseInputDevice::onMouseWheel(long nWheel) {
  m_Kernel.getInputServer()->generateInput(InputReceiver::Event(
    InputReceiver::Event::T_WHEEL,
    InputReceiver::Event::V_NONE,
    nWheel
  ));
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::setExclusive()                                           # //
// ############################################################################################# //
/** Accessing a device in exclusive mode might provide additional
    benefits like infinite mouse movement range or usage of the
    alt and windows keys as triggers.
    
    @param  bExclusive  Whether to access the device in exclusive mode
*/
void MouseInputDevice::setExclusive(bool bExclusive) {
  if(bExclusive) {
#ifdef NUCLEX_WIN32
    HWND hWindow = m_Kernel.getApplication().getWindowHandle();

    RECT Rect;
    POINT CursorPos;

    // All this code just to translate the ClientRect coordinates into
    // Screen coordinates...
    ::GetClientRect(hWindow, &Rect);
    CursorPos.x = Rect.left;
    CursorPos.y = Rect.top;
    ::ClientToScreen(hWindow, &CursorPos);
    Rect.left = CursorPos.x;
    Rect.top = CursorPos.y;
    CursorPos.x = Rect.right;
    CursorPos.y = Rect.bottom;
    ::ClientToScreen(hWindow, &CursorPos);
    Rect.right = CursorPos.x;
    Rect.bottom = CursorPos.y;
    
    //::ClipCursor(&Rect);
#endif
  } else {
    //::ClipCursor(NULL);
  }
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::poll()                                                   # //
// ############################################################################################# //
/** Should be called regularly to update the device's state
*/
void MouseInputDevice::poll() {
//  ::GetCursorPos(
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::bind()                                                   # //
// ############################################################################################# //
/** Adds a control to the input device
    
    @param  sName     Name of the control
    @param  sTrigger  Trigger to be captured
*/
void MouseInputDevice::bind(const string &sName, const string &sTrigger) {
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::unbind()                                                 # //
// ############################################################################################# //
/** Removes a control from the input device

    @param  sName     Name of the control to remove
    @param  sTrigger  Trigger of the control to remove
*/
void MouseInputDevice::unbind(const string &sName, const string &sTrigger) {
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::clearBindings()                                          # //
// ############################################################################################# //
/** Removes all controls from the input device
*/
void MouseInputDevice::clearBindings() {
}

// ############################################################################################# //
// # Nuclex::Input::MouseInputDevice::getState()                                               # //
// ############################################################################################# //
/** Returns the current state of a registered control

    @param  sName  Name of the control whose state to retrieve
    @return The control's current state
*/
real MouseInputDevice::getState(const string &sName) const {
  return 0;
}
