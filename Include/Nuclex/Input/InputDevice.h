//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## InputDevice.h - Input device                                              //
// ### # #      ###                                                                            //
// # ### #      ###  A device through which the user can trigger input events,                 //
// #  ## #   # ## ## like a keyboard or a mouse for example                                    //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_INPUT_INPUTDEVICE_H
#define NUCLEX_INPUT_INPUTDEVICE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Math/Math.h"
#include "Nuclex/Support/TimeSpan.h"
#include "Nuclex/Input/Input.h"
#include <list>

// Problem:
// 
// Irgendwie müssen die Achsen wieder zurückgesetzt werden, da Achsen
// grundsätzlich relativ sind.
//
// Texteingabe wird von KeyboardDevice direkt an InputServer gegeben ?
//
// Modus-Schalter: CaptureMouse und CaptureText ?
//
//
// Komplett anderes vorgehen statt snapshots ?
//   Wenn eine Eingabebindung registriert wird, dann reicht der InputServer
//   diese an die entsprechenden Devices weiter. Die Devices schicken
//   dann die Daten mit TimeStamps beim auftreten an den InputServer.
//
//  +> Code für Snapshot-generierung zentral in InputServer
//  -> TimeStamps müssen vorhanden und zwischen Geräten synchronisiert sein
//  -> Code für Input-Funktionen in jedem Eingabegerät
//
// Resultat: Snapshot-Technik auf Device-Seite einfacher zu implementieren
//
// Nomenclature
//
//   InputDevice   - Abstract definition of an input device, eg. a keyboard or mouse
//   Trigger       - Abstract definition of a trigger on an input device, including axes
//   Control       - Abstract input function a game understands. A shooter could have
//                   controls like "MoveForward", "MoveBackward", "Fire", "Jump"
//   Interposition - How a triggers are to be dispatched to controls
//   Action        - What has to be done to fire a control. Compound of a trigger and
//                   an interposition specifying what has to be done with the trigger
//                   to fire the control
//

namespace Nuclex { namespace Input {

//  //
//  Nuclex::Input::InputDevice                                                                 //
//  //
///
/** 
*/
class InputDevice {
  public:
    /// Device types 
    enum Type {
      T_UNKNOWN = 0,                                  ///< Unknown device type
      T_KEYBOARD,                                     ///< Keyboard
      T_MOUSE,                                        ///< Mouse or trackball
      T_GAMEPAD,                                      ///< Gamepad
      T_JOYSTICK,                                     ///< Joystick
      T_WHEEL                                         ///< Steering wheel
    };

    /// Input trigger
    struct Trigger {
      enum Kind {
        K_UNKNOWN = 0,                                ///< Unknown trigger kind
        K_BUTTON,                                     ///< Digital button
        K_SLIDER,                                     ///< Limited slider
        K_AXIS                                        ///< Unlimited axis
      };

      /// Trigger interposition
      enum Interposition {
        I_DIRECT,                                     ///< Direct trigger value
        I_DOWN,                                       ///< When trigger goes down
        I_UP                                          ///< When trigger goes up
      };

      /// Get modifier of action
      static Interposition getInterposition(const string &sAction);
      /// Get trigger of action
      static string getTrigger(const string &sAction);

      Kind   eKind;                                   ///< Which kind of trigger is it ?
      string sName;                                   ///< Human readable name
    };
    class TriggerEnumerator;

    /// Destructor
    NUCLEX_API virtual ~InputDevice() {}

  //
  // InputDevice implementation
  //
  public:
    /// Get device type
    /** Returns the type of the input device. This is only intended for
        informational purposes

        @return The device's type
    */
    NUCLEX_API virtual Type getType() const = 0;

    /// Get input device name
    /** Returns a name describing the input device. This name can be
        displayed to the user to specify which physical device is meant.

        @return The input device's name
    */
    NUCLEX_API virtual const string &getName() const = 0;

    /// Get new trigger enumerator
    /** Returns a new enumerator over all triggers of the input device.
        A trigger is an abstract representation of an axis, a key or
        any other kind of button on an input device.

        @return A new enumerator over all triggers
    */
    NUCLEX_API virtual shared_ptr<TriggerEnumerator> enumTriggers() const = 0;
    
    /// Access the device in exclusive mode ?
    /** Accessing a device in exclusive mode might provide additional
        benefits like infinite mouse movement range or usage of the
        alt and windows keys as triggers.
        
        @param  bExclusive  Whether to access the device in exclusive mode
    */
    NUCLEX_API virtual void setExclusive(bool bExclusive = true) {}

    /// Update device state
    /** Should be called regularly to update the device's state
    */
    NUCLEX_API virtual void poll() {}

    /// Add control
    /** Adds a control to the input device
        
        @param  sName     Name of the control
        @param  sTrigger  Trigger to be captured
    */
    NUCLEX_API virtual void bind(const string &sControl, const string &sAction) = 0;
    
    /// Remove control
    /** Removes a control from the input device
    
        @param  sName     Name of the control to remove
        @param  sTrigger  Trigger of the control to remove
    */
    NUCLEX_API virtual void unbind(const string &sControl, const string &sAction) = 0;
    
    /// Remove all controls
    /** Removes all controls from the input device
    */
    NUCLEX_API virtual void clearBindings() = 0;

    /// Get control state
    /** Returns the current state of a registered control
    
        @param  sName  Name of the control whose state to retrieve
        @return The control's current state
    */
    NUCLEX_API virtual real getState(const string &sControl) const = 0;
};

//  //
//  Nuclex::Input::InputDevice::TriggerEnumerator                                              //
//  //
/// Enumerates a list of triggers
class InputDevice::TriggerEnumerator {
  public:
    /// Destructor
    virtual ~TriggerEnumerator() {}

  //
  // TriggerEnumerator implementation
  //
  public:
    /// Cycle through triggers
    virtual const InputDevice::Trigger *cycle() = 0;
};

}} // namespace Nuclex::Input

#endif // NUCLEX_INPUT_INPUTDEVICE_H
