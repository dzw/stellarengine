//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_UI_UISERVER_H
#define TOXID_UI_UISERVER_H

#include "Toxid/Toxid.h"
#include "Toxid/Main.h"
#include "Nuclex/GUI/DynamicTheme.h"
#include "Nuclex/GUI/DesktopWindow.h"
#include "Nuclex/Video/VertexDrawer.h"

namespace Toxid {

namespace {

// ############################################################################################# //
// # Toxid::centerDialog()                                                                     # //
// ############################################################################################# //
inline Box2<float> centerDialog(const Point2<float> &Size) {
  Point2<float> Pos = (Point2<float>(
    Main::getInstance().getVideoDevice()->getDisplayMode().Resolution, 
    StaticCastTag()
  ) - Size) / 2;
    
  return Box2<float>(Pos, Pos + Size);
}

}

//  //
//  Toxid::UIServer                                                                            //
//  //
/// User interface server
/** Manages user interface related tasks
*/
class UIServer :
  public sigc::trackable {
  public:
    /// Constructor
    UIServer(const string &sThemeFile = "Toxid.theme.xml");

  //
  // UIServer implementation
  //
  public:
    /// Get currently active screen
    const shared_ptr<GUI::DesktopWindow> &getScreen() const;
    /// Assign another screen to become activate
    void setScreen(const shared_ptr<GUI::DesktopWindow> &spScreen);

    /// Render frame listener
    void renderFrame(
      const shared_ptr<Video::VideoDevice::RenderingContext> &spVideoRC,
      const shared_ptr<Audio::AudioDevice::RenderingContext> &spAudioRC
    );
  
  private:
    /// Theme which defines the GUI's look
    GUI::DynamicTheme m_Theme;
    /// Vertex drawer used to draw the theme
    Video::VertexDrawer m_VertexDrawer;
    /// ?
    shared_ptr<GUI::DesktopWindow> m_spPreviousScreen;
};

} // namespace Toxid

#endif // TOXID_UI_UISERVER_H