//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## KeyboardInputDevice.h - Keyboard input device                             //
// ### # #      ###                                                                            //
// # ### #      ###  Default implementation of an input device which uses the                  //
// #  ## #   # ## ## operating system's keyboard support routines                              //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_INPUT_KEYBOARDINPUTDEVICE_H
#define NUCLEX_INPUT_KEYBOARDINPUTDEVICE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Input/InputDevice.h"
#include "SigC++/SigC++.h"
#include <deque>

namespace Nuclex {
  class Kernel;
}

namespace Nuclex { namespace Input {

//  //
//  Nuclex::Input::KeyboardInputDevice                                                         //
//  //
/// Keyboard
/** Default implementation of the keyboard as an input device.
*/
class KeyboardInputDevice :
  public InputDevice,
  public sigc::trackable {
  public:
    /// Get scancode of a key by its name
    NUCLEX_API static unsigned char getScancode(const string &sKeyName);
    /// Get name of a key from its scancode
    NUCLEX_API static string getKeyName(unsigned char cScancode);
  
    /// Constructor
    NUCLEX_API KeyboardInputDevice(Kernel &TheKernel);
    /// Destructor
    NUCLEX_API virtual ~KeyboardInputDevice();

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
    class KeyboardTriggerStates;

    /// Binding of an action to a control
    struct Binding {
      size_t                 Scancode;                ///< Scancode of the bound key
      bool                   bPreviousState;          ///< Previous key state
      Trigger::Interposition eInterposition;          ///< Modifier for the key
      size_t                 ControlIndex;            ///< Control which is triggered
    };
    typedef std::vector<Binding> BindingVector;

    /// Keyboard control binding
    struct Control {
      string sName;                                   ///< Name of the control
      float  fValue;                                  ///< Current control value
    };
    typedef std::vector<Control> ControlVector;

    /// Key was pressed
    void onKeyDown(unsigned char cKey);
    /// Key was released
    void onKeyUp(unsigned char cKey);
    /// Text was entered
    void onChar(wchar_t wChar);
    
    Kernel        &m_Kernel;                          ///< The kernel which owns this device
    string         m_sName;                           ///< The keyboard's name
    ControlVector  m_Controls;                        ///< Controls which have been bound
    BindingVector  m_Bindings;                        ///< Current list of bindings
};

}} // namespace Nuclex::Input

#endif // NUCLEX_INPUT_KEYBOARDINPUTDEVICE_H
