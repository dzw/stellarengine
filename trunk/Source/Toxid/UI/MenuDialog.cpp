//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/MenuDialog.h"
#include "Toxid/UI/UIServer.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::MenuDialog::MenuDialog()                                               Constructor # //
// ############################################################################################# //
MenuDialog::MenuDialog() :
  Window(centerDialog(Point2<float>(100, 160))),
  m_spResume(new GUI::ButtonWidget(Box2<float>(10, 10, 90, 30), L"Resume")),
  m_spLoad(new GUI::ButtonWidget(Box2<float>(10, 40, 90, 60), L"Load")),
  m_spSave(new GUI::ButtonWidget(Box2<float>(10, 70, 90, 90), L"Save")),
  m_spExitGame(new GUI::ButtonWidget(Box2<float>(10, 100, 90, 120), L"Exit game")),
  m_spAbort(new GUI::ButtonWidget(Box2<float>(10, 130, 90, 150), L"Abort level")) {
  
  addWidget("Resume", m_spResume);
  addWidget("Load", m_spLoad);
  addWidget("Save", m_spSave);
  addWidget("ExitGame", m_spExitGame);
  addWidget("Abort", m_spAbort);

  m_spResume->OnPress.connect(sigc::mem_fun(this, &MenuDialog::onResume));
  m_spLoad->OnPress.connect(sigc::mem_fun(this, &MenuDialog::onLoad));
  m_spSave->OnPress.connect(sigc::mem_fun(this, &MenuDialog::onSave));
  m_spExitGame->OnPress.connect(sigc::mem_fun(this, &MenuDialog::onExitGame));
  m_spAbort->OnPress.connect(sigc::mem_fun(this, &MenuDialog::onAbort));
}

// ############################################################################################# //
// # Toxid::MenuDialog::~MenuDialog()                                               Destructor # //
// ############################################################################################# //
MenuDialog::~MenuDialog() {
}

// ############################################################################################# //
// # Toxid::MenuDialog::onResume()                                                             # //
// ############################################################################################# //
void MenuDialog::onResume() {
  onClose();
}

// ############################################################################################# //
// # Toxid::MenuDialog::onLoad()                                                               # //
// ############################################################################################# //
void MenuDialog::onLoad() {
}

// ############################################################################################# //
// # Toxid::MenuDialog::onSave()                                                               # //
// ############################################################################################# //
void MenuDialog::onSave() {
}

// ############################################################################################# //
// # Toxid::MenuDialog::onExitGame()                                                           # //
// ############################################################################################# //
void MenuDialog::onExitGame() {
}

// ############################################################################################# //
// # Toxid::MenuDialog::onAbort()                                                              # //
// ############################################################################################# //
void MenuDialog::onAbort() {
}
