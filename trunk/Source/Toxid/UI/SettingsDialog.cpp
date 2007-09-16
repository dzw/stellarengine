//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/SettingsDialog.h"
#include "Toxid/UI/UIServer.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::SettingsDialog::SettingsDialog()                                       Constructor # //
// ############################################################################################# //
SettingsDialog::SettingsDialog() :
  Window(centerDialog(Point2<float>(480, 320))),
  m_spVideoDevice(new GUI::InputWidget(Box2<float>(10, 10, 470, 30), L"<Device>")),
  m_spAudioDevice(new GUI::InputWidget(Box2<float>(10, 40, 470, 60), L"<Device>")),
  m_spDisplayModes(new GUI::ListWidget(Box2<float>(10, 90, 240, 190))),
  m_spOk(new GUI::ButtonWidget(Box2<float>(340, 290, 400, 310), L"Ok")),
  m_spCancel(new GUI::ButtonWidget(Box2<float>(410, 290, 470, 310), L"Cancel"))
  {

  m_spVideoDevice->setText(unicodeFromAscii(Main::getInstance().getVideoDriver()->getName()));
  if(Main::getInstance().getAudioDriver())
    m_spAudioDevice->setText(unicodeFromAscii(Main::getInstance().getAudioDriver()->getName()));

  addWidget("VideoDevice", m_spVideoDevice);
  addWidget("AudioDevice", m_spAudioDevice);
  addWidget("DisplayModes", m_spDisplayModes);
  addWidget("Ok", m_spOk);
  addWidget("Cancel", m_spCancel);

  fillDisplayModes();

  m_spOk->OnPress.connect(sigc::mem_fun(this, &SettingsDialog::onOk));
  m_spCancel->OnPress.connect(sigc::mem_fun(this, &SettingsDialog::onCancel));
}

// ############################################################################################# //
// # Toxid::SettingsDialog::~SettingsDialog()                                       Destructor # //
// ############################################################################################# //
SettingsDialog::~SettingsDialog() {
}

// ############################################################################################# //
// # Toxid::SettingsDialog::onOk()                                                             # //
// ############################################################################################# //
void SettingsDialog::onOk() {
  onClose(true);
}

// ############################################################################################# //
// # Toxid::SettingsDialog::onCancel()                                                         # //
// ############################################################################################# //
void SettingsDialog::onCancel() {
  onClose(false);
}

// ############################################################################################# //
// # Toxid::SettingsDialog::fillDisplayModes()                                                 # //
// ############################################################################################# //
void SettingsDialog::fillDisplayModes() {
  std::vector<wstring> DisplayModes;

  shared_ptr<Video::VideoDriver::DisplayModeEnumerator> spEnumerator =
    Main::getInstance().getVideoDriver()->enumDisplayModes();
    
  while(spEnumerator->next())
    DisplayModes.push_back(unicodeFromAscii(spEnumerator->get().getName()));
  
  m_spDisplayModes->setItems(DisplayModes);
}