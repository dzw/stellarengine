//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## MouseInputDevice.h - Mouse input device                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Default implementation of an input device which uses the                  //
// #  ## #   # ## ## operating system's mouse support routines                                 //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_INPUT_MOUSEINPUTDEVICE_H
#define NUCLEX_INPUT_MOUSEINPUTDEVICE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Point2.h"
#include "Nuclex/Input/InputDevice.h"
#include "SigC++/SigC++.h"
#include <deque>

namespace Nuclex {
  class Kernel;
}

namespace Nuclex { namespace Input {

//  //
//  Nuclex::Input::MouseInputDevice                                                            //
//  //
/// Mouse
/** Default implementation of an input device representing the system mouse
*/
class MouseInputDevice :
  public InputDevice,
  public sigc::trackable {
  public:
    /// Return whether a mouse is available
    static bool available();
    
    /// Constructor
    NUCLEX_API MouseInputDevice(Kernel &TheKernel);
    /// Destructor
    NUCLEX_API virtual ~MouseInputDevice();
    
  //
  // InputDevice implementation
  //
  public:
    /// Get device type
    NUCLEX_API Type getType() const;

    /// Get input device name
    NUCLEX_API const string &getName() const { return m_sName; }

    /// Get new trigger enumerator
    NUCLEX_API shared_ptr<TriggerEnumerator> enumTriggers() const;

    /// Access the device in exclusive mode ?
    NUCLEX_API void setExclusive(bool bExclusive = true);

    /// Update device state
    NUCLEX_API void poll();

    /// Add control
    NUCLEX_API void bind(const string &sControl, const string &sAction);

    /// Remove control
    NUCLEX_API void unbind(const string &sControl, const string &sAction);

    /// Remove all controls
    NUCLEX_API void clearBindings();

    /// Get control state
    NUCLEX_API real getState(const string &sControl) const;

  private:
    class MouseTriggerStates;

    /// Mouse button was pressed
    void onMouseButtonDown(const Point2<long> &Location, unsigned char cButton);
    /// Mouse button was released
    void onMouseButtonUp(const Point2<long> &Location, unsigned char cButton);
    /// Mouse was moved
    void onMouseMove(const Point2<long> &Location);
    /// Mouse wheel was rotated
    void onMouseWheel(long nWheel);
    /// Application has gotten or lost focus
    void onFocusChanged(bool bFocused);

    Kernel                 &m_Kernel;
    string                  m_sName;
    size_t                  m_nNumMouseButtons;
    bool                    m_bMouseWheelPresent;
    bool                    m_bFocused;
};

}} // namespace Nuclex::Input

#endif // NUCLEX_INPUT_MOUSEINPUTDEVICE_H
