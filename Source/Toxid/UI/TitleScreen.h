//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_UI_TITLESCREEN_H
#define TOXID_UI_TITLESCREEN_H

#include "Toxid/Toxid.h"
#include "Nuclex/GUI/DesktopWindow.h"
#include "Nuclex/GUI/ButtonWidget.h"
#include "Toxid/UI/UIServer.h"
#include "SigC++/SigC++.h"

namespace Toxid {

//  //
//  Toxid::TitleScreen                                                                         //
//  //
class TitleScreen :
  public GUI::DesktopWindow,
  public sigc::trackable {
  public:
    /// Constructor
    TitleScreen();
    /// Destructor
    virtual ~TitleScreen();

  //
  // Window implementation
  //
  public:
    void draw(Video::VertexDrawer &VD, GUI::Theme &T);
    
  private:
    void onStart();
    void onSettings();
    void onControls();
    void onQuit();
    void onCloseDialog(bool);
    void onCloseStartDialog(bool);
    void enableControls(bool bEnable = true);

    shared_ptr<GUI::ButtonWidget> m_spStartGame;      ///< Start game button
    shared_ptr<GUI::ButtonWidget> m_spSettings;       ///< Settings button
    shared_ptr<GUI::ButtonWidget> m_spControls;       ///< Controls button
    shared_ptr<GUI::ButtonWidget> m_spQuit;           ///< Quit button
};

} // namespace Toxid

#endif // TOXID_UI_TITLESCREEN_H