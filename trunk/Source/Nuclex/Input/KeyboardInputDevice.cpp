//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## KeyboardInputDevice.cpp - Keyboard input device                           //
// ### # #      ###                                                                            //
// # ### #      ###  Default implementation of an input device which uses the                  //
// #  ## #   # ## ## operating system's keyboard support routines                              //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Input/KeyboardInputDevice.h"
#include "Nuclex/Application.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Input/InputServer.h"
#include <map>

using namespace Nuclex;
using namespace Nuclex::Input;

namespace {
 
//  //
//  KeyboardTriggers                                                                           //
//  //
/// Scancode/Keyname translation
/** Handles the translaten of scancodes to keynames and vice versa
*/
class KeyboardTriggers {
  public:
    /// Constructor
    KeyboardTriggers() {
#ifdef NUCLEX_WIN32
      // Win32: GetKeyNameText()
      for(size_t i = 0; i < 256; ++i) {
        char pszKeyName[256];

        if(::GetKeyNameText((i << 16) | (1 << 25), pszKeyName, sizeof(pszKeyName)))
          m_Scancodes[pszKeyName] = i;
      }
#else
      const char *ppszKeyNames[] = {
        NULL, "Escape", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Minus", "Equals", "Backspace", "Tab",
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "LeftBracket", "RightBracket", "Return", "LeftControl", "A", "S",
        "D", "F", "G", "H", "J", "K", "L", "Semicolon", "Apostrophe", "Grave", "LeftShift", "Backslash", "Z", "X", "C", "V",
        "B", "N", "M", "Comma", "Period", "Slash", "RightShift", "Multiply", "LeftMenu", "Space", "Capital", "F1", "F2", "F3", "F4", "F5",
        "F6", "F7", "F8", "F9", "F10", "NumLock", "Scroll", "NumHome", "Up", "NumPageUp", "Subtract", "Left", "NumCenter", "Right", "Add", "NumEnd",
        "Down", "NumPageDown", "NumInsert", "Decimal", NULL, NULL, "OEM102", "F11", "F12", NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, "F13", "F14", "F15", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Kana", NULL, NULL, "AbntC1", NULL, NULL, NULL, NULL, NULL, "Convert", NULL, "NoConvert", NULL, "Yen", "AbntC2", NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "NumEquals", NULL, NULL,
        "PrevTrack", "At", "Colon", "Underline", "Kanji", "Stop", "Ax", "Unlabelled", NULL, "NextTrack", NULL, NULL, "NumEnter", "RightControl", NULL, NULL,
        "Mute", "Calculator", "Play", NULL, "MediaStop", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "VolumeDown", NULL,
        "VolumeUp", NULL, "WebHome", "NumComma", NULL, "Divide", NULL, "SysRequest", "RightMenu", NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, "Pause", NULL, "Home", "NumUp", "PageUp", NULL, "NumLeft", NULL, "NumRight", NULL, "End",
        "NumDown", "PageDown", "Insert", "Delete", NULL, NULL, NULL, NULL, NULL, NULL, NULL, "LeftWin", "RightWin", "AppMenu", "Power", "Sleep",
        NULL, NULL, NULL, "Wake", NULL, "WebSearch", "WebFavorites", "WebRefresh", "WebStop", "WebForward", "WebBackward", "MyComputer", "Mail", "MediaSelect", NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
      };

      for(size_t i = 0; i < sizeof(ppszKeyNames) / sizeof(*ppszKeyNames); ++i)
        if(ppszKeyNames[i])
          m_Scancodes[ppszKeyNames[i]] = i;
#endif
    }

    /// Retrieves the scancode of a key
    unsigned char getScancode(const string &sTrigger) const {
      CharMap::const_iterator CharIt = m_Scancodes.find(sTrigger);
      if(CharIt == m_Scancodes.end()) {
        string::size_type Pos = sTrigger.find("Key 0x");
        if((Pos != string::npos) && sTrigger.length() == 8)
          return (toupper(sTrigger.at(6)) - 'A') * 16 + (toupper(sTrigger.at(7)) - 'A');
        else
          throw InvalidArgumentException("KeyboardTriggers::getScancode()",
                                        string("A key labelled '" + sTrigger + "' does not exist"));
      }

      return CharIt->second;
    }

    /// Retrieves the name of a key
    string getKeyName(unsigned char cScancode) const {
      for(CharMap::const_iterator CharIt = m_Scancodes.begin();
          CharIt != m_Scancodes.end();
          ++CharIt)
        if(CharIt->second == cScancode)
          return CharIt->first;
          
      return string("Key 0x") + lexical_cast<string>(static_cast<int>(cScancode));
    }      

  private:
    typedef std::map<string, unsigned char> CharMap;
    CharMap m_Scancodes;
} g_KeyboardTriggers;

//  //
//  KeyboardTriggerEnumerator                                                                  //
//  //
/// Keyboard trigger enumerator
/** A trigger enumerator which enumerates all keys available on the keyboard
*/
class KeyboardTriggerEnumerator :
  public InputDevice::TriggerEnumerator {
  public:
    /// Constructor
    KeyboardTriggerEnumerator() :
      m_nCurrentTrigger(0) {}

    /// Destructor
    virtual ~KeyboardTriggerEnumerator() {}

  //
  // TriggerEnumerator implementation
  //
  public:
    /// Cycle through triggers
    const InputDevice::Trigger *cycle() {
      if(m_nCurrentTrigger == 256)
        return NULL;

      m_Trigger.eKind = InputDevice::Trigger::K_BUTTON;
      m_Trigger.sName = KeyboardInputDevice::getKeyName(static_cast<unsigned char>(m_nCurrentTrigger));
      ++m_nCurrentTrigger;
      
      return &m_Trigger;
    }
    
   private:
     InputDevice::Trigger m_Trigger;
     unsigned long        m_nCurrentTrigger;
};

} // namespace

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::getScancode()                                         # //
// ############################################################################################# //
/** Returns the scancode of the specified key

    @param  sKeyName  Name of the key whose scancode to return
    @return The key's scancode
*/
unsigned char KeyboardInputDevice::getScancode(const string &sKeyName) {
  return g_KeyboardTriggers.getScancode(sKeyName);
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::getKeyName()                                          # //
// ############################################################################################# //
/** Returns the name of the specified key

    @param  cScancode  Scancode of the key whose name to return
    @return The key's name
*/
string KeyboardInputDevice::getKeyName(unsigned char cScancode) {
  return g_KeyboardTriggers.getKeyName(cScancode);
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::KeyboardInputDevice()                                 # //
// ############################################################################################# //
/** Initializes an instance of KeyboardInputDevice

    @param  TheKernel  Kernel to which the device belongs
*/
KeyboardInputDevice::KeyboardInputDevice(Kernel &TheKernel) :
  m_Kernel(TheKernel),
  m_sName("Default Keyboard") {

  Application &App = TheKernel.getApplication();
  App.OnKeyDown.connect(sigc::mem_fun(this, &KeyboardInputDevice::onKeyDown));
  App.OnKeyUp.connect(sigc::mem_fun(this, &KeyboardInputDevice::onKeyUp));
  App.OnChar.connect(sigc::mem_fun(this, &KeyboardInputDevice::onChar));
  //TheKernel.OnHeartBeat.connect(SigC::slot(*this, KeyboardInputDevice::poll));
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::~KeyboardInputDevice()                                # //
// ############################################################################################# //
/** Releases an instance of KeyboardInputDevice
*/
KeyboardInputDevice::~KeyboardInputDevice() {}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::getType()                                             # //
// ############################################################################################# //
/** Returns the type of the input device. This is only intended for
    informational purposes

    @return The device's type
*/
InputDevice::Type KeyboardInputDevice::getType() const {
  return InputDevice::T_KEYBOARD;
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::getName()                                             # //
// ############################################################################################# //
/** Returns a new enumerator over all triggers of the input device.
    A trigger is an abstract representation of an axis, a key or
    any other kind of button on an input device.

    @return A new enumerator over all triggers
*/
shared_ptr<InputDevice::TriggerEnumerator> KeyboardInputDevice::enumTriggers() const {
  return shared_ptr<InputDevice::TriggerEnumerator>(
    new KeyboardTriggerEnumerator()
  );
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::onKeyDown()                                           # //
// ############################################################################################# //
void KeyboardInputDevice::onKeyDown(unsigned char cKey) {
  m_Kernel.getInputServer()->generateInput(InputReceiver::Event(
    InputReceiver::Event::T_KEYDOWN,
    cKey
  ));
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::onKeyUp()                                             # //
// ############################################################################################# //
void KeyboardInputDevice::onKeyUp(unsigned char cKey) {
  m_Kernel.getInputServer()->generateInput(InputReceiver::Event(
    InputReceiver::Event::T_KEYUP,
    cKey
  ));
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::onChar()                                              # //
// ############################################################################################# //
void KeyboardInputDevice::onChar(wchar_t wChar) {
  m_Kernel.getInputServer()->generateInput(InputReceiver::Event(
    InputReceiver::Event::T_CHAR,
    wChar
  ));
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::poll()                                                # //
// ############################################################################################# //
/** Should be called regularly to update the device's state
*/
void KeyboardInputDevice::poll() {
  for(size_t ControlIndex = 0; ControlIndex < m_Controls.size(); ++ControlIndex)
    m_Controls[ControlIndex].fValue = 0.0f;
    
  for(size_t BindingIndex = 0; BindingIndex < m_Bindings.size(); ++BindingIndex) {
    bool bDown = !!(::GetAsyncKeyState(
      ::MapVirtualKey(m_Bindings[BindingIndex].Scancode, 1)
    ) & 0x8000);
  
    switch(m_Bindings[BindingIndex].eInterposition) {
      case Trigger::I_DIRECT: {
        if(bDown)
          m_Controls[m_Bindings[BindingIndex].ControlIndex].fValue = 1.0f;

        break;
      }
      case Trigger::I_DOWN: {
        if(!m_Bindings[BindingIndex].bPreviousState && bDown)
          m_Controls[m_Bindings[BindingIndex].ControlIndex].fValue = 1.0f;

        m_Bindings[BindingIndex].bPreviousState = bDown;
      }
      case Trigger::I_UP: {
        if(m_Bindings[BindingIndex].bPreviousState && !bDown)
          m_Controls[m_Bindings[BindingIndex].ControlIndex].fValue = 1.0f;

        m_Bindings[BindingIndex].bPreviousState = bDown;
      }
    }
  }
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::bind()                                                # //
// ############################################################################################# //
/** Adds a control to the input device
    
    @param  sName     Name of the control
    @param  sTrigger  Trigger to be captured
*/
void KeyboardInputDevice::bind(const string &sName, const string &sAction) {
  size_t Scancode = getScancode(Trigger::getTrigger(sAction));
  Trigger::Interposition eInterposition = Trigger::getInterposition(sAction);

  size_t ControlIndex;
  for(ControlIndex = 0; ControlIndex < m_Controls.size(); ++ControlIndex)
    if(m_Controls[ControlIndex].sName == sName)
      break;

  if(ControlIndex == m_Controls.size()) {
    m_Controls.resize(ControlIndex + 1);
    m_Controls[ControlIndex].sName = sName;
    m_Controls[ControlIndex].fValue = 0.0f;
  } else {
    for(size_t BindingIndex = 0; BindingIndex < m_Bindings.size(); ++BindingIndex)
      if((m_Bindings[BindingIndex].ControlIndex == ControlIndex) &&
         (m_Bindings[BindingIndex].Scancode == Scancode) &&
         (m_Bindings[BindingIndex].eInterposition == eInterposition))
        throw FailedException(
          "Nuclex::Input::KeyboardInputDevice::bind()",
          "A control with the same name and the same action has already been registered"
        );
  }
  
  size_t BindingIndex = m_Bindings.size();
  m_Bindings.resize(BindingIndex + 1);
  m_Bindings[BindingIndex].Scancode = Scancode;
  m_Bindings[BindingIndex].bPreviousState = false;
  m_Bindings[BindingIndex].eInterposition = eInterposition;
  m_Bindings[BindingIndex].ControlIndex = ControlIndex;
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::unbind()                                              # //
// ############################################################################################# //
/** Removes a control from the input device

    @param  sName     Name of the control to remove
    @param  sTrigger  Trigger of the control to remove
*/
void KeyboardInputDevice::unbind(const string &sName, const string &sAction) {
  size_t Scancode = getScancode(Trigger::getTrigger(sAction));
  Trigger::Interposition eInterposition = Trigger::getInterposition(sAction);

  size_t ControlIndex;
  for(ControlIndex = 0; ControlIndex < m_Controls.size(); ++ControlIndex)
    if(m_Controls[ControlIndex].sName == sName)
      break;

  if(ControlIndex == m_Controls.size())
    throw FailedException(
      "Nuclex::Input::KeyboardInputDevice::unbind()",
      "A control with the specified name and action was not found"
    );

  for(size_t BindingIndex = 0; BindingIndex < m_Bindings.size(); ++BindingIndex) {
    if((m_Bindings[BindingIndex].ControlIndex == ControlIndex) &&
        (m_Bindings[BindingIndex].Scancode == Scancode) &&
        (m_Bindings[BindingIndex].eInterposition == eInterposition)) {
      m_Bindings[BindingIndex] = m_Bindings[m_Bindings.size() - 1];

      if(m_Bindings.size() > 0)
        m_Bindings.resize(m_Bindings.size() - 1);
      else
        m_Bindings.clear();

      return;
    }
  }
  
  throw FailedException(
    "Nuclex::Input::KeyboardInputDevice::unbind()",
    "A control with the specified name and action was not found"
  );
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::clearBindings()                                       # //
// ############################################################################################# //
/** Removes all controls from the input device
*/
void KeyboardInputDevice::clearBindings() {
  m_Bindings.clear();
  m_Controls.clear();
}

// ############################################################################################# //
// # Nuclex::Input::KeyboardInputDevice::getState()                                            # //
// ############################################################################################# //
/** Returns the current state of a registered control

    @param  sName  Name of the control whose state to retrieve
    @return The control's current state
*/
real KeyboardInputDevice::getState(const string &sName) const {
  for(size_t ControlIndex = 0; ControlIndex < m_Controls.size(); ++ControlIndex)
    if(m_Controls[ControlIndex].sName == sName)
      return m_Controls[ControlIndex].fValue;

  return 0.0f;
}
