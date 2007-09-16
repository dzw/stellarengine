//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/StartDialog.h"
#include "Toxid/UI/UIServer.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::StartDialog::StartDialog()                                             Constructor # //
// ############################################################################################# //
StartDialog::StartDialog() :
  Window(centerDialog(Point2<float>(480, 320))),
  m_spTitle(new GUI::TextWidget(Box2<float>(10, 10, 470, 30), L"Ich brauche dringend eine Liste!")),
  m_spSlider(new GUI::SliderWidget(Box2<float>(450, 60, 470, 280), 0, 1, 0, GUI::SliderWidget::O_VERTICAL)),
  m_spOk(new GUI::ButtonWidget(Box2<float>(340, 290, 400, 310), L"Ok")),
  m_spCancel(new GUI::ButtonWidget(Box2<float>(410, 290, 470, 310), L"Cancel")) {
  
  addWidget("Title", m_spTitle);
  addWidget("Ok", m_spOk);
  addWidget("Cancel", m_spCancel);

  m_spOk->OnPress.connect(sigc::mem_fun(this, &StartDialog::onOk));
  m_spCancel->OnPress.connect(sigc::mem_fun(this, &StartDialog::onCancel));
}

// ############################################################################################# //
// # Toxid::StartDialog::~StartDialog()                                             Destructor # //
// ############################################################################################# //
StartDialog::~StartDialog() {
}

// ############################################################################################# //
// # Toxid::StartDialog::onOk()                                                                # //
// ############################################################################################# //
void StartDialog::onOk() {
  onClose(true);
}

// ############################################################################################# //
// # Toxid::StartDialog::onCancel()                                                            # //
// ############################################################################################# //
void StartDialog::onCancel() {
  onClose(false);
}