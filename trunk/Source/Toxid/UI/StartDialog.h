//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_UI_STARTDIALOG_H
#define TOXID_UI_STARTDIALOG_H

#include "Toxid/Toxid.h"
#include "Nuclex/GUI/Window.h"
#include "Nuclex/GUI/ButtonWidget.h"
#include "Nuclex/GUI/SliderWidget.h"
#include "Nuclex/GUI/TextWidget.h"
#include "SigC++/SigC++.h"

namespace Toxid {

//  //
//  Toxid::StartDialog                                                                         //
//  //
class StartDialog :
  public GUI::Window,
  public sigc::trackable {
  public:
    typedef SigC::Signal1<void, bool> CloseSignal;

    /// Constructor
    StartDialog();
    /// Destructor
    virtual ~StartDialog();

  //
  // StartDialog implementation
  //    
  public:
    CloseSignal onClose;

  private:
    void onOk();
    void onCancel();
  
    shared_ptr<GUI::ButtonWidget>               m_spOk;
    shared_ptr<GUI::ButtonWidget>               m_spCancel;
    shared_ptr<GUI::TextWidget>                 m_spTitle;
    shared_ptr<GUI::SliderWidget>               m_spSlider;
    std::vector<shared_ptr<GUI::ButtonWidget> > m_spPlanets;
};

} // namespace Toxid

#endif // TOXID_UI_STARTDIALOG_H