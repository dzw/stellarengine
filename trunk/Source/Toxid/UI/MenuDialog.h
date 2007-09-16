//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_UI_MENUDIALOG_H
#define TOXID_UI_MENUDIALOG_H

#include "Toxid/Toxid.h"
#include "Nuclex/GUI/Window.h"
#include "Nuclex/GUI/ButtonWidget.h"
#include "Nuclex/GUI/InputWidget.h"
#include "Nuclex/GUI/TextWidget.h"
#include "SigC++/SigC++.h"

namespace Toxid {

//  //
//  Toxid::MenuDialog                                                                          //
//  //
class MenuDialog :
  public GUI::Window,
  public sigc::trackable {
  public:
    typedef SigC::Signal0<void> CloseSignal;

    /// Constructor
    MenuDialog();
    /// Destructor
    virtual ~MenuDialog();

  //
  // MenuDialog implementation
  //    
  public:
    CloseSignal onClose;

  private:
    void onResume();
    void onLoad();
    void onSave();
    void onExitGame();
    void onAbort();
  
    shared_ptr<GUI::ButtonWidget> m_spResume;
    shared_ptr<GUI::ButtonWidget> m_spLoad;
    shared_ptr<GUI::ButtonWidget> m_spSave;
    shared_ptr<GUI::ButtonWidget> m_spExitGame;
    shared_ptr<GUI::ButtonWidget> m_spAbort;
};

} // namespace Toxid

#endif // TOXID_UI_MENUDIALOG_H