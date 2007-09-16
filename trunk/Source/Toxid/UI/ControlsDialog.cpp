//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/ControlsDialog.h"
#include "Toxid/UI/UIServer.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::ControlsDialog::ControlsDialog()                                       Constructor # //
// ############################################################################################# //
ControlsDialog::ControlsDialog() :
  Window(centerDialog(Point2<float>(480, 320))),
  m_spOk(new GUI::ButtonWidget(Box2<float>(340, 290, 400, 310), L"Ok")),
  m_spCancel(new GUI::ButtonWidget(Box2<float>(410, 290, 470, 310), L"Cancel")) {
  
  addWidget("Ok", m_spOk);
  addWidget("Cancel", m_spCancel);

  m_spOk->OnPress.connect(sigc::mem_fun(this, &ControlsDialog::onOk));
  m_spCancel->OnPress.connect(sigc::mem_fun(this, &ControlsDialog::onCancel));
}

// ############################################################################################# //
// # Toxid::ControlsDialog::~ControlsDialog()                                       Destructor # //
// ############################################################################################# //
ControlsDialog::~ControlsDialog() {
}

// ############################################################################################# //
// # Toxid::ControlsDialog::onOk()                                                             # //
// ############################################################################################# //
void ControlsDialog::onOk() {
  onClose(true);
}

// ############################################################################################# //
// # Toxid::ControlsDialog::onCancel()                                                         # //
// ############################################################################################# //
void ControlsDialog::onCancel() {
  onClose(false);
}