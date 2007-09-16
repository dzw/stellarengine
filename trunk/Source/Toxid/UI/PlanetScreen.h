//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # PlanetScreen.h - Toxid title screen                                       //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_UI_PLANETSCREEN_H
#define TOXID_UI_PLANETSCREEN_H

#include "Toxid/Toxid.h"
#include "Nuclex/GUI/DesktopWindow.h"
#include "Nuclex/GUI/ButtonWidget.h"
#include "Toxid/UI/UIServer.h"
#include "SigC++/SigC++.h"

namespace Toxid {

//  //
//  Toxid::PlanetScreen                                                                        //
//  //
class PlanetScreen :
  public GUI::DesktopWindow,
  public sigc::trackable {
  public:
    /// Constructor
    PlanetScreen();
    /// Destructor
    virtual ~PlanetScreen();

  //
  // Window implementation
  //
  public:
    void draw(Video::VertexDrawer &VD, GUI::Theme &T);
    
  private:
    void onBack();
    void onMenu();
    void onCloseDialog();
    void enableControls(bool bEnable = true);

    shared_ptr<GUI::ButtonWidget> m_spBack;           ///< Back button
    shared_ptr<GUI::ButtonWidget> m_spMenu;           ///< Menu button
};

} // namespace Toxid

#endif // TOXID_UI_PLANETSCREEN_H