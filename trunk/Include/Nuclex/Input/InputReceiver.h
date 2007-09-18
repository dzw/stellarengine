//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## InputReceiver.h - Input event receiver                                    //
// ### # #      ###                                                                            //
// # ### #      ###  Allows an object to register itself to the input server in order          //
// #  ## #   # ## ## to be notified of all standard input                                      //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_INPUT_INPUTRECEIVER_H
#define NUCLEX_INPUT_INPUTRECEIVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Point2.h"
#include "Loki/SmallObj.h"

namespace Nuclex { namespace Input {

//  //
//  Nuclex::Input::InputReceiver                                                               //
//  //
/// User input receiver
/** 
*/
class InputReceiver {
  public:
    /// Input event data
    struct Event /* : public Loki::SmallObject<LOKI_DEFAULT_THREADING, 16 * 128, 16>*/ {
      /// Input type
      enum Type {
        T_NONE,                                       ///< No input
        T_MOUSEMOVE,                                  ///< Mouse moved
        T_MOUSEBUTTONDOWN,                            ///< Mouse button pressed
        T_MOUSEBUTTONUP,                              ///< Mouse button released
        T_WHEEL,                                      ///< Mouse wheel rotated
        T_KEYDOWN,                                    ///< Key pressed
        T_KEYUP,                                      ///< Key released
        T_CHAR,                                       ///< Character input
      };

      /// Variation key type
      enum Variation {
        V_NONE = 0,                                   ///< No variation (execute default action)
        V_SHIFT = 1,                                  ///< Holding down shift key
        V_ALT = 2,                                    ///< Holding down alternate key
        V_CONTROL = 4                                 ///< Holding down control key
      };
      
      Type            eType;                          ///< Kind of input
      Variation       eVariation;                     ///< Pressed variation keys
      union {
        wchar_t       wChar;                          ///< Character being typed
        unsigned char cKey;                           ///< Key being pressed or released
        long          nButton;                        ///< Button being pressed or released
        long          nWheel;                         ///< Amount the wheel was rotated
      };
      Point2<long>    Location;                       ///< Location of the mouse cursor

      Event(Type eType, Variation eVariation = V_NONE, long nButtonOrWheel = 0,
            const Point2<long> &Location = Point2<long>()) :
        eType(eType), eVariation(eVariation), nButton(nButtonOrWheel), Location(Location) {}
      Event(Type eType, Variation eVariation = V_NONE,
            const Point2<long> &Location = Point2<long>()) :
        eType(eType), eVariation(eVariation), Location(Location) {}
      Event(Type eType, wchar_t wChar, Variation eVariation = V_NONE) :
        eType(eType), wChar(wChar), eVariation(eVariation) {}
      Event(Type eType, unsigned char cKey, Variation eVariation = V_NONE) :
        eType(eType), cKey(cKey), eVariation(eVariation) {}
    };
    
    /// Destructor
    NUCLEX_API virtual ~InputReceiver() {}

  //
  // InputReceiver implementation
  //
  public:
    /// Process user input
    NUCLEX_API virtual bool processInput(const Event &InputEvent) = 0;
};

inline bool isKeyboardEvent(const InputReceiver::Event &InputEvent) {
  return (InputEvent.eType == InputReceiver::Event::T_KEYDOWN) ||
         (InputEvent.eType == InputReceiver::Event::T_KEYUP) ||
         (InputEvent.eType == InputReceiver::Event::T_CHAR);
}
inline bool hasMouseCoordinates(const InputReceiver::Event &InputEvent) {
  return (InputEvent.eType == InputReceiver::Event::T_MOUSEBUTTONDOWN) ||
         (InputEvent.eType == InputReceiver::Event::T_MOUSEBUTTONUP) ||
         (InputEvent.eType == InputReceiver::Event::T_MOUSEMOVE);
}

}} // namespace Nuclex::Input

#endif // NUCLEX_INPUT_INPUTRECEIVER_H
