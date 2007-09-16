//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # GameScreen.cpp - Toxid title screen                                      //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/GameScreen.h"
#include "Toxid/Main.h"
#include "Nuclex/GUI/GUIServer.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::GameScreen::GameScreen()                                               Constructor # //
// ############################################################################################# //
GameScreen::GameScreen() :
  m_spMenu(new GUI::ButtonWidget(Box2<float>(), L"Menu")) {
  
  m_spMenu->OnPress.connect(sigc::mem_fun(this, &GameScreen::onMenu));
  
  addWidget("Menu", m_spMenu);
}

// ############################################################################################# //
// # Toxid::GameScreen::draw()                                                                 # //
// ############################################################################################# //
void GameScreen::draw(Video::VertexDrawer &VD, GUI::Theme &T) {
//  setRegion(Box2<float>(Point2<float>(), VD.getScreenSize());

  m_spMenu->setRegion(Box2<float>(
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(410, 40),
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(210, 20)
  ));

  DesktopWindow::draw(VD, T);
}
  
// ############################################################################################# //
// # Toxid::GameScreen::onMenu()                                                               # //
// ############################################################################################# //
void GameScreen::onMenu() {
  if(getWindowCount() == 0) {
/*  
    enableControls(false);
    shared_ptr<StartDialog> spStartDialog(new StartDialog());
    spStartDialog->onClose.connect(SigC::slot(*this, GameScreen::onCloseDialog));
    addWindow("Start", spStartDialog);
*/    
  }
}

// ############################################################################################# //
// # Toxid::GameScreen::oncloseDialog()                                                        # //
// ############################################################################################# //
void GameScreen::onCloseDialog(bool) {
  clearWindows();
  enableControls();
}

// ############################################################################################# //
// # Toxid::GameScreen::enableControls()                                                       # //
// ############################################################################################# //
void GameScreen::enableControls(bool bEnable) {
  m_spMenu->setEnabled(bEnable);
}