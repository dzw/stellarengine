//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Input.h - Input management system                                         //
// ### # #      ###                                                                            //
// # ### #      ###  Everything related to realtime user input                                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_INPUT_INPUT_H
#define NUCLEX_INPUT_INPUT_H

namespace Nuclex {

/// Input management
/** Nuclex provides a complete system for abstracting input generators (keyboards, mice,
    etc.) from actual input methods (like 'fire', 'jump' etc.) by allowing the user to
    bind keys, buttons or axes to the input methods, which will then be triggered whenever
    the bound key is hit or the bound axis is moved.
    
    By using snapshots of the input devices, which are generated at a fixed interval,
    nuclex' input system is completely framerate independent, allowing the player to
    perform exact targetting even at very low framerates, as long as the input device
    supports time-stamped input events or can be polled in a thread.
    
    Terminology
    - InputDevice - an input device represents a physcially attached input device like
                    a keyboard, mouse or joystick
    - Trigger - a button, axis or slider of an input device
    - Modifier - how a button, axis or slider should signal, eg. once when it has been
                 pressed, once when it has been released or as long as it is held
    - Control - Abstract form of input received by the game. For example, there might
                be a control "fire", a control "move_left" and "move_right"
    - Action - Combination of a trigger and a modifier, describing what exactly
               the user has to do on the input device.
    - Binding - The association of an action with a control. Multiple actions can be
                bound to the same control
*/
namespace Input {

const size_t MaxSnapshotHistory = 100;

} // namespace Input

} // namespace Nuclex

#endif // NUCLEX_INPUT_INPUT_H
