//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## GUI.h - Graphical User Interface                                          //
// ### # #      ###                                                                            //
// # ### #      ###  Everything related to the GUI                                             //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_GUI_GUI_H
#define NUCLEX_GUI_GUI_H

namespace Nuclex {

/// Graphical User Interface
/** The Nuclex GUI system is a very basic implementation of a GUI without many of the 
    advanced possibilities you be used to from your operating system. It focuses on a highly
    customizable look & feel for all components while trying to stay conformant with
    established standards in order to avoid an additional learning curve for the end-user.
    Thus, it is ideally suited for simple to moderate ingame user interfaces.
    
    The GUI distinguishes between two types of components: windows and widgets. A window
    is a visual container for widgets and also for other windows, while a widget is
    responsible for the actual user interaction. A widget could for example be a button
    which the user can click on.
    
    There's a special type of window which serves as the root for all other components:
    the desktop window. This window is invisible and always has the same size as the entire
    screen. Any windows or widgets you wish to show on the screen have to be added as
    childs to this window.
    
    Plugins can provide new window and widget types or even entire default dialog
    implementations for you.
*/
namespace GUI {

// Nothing yet

} // namespace GUI

} // namespace Nuclex

#endif // NUCLEX_GUI_GUI_H
