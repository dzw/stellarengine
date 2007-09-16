//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## DirectInput8InputDevice.cpp - DirectInput8 input device                   //
// ### # #      ###                                                                            //
// # ### #      ###  Default implementation of an input device which uses the                  //
// #  ## #   # ## ## operating system's mouse support routines                                 //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "DirectInput8Plugin/Input/DirectInput8InputDevice.h"
#include "Nuclex/Input/InputReceiver.h"
#include "Nuclex/Input/InputServer.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Application.h"
//#include "MemoryTracker/DisableMemoryTracker.h"
#include "Loki/SmallObj.h"
//#include "MemoryTracker/MemoryTracker.h"
#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>
using namespace std;

using namespace Nuclex;
using namespace Nuclex::Input;

namespace {

//  //
//  DirectInput8TriggerEnumerator                                                              //
//  //
class DirectInput8TriggerEnumerator :
  public InputDevice::TriggerEnumerator {
  public:
    DirectInput8TriggerEnumerator(const IDirectInputDevice8Ptr &spDevice) :
      m_CurrentTrigger(0) {
      DInputCheck(
        "DirectInput8TriggerEnumerator::DirectInput8TriggerEnumerator()",
        "IDirectInputDevice8::EnumObjects()",
        spDevice->EnumObjects(DIEnumDeviceObjectsCallback, this, DIDFT_ALL)
      );
    }

    /// Destructor
    virtual ~DirectInput8TriggerEnumerator() {}

  //
  // DirectInput8TriggerEnumerator
  //
  public:
    void addTrigger(const InputDevice::Trigger &Trigger) {
      m_Triggers.push_back(Trigger);
    }
  
  //
  // TriggerEnumerator implementation
  //
  public:
    /// Cycle through triggers
    const InputDevice::Trigger *cycle() {
      if(m_CurrentTrigger < m_Triggers.size())
        return &m_Triggers[m_CurrentTrigger++];
      else
        return NULL;
    }

  private:                    
    static BOOL CALLBACK DIEnumDeviceObjectsCallback(
      LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef
    ) {
      DirectInput8TriggerEnumerator &TheEnumerator =
        *static_cast<DirectInput8TriggerEnumerator *>(pvRef);
 
      InputDevice::Trigger Trigger;
      Trigger.sName = lpddoi->tszName;

      switch(DIDFT_GETINSTANCE(lpddoi->dwType)) {
        case DIDFT_RELAXIS: {
          Trigger.eKind = InputDevice::Trigger::K_AXIS;
          break;
        }
        case DIDFT_ABSAXIS: {
          Trigger.eKind = InputDevice::Trigger::K_SLIDER;
          break;
        }
        case DIDFT_PSHBUTTON: case DIDFT_TGLBUTTON: {
          Trigger.eKind = InputDevice::Trigger::K_BUTTON;
          break;
        }
        default:
          Trigger.eKind = InputDevice::Trigger::K_UNKNOWN;
      }
      
      TheEnumerator.addTrigger(Trigger);
      
      return DIENUM_CONTINUE;
    }
    
    std::vector<InputDevice::Trigger> m_Triggers;
    size_t                            m_CurrentTrigger;
};

} // namespace

//  //
//  Nuclex::Input::DirectInput8InputDevice::DirectInput8TriggerStates                          //
//  //
/*
class DirectInput8InputDevice::DirectInput8TriggerStates :
  public InputDevice::TriggerStates,
  public Loki::SmallObject<DEFAULT_THREADING, 36 * MaxSnapshotHistory, 36> {
  public:
    /// Constructor
    DirectInput8TriggerStates(const TimeSpan &SnapshotTime) :
      m_SnapshotTime() {}

    /// Destructor
    virtual ~DirectInput8TriggerStates() {}
     
    /// Get time it which snapshot was taken
    void setSnapshotTime(const TimeSpan &Time) {
      m_SnapshotTime = Time;
    }

  //
  // TriggerStates implementation
  //
  public:
    shared_ptr<TriggerStates> clone() const {
      return shared_ptr<TriggerStates>(new DirectInput8TriggerStates(*this));
    }

    /// Get time it which snapshot was taken
    const TimeSpan &getSnapshotTime() const {
      return m_SnapshotTime;
    }
    
    /// Get state of an input trigger
    real getTriggerState(const string &sTrigger) {
      return 0;
    }

  private:
    TimeSpan      m_SnapshotTime;
};
*/
// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::DirectInput8InputDevice()             Constructor # //
// ############################################################################################# //
/** Initializes an instance of DirectInput8InputDevice
*/
DirectInput8InputDevice::DirectInput8InputDevice(GUID InstanceGUID) :
  m_sName("DirectInput8 device") {
  
  DInputCheck(
    "Nuclex::Input::DirectInput8InputDevice::DirectInput8InputDevice()",
    "IDirectInput8::CreateDevice()",
    getDirectInput8()->CreateDevice(InstanceGUID, &m_spDirectInputDevice, NULL)
  );
  
  DIDEVICEINSTANCE DeviceInstance = { sizeof(DeviceInstance) };
  DInputCheck(
    "Nuclex::Input::DirectInput8InputDevice::DirectInput8InputDevice()",
    "IDirectInputDevice8::GetDeviceInfo()",
    m_spDirectInputDevice->GetDeviceInfo(&DeviceInstance)
  );
  
//  D
  
  m_sName = string(DeviceInstance.tszProductName) + " through DirectInput 8";
  m_eDeviceType = InputDeviceTypeFromDInputDevType(DeviceInstance.dwDevType);
}

// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::~DirectInput8InputDevice()             Destructor # //
// ############################################################################################# //
/** Releases an instance of DirectInput8InputDevice
*/
DirectInput8InputDevice::~DirectInput8InputDevice() {}

// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::enumTriggers()                                    # //
// ############################################################################################# //
/** Returns a new enumerator over all triggers of the input device.
    A trigger is an abstract representation of an axis, a key or
    any other kind of button on an input device.

    @return A new enumerator over all triggers
*/
shared_ptr<InputDevice::TriggerEnumerator> DirectInput8InputDevice::enumTriggers() const {
  return shared_ptr<InputDevice::TriggerEnumerator>(
    new DirectInput8TriggerEnumerator(m_spDirectInputDevice)
  ); 
}

// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::getTriggerState()                                 # //
// ############################################################################################# //
/*
shared_ptr<InputDevice::TriggerStates> DirectInput8InputDevice::snapTriggerStates() const {
  throw NotSupportedException(
    "Nuclex::Input::DirectInput8InputDevice::getTriggerState()",
    "Not implemented yet"
  );
}

// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::setSnapshotInterval()                             # //
// ############################################################################################# //
void DirectInput8InputDevice::setSnapshotInterval(const TimeSpan &Interval, size_t MaxHistory) {
  m_Interval = Interval;
}

// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::withdrawSnapshots()                               # //
// ############################################################################################# //
std::list<shared_ptr<InputDevice::TriggerStates> > DirectInput8InputDevice::withdrawSnapshots(const TimeSpan &EndTime) {
  std::list<shared_ptr<InputDevice::TriggerStates> > Snapshots;
  
  while(m_TriggerStateHistory.size() > 1) {
    if(m_TriggerStateHistory.front()->getSnapshotTime() > EndTime)
      break;

    Snapshots.push_back(m_TriggerStateHistory.front());
    m_TriggerStateHistory.pop_front();
  }
  
  return Snapshots;
}

// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::getCurrentStates()                                # //
// ############################################################################################# //
DirectInput8InputDevice::DirectInput8TriggerStates &DirectInput8InputDevice::getCurrentStates() {
  if(m_TriggerStateHistory.empty()) {
    TimeSpan Now = TimeSpan::getRunningTime();
    
    m_TriggerStateHistory.push_back(shared_ptr<DirectInput8TriggerStates>(
      new DirectInput8TriggerStates(Now - (Now % m_Interval))
    ));
  } else if(m_TriggerStateHistory.front()->getSnapshotTime() < TimeSpan::getRunningTime() - m_Interval) {
    TimeSpan Now = TimeSpan::getRunningTime();
  
    m_TriggerStateHistory.push_back(shared_ptr<DirectInput8TriggerStates>(
      new DirectInput8TriggerStates(*m_TriggerStateHistory.front().get())
    ));
    m_TriggerStateHistory.front()->setSnapshotTime(Now - (Now % m_Interval));
  }
  
  return *m_TriggerStateHistory.front().get();
}

// ############################################################################################# //
// # Nuclex::Input::DirectInput8InputDevice::updateTriggerstates()                             # //
// ############################################################################################# //
void DirectInput8InputDevice::updateTriggerStates() {
  DInputCheck(
    "Nuclex::Input::DirectInput8InputDevice::updateTriggerStates()",
    "IDirectInput8InputDevice::Poll()",
    m_spDirectInputDevice->Poll()
  );
}
*/

    /// Access the device in exclusive mode ?
void DirectInput8InputDevice::setExclusive(bool bExclusive) {
}

    /// Add control
void DirectInput8InputDevice::addControl(const string &sName, const string &sTrigger) {
}

    /// Remove control
void DirectInput8InputDevice::removeControl(const string &sName, const string &sTrigger) {
}

    /// Remove all controls
void DirectInput8InputDevice::clearControls() {
}

    /// Get control state
real DirectInput8InputDevice::getControl(const string &sName) {
  return 0;
}
