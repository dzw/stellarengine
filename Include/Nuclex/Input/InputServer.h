//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## InputServer.h - Input device manager                                      //
// ### # #      ###                                                                            //
// # ### #      ###  Manages all input devices and handles generation of input events          //
// #  ## #   # ## ## for standard input                                                        //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_INPUT_INPUTSERVER_H
#define NUCLEX_INPUT_INPUTSERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Input/InputDevice.h"
#include "Nuclex/Input/InputReceiver.h"
#include "SigC++/SigC++.h"
#include <map>

/*  Concept:

    The InputServer manages a set of input devices. An input device is a
    physical device accessed through a specific API.

    Virtual triggers can be registered to any button or axis of such a
    device together with a modifier which defines how the trigger will
    be activated.
    Something like this:
      Keyboard:KeyA.Press, Mouse:X.Axis, Joystick:Throttle.Value

    Need a way to gather input data a fixed number of times per second and
    make it easily accessable to the user.
    
    
              - Direct trigger value (digital oder analog)
    .Down     - Once when the trigger went down
    .Up       - Once when the trigger went up

    
    
    Controls unabhängig an- und abmeldbar ?
*/

namespace Nuclex {
  class Kernel;
}

namespace Nuclex { namespace Input {

class InputReceiver;

//  //
//  Nuclex::Input::InputServer                                                                 //
//  //
/// Nuclex input server
/**
*/
class InputServer :
  public sigc::trackable {
  public:
    /// Enumerator over a list of Input devices
    class DeviceEnumerator;

    /// Constructor
    NUCLEX_API InputServer();
    /// Destructor
    NUCLEX_API virtual ~InputServer();

  //
  // InputServer implementation
  //
  public:
    /// Retrieve device
    NUCLEX_API const shared_ptr<InputDevice> &getDevice(const string &sName) const;
    /// Add device
    NUCLEX_API void addDevice(const string &sName, const shared_ptr<InputDevice> &spDevice);
    /// Remove device
    NUCLEX_API void removeDevice(const string &sName);
    /// Remove all devices
    NUCLEX_API void clearDevices();
    /// Get renderer enumerator
    NUCLEX_API shared_ptr<DeviceEnumerator> enumDevices() const;

    /// Set interval for trigger snapshots
    NUCLEX_API void setSnapshotInterval(
      const TimeSpan &Interval,
      size_t MaxHistory = MaxSnapshotHistory
    );

    /// Update state on all devices
    NUCLEX_API void poll();

    /// Add a control
    NUCLEX_API void bind(const string &sControl, const string &sDevice, const string &sAction);
    /// Remove a control
    NUCLEX_API void unbind(const string &sControl, const string &sDevice, const string &sAction);
    /// Remove all controls
    NUCLEX_API void clearBindings();
    /// Get status of control
    NUCLEX_API real getState(const string &sControl) const;
    // enumControls
    // enumTriggersOfControl

    /// Generate an input event for the current receiver
    NUCLEX_API void generateInput(const InputReceiver::Event &InputEvent);

    /// Retrieve the current input receiver
    NUCLEX_API const shared_ptr<InputReceiver> &getReceiver() const { return m_spReceiver; }
    /// Set the input receiver
    NUCLEX_API void setReceiver(const shared_ptr<InputReceiver> &spReceiver = shared_ptr<InputReceiver>());

  private:
    /// Map of Input devices
    typedef std::map<string, shared_ptr<InputDevice> > DeviceMap;
    
    /// Binding of an action to a control
    struct Binding {
      string                              sDevice;    ///< Bound device
      string                              sTrigger;   ///< Bound trigger
      InputDevice::Trigger::Interposition eInterposition; ///< Modifier for the trigger
      string                              sControl;   ///< Control name
      size_t                              ControlIndex; ///< Control which is triggered
    };
    typedef std::vector<Binding> BindingVector;
    typedef std::vector<float> FloatVector;
    typedef std::map<string, size_t> IndexMap;

    DeviceMap                  m_Devices;             ///< Map of devices
    shared_ptr<InputReceiver>  m_spReceiver;          ///< Input event receiver
    IndexMap                   m_Controls;            ///< Controls which have been bound
    FloatVector                m_States;              ///< Current states of the bound controls
    BindingVector              m_Bindings;            ///< Current list of bindings
};

//  //
//  Nuclex::Input::InputServer::DeviceEnumerator                                               //
//  //
/** Enumerates all registered devices
*/
class InputServer::DeviceEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of DeviceIterator
    */
    NUCLEX_API virtual ~DeviceEnumerator() {}

  //
  // DeviceEnumerator implementation
  //
  public:
    /// Cycle through devices
    /** Returns the current device being enumerated and advances
        to the next. If no more devices are remaining, NULL is returned

        @return The currently enumerated renderer
    */
    NUCLEX_API virtual const shared_ptr<InputDevice> &cycle() = 0;
};

}} // namespace Nuclex::Input

#endif // NUCLEX_INPUT_INPUTSERVER_H
