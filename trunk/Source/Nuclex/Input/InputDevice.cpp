//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## InputDevice.cpp - Input device                                            //
// ### # #      ###                                                                            //
// # ### #      ###  A device through which the user can trigger input events,                 //
// #  ## #   # ## ## like a keyboard or a mouse for example                                    //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Input/InputDevice.h"

using namespace Nuclex;
using namespace Nuclex::Input;

// ############################################################################################# //
// # Nuclex::Input::InputDevice::Trigger::getInterposition()                                   # //
// ############################################################################################# //
InputDevice::Trigger::Interposition InputDevice::Trigger::getInterposition(const string &sAction) {
  string::size_type InterpositionPos = sAction.rfind('.');
  
  if(InterpositionPos != string::npos) {
    if(sAction.substr(InterpositionPos) == ".down")
      return InputDevice::Trigger::I_DOWN;
    else if(sAction.substr(InterpositionPos) == ".up")
      return InputDevice::Trigger::I_UP;
  }

  return InputDevice::Trigger::I_DIRECT;
}

// ############################################################################################# //
// # Nuclex::Input::InputDevice::Trigger::getTrigger()                                         # //
// ############################################################################################# //
string InputDevice::Trigger::getTrigger(const string &sAction) {
  string::size_type VariationPos = sAction.rfind('.');
  
  if(VariationPos != string::npos) {
    if((sAction.substr(VariationPos) == ".down") ||
       (sAction.substr(VariationPos) == ".up") ||
       (sAction.substr(VariationPos) == ".direct") ||
       (sAction.substr(VariationPos) == "."))
      return sAction.substr(0, VariationPos);
  }
  
  return sAction;
}
