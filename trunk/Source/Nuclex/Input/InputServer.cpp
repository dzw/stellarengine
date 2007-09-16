//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## InputServer.cpp - Input device manager                                    //
// ### # #      ###                                                                            //
// # ### #      ###  Manages all input devices and handles generation of input events          //
// #  ## #   # ## ## for standard input                                                        //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Input/InputServer.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Input/InputDevice.h"
#include "Nuclex/Input/MouseInputDevice.h"
#include "Nuclex/Input/KeyboardInputDevice.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Nuclex::Input;

namespace {

//  //
//  InputDeviceEnumerator                                                                      //
//  //
/// Device enumerator for Input server
/** Enumerates over all devices registered to the Input server
*/
class InputDeviceEnumerator :
  public InputServer::DeviceEnumerator {
  typedef std::map<string, shared_ptr<InputDevice> > DeviceMap;
  public:
    /// Constructor
    /** Initializes an instance of InputDeviceEnumerator

        @param  DeviceMap  Devices to enumerate
    */
    InputDeviceEnumerator(const DeviceMap &Devices) :
      m_DeviceIt(Devices.begin()),
      m_DeviceEnd(Devices.end()) {}

    /// Destructor
    /** Destroys an instance of InputDeviceIterator
    */
    virtual ~InputDeviceEnumerator() {}

  //
  // InputDeviceEnumerator implementation
  //
  public:
    /// Cycle through input devices
    /** Returns the current device being enumerated and advances
        to the next. If no more devices are remaining, NULL is returned

        @return The currently enumerated device
    */
    const shared_ptr<InputDevice> &cycle() {
      static const shared_ptr<InputDevice> spEmpty;

      if(m_DeviceIt != m_DeviceEnd) {
        const shared_ptr<InputDevice> &spDevice = m_DeviceIt->second;
        m_DeviceIt++;
        return spDevice;
      } else {
        return spEmpty;
      }
    }

  private:
    DeviceMap::const_iterator m_DeviceIt;             ///< Current renderer
    DeviceMap::const_iterator m_DeviceEnd;            ///< List end
};

} // namespace

// ############################################################################################# //
// # Nuclex::Input::InputServer::InputServer()                                     Constructor # //
// ############################################################################################# //
/** Creates an instance of InputServer
*/
InputServer::InputServer() {}

// ############################################################################################# //
// # Nuclex::Input::InputServer::~InputServer()                                     Destructor # //
// ############################################################################################# //
/** Destroys an instance of InputServer
*/
InputServer::~InputServer() {}

// ############################################################################################# //
// # Nuclex::Input::InputServer::getDevice()                                                   # //
// ############################################################################################# //
/** Retrieves a Input device by name

    @param  sName     Name of the Input device to retrieve
    @return The Input device or NULL if not found
*/
const shared_ptr<InputDevice> &InputServer::getDevice(const string &sName) const {
  DeviceMap::const_iterator DeviceIt = m_Devices.find(sName);
  if(DeviceIt == m_Devices.end())
    throw InvalidArgumentException("Nuclex::Input::InputServer::getDevice()",
                                   string("Item not found: '") + sName + "'");

  return DeviceIt->second;
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::addDevice()                                                   # //
// ############################################################################################# //
/** Add a new Input device.

    @param  sName     Name of the Input device
    @param  spDevice  The Input device to add
*/
void InputServer::addDevice(const string &sName, const shared_ptr<InputDevice> &spDevice) {
  DeviceMap::iterator DeviceIt = m_Devices.find(sName);
  if(DeviceIt != m_Devices.end())
    DeviceIt->second = spDevice;
   else
    m_Devices.insert(DeviceMap::value_type(sName, spDevice));
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::removeDevice()                                                # //
// ############################################################################################# //
/** Removes a Input device previously added using addDevice()

    @param  sName  Name of the Input device to remove
*/
void InputServer::removeDevice(const string &sName) {
  DeviceMap::iterator DeviceIt = m_Devices.find(sName);
  if(DeviceIt == m_Devices.end())
    throw InvalidArgumentException("Nuclex::InputServer::getDevice()",
                                   string("Item not found: '") + sName + "'");
  else
    m_Devices.erase(DeviceIt);
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::clearDevices()                                                # //
// ############################################################################################# //
/** Removes all Input devices currently added to the Input server
*/
void InputServer::clearDevices() {
  m_Devices.clear();
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::enumDevices()                                                 # //
// ############################################################################################# //
/** Returns a new enumerator over all Input devices registered to
    the Input server.

    @return A new enumerator over all Input devices
*/
shared_ptr<InputServer::DeviceEnumerator> InputServer::enumDevices() const {
  return shared_ptr<InputServer::DeviceEnumerator>(new InputDeviceEnumerator(m_Devices));
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::setSnapshotInterval()                                         # //
// ############################################################################################# //
/** Sets the interval at which to take snapshots of the device states

    @param  Interval    Interval at which to take snapshots
    @param  MaxHistory  Maximum number of snapshots to store before trashing
*/
void InputServer::setSnapshotInterval(const TimeSpan &Interval, size_t MaxHistory) {
/*
  for(DeviceMap::iterator DeviceIt = m_Devices.begin();
      DeviceIt != m_Devices.end();
      ++DeviceIt)
    DeviceIt->second->setSnapshotInterval(Interval, MaxHistory);
*/
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::poll()                                                        # //
// ############################################################################################# //
void InputServer::poll() {
  for(DeviceMap::iterator It = m_Devices.begin(); It != m_Devices.end(); ++It)
    It->second->poll();
    
  for(size_t ControlIndex = 0; ControlIndex < m_States.size(); ++ControlIndex)
    m_States[ControlIndex] = 0.0f;
    
  for(size_t BindingIndex = 0; BindingIndex < m_Bindings.size(); ++BindingIndex) {
    shared_ptr<InputDevice> spDevice = getDevice(m_Bindings[BindingIndex].sDevice);

    m_States[m_Bindings[BindingIndex].ControlIndex] = max(
      m_States[m_Bindings[BindingIndex].ControlIndex],
      spDevice->getState(m_Bindings[BindingIndex].sControl)
    );
  }
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::bind()                                                        # //
// ############################################################################################# //
void InputServer::bind(const string &sControl, const string &sDevice, const string &sAction) {
  shared_ptr<InputDevice> spDevice = getDevice(sDevice);
  
  IndexMap::iterator ControlIt = m_Controls.find(sControl);
  if(ControlIt == m_Controls.end()) {
    size_t StateIndex = m_States.size();
    m_States.resize(StateIndex + 1);
    m_States[StateIndex] = 0.0f;
    ControlIt = m_Controls.insert(IndexMap::value_type(sControl, StateIndex)).first;
    ControlIt->second = StateIndex;
  }
  
  size_t BindingIndex = m_Bindings.size();
  m_Bindings.resize(BindingIndex + 1);
  m_Bindings[BindingIndex].sDevice = sDevice;
  m_Bindings[BindingIndex].sTrigger = InputDevice::Trigger::getTrigger(sAction);
  m_Bindings[BindingIndex].eInterposition = InputDevice::Trigger::getInterposition(sAction);
  m_Bindings[BindingIndex].ControlIndex = ControlIt->second;
  m_Bindings[BindingIndex].sControl = sControl;
  
  spDevice->bind(sControl, sAction);
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::unbind()                                                      # //
// ############################################################################################# //
void InputServer::unbind(const string &sControl, const string &sDevice, const string &sAction) {
  shared_ptr<InputDevice> spDevice = getDevice(sDevice);
  
  IndexMap::iterator ControlIt = m_Controls.find(sControl);
  if(ControlIt == m_Controls.end())
    throw FailedException(
      "Nuclex::Input::InputServer::unbind()",
      "A control with the specified name and action was not found"
    );
  
  spDevice->unbind(sControl, sAction);  
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::clearBindings()                                               # //
// ############################################################################################# //
void InputServer::clearBindings() {
  for(DeviceMap::iterator It = m_Devices.begin(); It != m_Devices.end(); ++It)
    It->second->clearBindings();
    
  m_Bindings.clear();
  m_States.clear();
  m_Controls.clear();
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::getState()                                                    # //
// ############################################################################################# //
float InputServer::getState(const string &sControl) const {
  IndexMap::const_iterator ControlIt = m_Controls.find(sControl);
  
  if(ControlIt != m_Controls.end())
    return m_States[ControlIt->second];
  else
    return 0.0f;
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::generateInput()                                               # //
// ############################################################################################# //
void InputServer::generateInput(const InputReceiver::Event &InputEvent) {
  if(m_spReceiver)
    m_spReceiver->processInput(InputEvent);
}

// ############################################################################################# //
// # Nuclex::Input::InputServer::setReceiver()                                                 # //
// ############################################################################################# //
void InputServer::setReceiver(const shared_ptr<InputReceiver> &spReceiver) {
  m_spReceiver = spReceiver;
}

