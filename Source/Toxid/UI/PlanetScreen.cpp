//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # PlanetScreen.cpp - Toxid title screen                                     //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/PlanetScreen.h"
#include "Toxid/Main.h"
#include "Nuclex/GUI/GUIServer.h"
#include "Toxid/UI/TitleScreen.h"
#include "Toxid/UI/MenuDialog.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::PlanetScreen::PlanetScreen()                                           Constructor # //
// ############################################################################################# //
PlanetScreen::PlanetScreen() :
  m_spBack(new GUI::ButtonWidget(Box2<float>(), L"Back")),
  m_spMenu(new GUI::ButtonWidget(Box2<float>(), L"Menu")) {
  
  m_spBack->OnPress.connect(sigc::mem_fun(this, &PlanetScreen::onBack));
  m_spMenu->OnPress.connect(sigc::mem_fun(this, &PlanetScreen::onMenu));
  
  addWidget("Back", m_spBack);
  addWidget("Menu", m_spMenu);
}

// ############################################################################################# //
// # Toxid::PlanetScreen::~PlanetScreen()                                           Destructor # //
// ############################################################################################# //
PlanetScreen::~PlanetScreen() {
}

// ############################################################################################# //
// # Toxid::PlanetScreen::draw()                                                               # //
// ############################################################################################# //
void PlanetScreen::draw(Video::VertexDrawer &VD, GUI::Theme &T) {
//  setRegion(Box2<float>(Point2<float>(), VD.getScreenSize());

  m_spBack->setRegion(Box2<float>(
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(110, 30),
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(10, 10)
  ));
  m_spMenu->setRegion(Box2<float>(
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(220, 30),
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(120, 10)
  ));

  DesktopWindow::draw(VD, T);
}
  
// ############################################################################################# //
// # Toxid::PlanetScreen::onBack()                                                             # //
// ############################################################################################# //
void PlanetScreen::onBack() {
  Main::getInstance().getUIServer().setScreen(
    shared_ptr<GUI::DesktopWindow>(new TitleScreen())
  );
}

// ############################################################################################# //
// # Toxid::PlanetScreen::onMenu()                                                             # //
// ############################################################################################# //
void PlanetScreen::onMenu() {
  if(getWindowCount() == 0) {
    enableControls(false);
    shared_ptr<MenuDialog> spMenuDialog(new MenuDialog());
    spMenuDialog->onClose.connect(sigc::mem_fun(this, &PlanetScreen::onCloseDialog));
    addWindow("Menu", spMenuDialog);
  }
}

// ############################################################################################# //
// # Toxid::PlanetScreen::oncloseDialog()                                                      # //
// ############################################################################################# //
void PlanetScreen::onCloseDialog() {
  clearWindows();
  enableControls();
}

// ############################################################################################# //
// # Toxid::PlanetScreen::enableControls()                                                     # //
// ############################################################################################# //
void PlanetScreen::enableControls(bool bEnable) {
  m_spMenu->setEnabled(bEnable);
  m_spBack->setEnabled(bEnable);
}
