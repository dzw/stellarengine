//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_UI_CONTROLSDIALOG_H
#define TOXID_UI_CONTROLSDIALOG_H

#include "Toxid/Toxid.h"
#include "Nuclex/GUI/Window.h"
#include "Nuclex/GUI/ButtonWidget.h"
#include "Nuclex/GUI/InputWidget.h"
#include "SigC++/SigC++.h"

namespace Toxid {

//  //
//  Toxid::ControlsDialog                                                                      //
//  //
class ControlsDialog :
  public GUI::Window,
  public sigc::trackable {
  public:
    typedef SigC::Signal1<void, bool> CloseSignal;

    /// Constructor
    ControlsDialog();
    /// Destructor
    virtual ~ControlsDialog();

  //
  // ControlsDialog implementation
  //    
  public:
    CloseSignal onClose;

  private:
    void onOk();
    void onCancel();
  
    shared_ptr<GUI::ButtonWidget> m_spOk;
    shared_ptr<GUI::ButtonWidget> m_spCancel;
};

} // namespace Toxid

#endif // TOXID_UI_CONTROLSDIALOG_H