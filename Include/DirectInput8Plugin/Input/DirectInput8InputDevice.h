//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## DirectInput8InputDevice.h - DirectInput8 input device                     //
// ### # #      ###                                                                            //
// # ### #      ###  Default implementation of an input device which uses the                  //
// #  ## #   # ## ## operating system's mouse support routines                                 //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_INPUT_DIRECTINPUT8INPUTDEVICE_H
#define NUCLEX_INPUT_DIRECTINPUT8INPUTDEVICE_H

#include "DirectInput8Plugin/DirectInput8Plugin.h"
#include "Nuclex/Math/Point2.h"
#include "Nuclex/Input/InputDevice.h"
#include "SigC++/SigC++.h"
#include <deque>

namespace Nuclex {
  class Kernel;
}

namespace Nuclex { namespace Input {

//  //
//  Nuclex::Input::DirectInput8InputDevice                                                     //
//  //
///
/** 
*/
class DirectInput8InputDevice :
  public InputDevice {
  public:
    /// Constructor
    NUCLEXDIRECTINPUT8_API DirectInput8InputDevice(GUID InstanceGUID);
    /// Destructor
    NUCLEXDIRECTINPUT8_API virtual ~DirectInput8InputDevice();
    
  //
  // InputDevice implementation
  //
  public:
    /// Get device type
    NUCLEXDIRECTINPUT8_API Type getType() const { return m_eDeviceType; }

    /// Get input device name
    NUCLEXDIRECTINPUT8_API const string &getName() const { return m_sName; }

    /// Get new trigger enumerator
    NUCLEXDIRECTINPUT8_API shared_ptr<TriggerEnumerator> enumTriggers() const;
    
    /// Access the device in exclusive mode ?
    NUCLEXDIRECTINPUT8_API void setExclusive(bool bExclusive = true);

    /// Add control
    NUCLEXDIRECTINPUT8_API void addControl(const string &sName, const string &sTrigger);

    /// Remove control
    NUCLEXDIRECTINPUT8_API void removeControl(const string &sName, const string &sTrigger);

    /// Remove all controls
    NUCLEXDIRECTINPUT8_API void clearControls();

    /// Get control state
    NUCLEXDIRECTINPUT8_API real getControl(const string &sName);

/*
    /// Get input trigger states snapshot
    NUCLEXDIRECTINPUT8_API shared_ptr<TriggerStates> snapTriggerStates() const;
    
    /// Set interval for trigger snapshots
    NUCLEXDIRECTINPUT8_API void setSnapshotInterval(const TimeSpan &Interval, size_t MaxHistory = MaxSnapshotHistory);

    /// Get input trigger states snapshot
    NUCLEXDIRECTINPUT8_API std::list<shared_ptr<TriggerStates> > withdrawSnapshots(const TimeSpan &EndTime);
*/
  private:
    class DirectInput8TriggerStates;

    /// Update trigger states from DirectInput
    void updateTriggerStates();

    DirectInput8TriggerStates &getCurrentStates();

    typedef std::deque<shared_ptr<DirectInput8TriggerStates> > DirectInput8TriggerStateDeque;
  
    string                        m_sName;
    DirectInput8TriggerStateDeque m_TriggerStateHistory;
    TimeSpan                      m_Interval;
    
    IDirectInputDevice8Ptr        m_spDirectInputDevice;
    Type                          m_eDeviceType;
};

}} // namespace Nuclex::Input

#endif // NUCLEX_INPUT_DIRECTINPUT8INPUTDEVICE_H
