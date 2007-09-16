//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.cpp - Toxid title screen                                      //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/TitleScreen.h"
#include "Toxid/UI/PlanetScreen.h"
#include "Toxid/UI/SettingsDialog.h"
#include "Toxid/UI/ControlsDialog.h"
#include "Toxid/UI/StartDialog.h"
#include "Toxid/Main.h"
#include "Nuclex/GUI/GUIServer.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::TitleScreen::TitleScreen()                                             Constructor # //
// ############################################################################################# //
TitleScreen::TitleScreen() :
  m_spStartGame(new GUI::ButtonWidget(Box2<float>(), L"Start Game")),
  m_spSettings(new GUI::ButtonWidget(Box2<float>(), L"Settings")),
  m_spControls(new GUI::ButtonWidget(Box2<float>(), L"Controls")),
  m_spQuit(new GUI::ButtonWidget(Box2<float>(), L"Quit")) {
  
  m_spStartGame->OnPress.connect(sigc::mem_fun(this, &TitleScreen::onStart));
  m_spSettings->OnPress.connect(sigc::mem_fun(this, &TitleScreen::onSettings));
  m_spControls->OnPress.connect(sigc::mem_fun(this, &TitleScreen::onControls));
  m_spQuit->OnPress.connect(sigc::mem_fun(this, &TitleScreen::onQuit));
  
  addWidget("Start", m_spStartGame);
  addWidget("Settings", m_spSettings);
  addWidget("Controls", m_spControls);
  addWidget("Quit", m_spQuit);
}

// ############################################################################################# //
// # Toxid::TitleScreen::~TitleScreen()                                             Destructor # //
// ############################################################################################# //
TitleScreen::~TitleScreen() {
}

// ############################################################################################# //
// # Toxid::TitleScreen::draw()                                                                # //
// ############################################################################################# //
void TitleScreen::draw(Video::VertexDrawer &VD, GUI::Theme &T) {
//  setRegion(Box2<float>(Point2<float>(), VD.getScreenSize());

  m_spStartGame->setRegion(Box2<float>(
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(210, 120),
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(10, 100)
  ));
  m_spSettings->setRegion(Box2<float>(
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(210, 90),
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(10, 70)
  ));
  m_spControls->setRegion(Box2<float>(
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(210, 60),
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(10, 40)
  ));
  m_spQuit->setRegion(Box2<float>(
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(210, 30),
    Point2<float>(VD.getScreenSize(), StaticCastTag()) - Point2<float>(10, 10)
  ));

  DesktopWindow::draw(VD, T);
}
  
// ############################################################################################# //
// # Toxid::TitleScreen::onStart()                                                             # //
// ############################################################################################# //
void TitleScreen::onStart() {
  if(getWindowCount() == 0) {
    enableControls(false);
    shared_ptr<StartDialog> spStartDialog(new StartDialog());
    spStartDialog->onClose.connect(sigc::mem_fun(this, &TitleScreen::onCloseStartDialog));
    addWindow("Start", spStartDialog);
  }
}

// ############################################################################################# //
// # Toxid::TitleScreen::onSettings()                                                          # //
// ############################################################################################# //
void TitleScreen::onSettings() {
  if(getWindowCount() == 0) {
    enableControls(false);
    shared_ptr<SettingsDialog> spSettingsDialog(new SettingsDialog());
    spSettingsDialog->onClose.connect(sigc::mem_fun(this, &TitleScreen::onCloseDialog));
    addWindow("Settings", spSettingsDialog);
  }
}

// ############################################################################################# //
// # Toxid::TitleScreen::onControls()                                                          # //
// ############################################################################################# //
void TitleScreen::onControls() {
  if(getWindowCount() == 0) {
    enableControls(false);
    shared_ptr<ControlsDialog> spControlsDialog(new ControlsDialog());
    spControlsDialog->onClose.connect(sigc::mem_fun(this, &TitleScreen::onCloseDialog));
    addWindow("Controls", spControlsDialog);
  }
}

// ############################################################################################# //
// # Toxid::TitleScreen::onQuit()                                                              # //
// ############################################################################################# //
void TitleScreen::onQuit() {
  Kernel::getInstance().requestShutdown("User pressed the quit button");     
}

// ############################################################################################# //
// # Toxid::TitleScreen::oncloseDialog()                                                       # //
// ############################################################################################# //
void TitleScreen::onCloseDialog(bool) {
  clearWindows();
  enableControls();
}

// ############################################################################################# //
// # Toxid::TitleScreen::oncloseStartDialog()                                                  # //
// ############################################################################################# //
void TitleScreen::onCloseStartDialog(bool bOk) {
  clearWindows();
  enableControls();
  
  if(bOk)
    Main::getInstance().getUIServer().setScreen(
      shared_ptr<GUI::DesktopWindow>(new PlanetScreen())
    );
}

// ############################################################################################# //
// # Toxid::TitleScreen::enableControls()                                                      # //
// ############################################################################################# //
void TitleScreen::enableControls(bool bEnable) {
  m_spStartGame->setEnabled(bEnable);
  m_spSettings->setEnabled(bEnable);
  m_spControls->setEnabled(bEnable);
  m_spQuit->setEnabled(bEnable);
}