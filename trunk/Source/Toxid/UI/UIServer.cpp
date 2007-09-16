//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # TitleScreen.h - Toxid title screen                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   # The title screen of the game                                              //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/UI/UIServer.h"
#include "Toxid/Main.h"
#include "Toxid/UI/TitleScreen.h"
#include "Nuclex/Input/InputServer.h"
#include "Nuclex/GUI/GUIServer.h"
#include "Nuclex/Storage/XMLSerializer.h"
#include "Nuclex/Storage/StorageServer.h"
#include "Nuclex/Video/Image.h"
#include "Nuclex/Video/VertexCache.h"
#include "Nuclex/Video/VideoServer.h"

using namespace Toxid;

Video::VideoDevice::PretransformedVertex V[] = {
  { 
    Point3<float>(10, 10, 1), 1,
    0xFFFFFFFF, 0xFFFFFFFF,
    Point2<float>(0, 0)
  },
  {
    Point3<float>(10, 20, 1), 1,
    0xFFFFFFFF, 0xFFFFFFFF,
    Point2<float>(0, 0)
  },
  {
    Point3<float>(20, 20, 1), 1,
    0xFFFFFFFF, 0xFFFFFFFF,
    Point2<float>(0, 0)
  },
  {
    Point3<float>(20, 10, 1), 1,
    0xFFFFFFFF, 0xFFFFFFFF,
    Point2<float>(0, 0)
  }
};

// ############################################################################################# //
// # Toxid::UIServer::UIServer()                                                   Constructor # //
// ############################################################################################# //
UIServer::UIServer(const string &sThemeFile) :
  m_Theme(
    Kernel::getInstance().getStorageServer(),
    Kernel::getInstance().getVideoServer(),
    Kernel::getInstance().getTextServer()
  ),
  m_VertexDrawer(Main::getInstance().getVideoDevice()) {

  Storage::XMLSerializer Theme(stringFromStream(
    Kernel::getInstance().getStorageServer()->openStream(
      sThemeFile, Storage::Stream::AM_READ
    )
  ));
  m_Theme.load(Theme.openScope("theme"));

  // Assign a cursor to be used 
  shared_ptr<Video::Image> spCursorImage = Kernel::getInstance().getVideoServer()->loadImage(
    Kernel::getInstance().getStorageServer()->openStream("silly_cursor.png")
  );
  Main::getInstance().getVideoDevice()->setCursor(spCursorImage);
};

// ############################################################################################# //
// # Toxid::UIServer::renderFrame()                                                            # //
// ############################################################################################# //
void UIServer::renderFrame(
  const shared_ptr<Video::VideoDevice::RenderingContext> &spVideoRC,
  const shared_ptr<Audio::AudioDevice::RenderingContext> &spAudioRC
) {
  m_VertexDrawer.begin(spVideoRC);
  Kernel::getInstance().getGUIServer()->draw(m_VertexDrawer, m_Theme);
  m_VertexDrawer.end();
}

// ############################################################################################# //
// # Toxid::UIServer::getScreen()                                                              # //
// ############################################################################################# //
const shared_ptr<GUI::DesktopWindow> &UIServer::getScreen() const {
  return Kernel::getInstance().getGUIServer()->getDesktopWindow();
}

// ############################################################################################# //
// # Toxid::UIServer::setScreen()                                                              # //
// ############################################################################################# //
void UIServer::setScreen(const shared_ptr<GUI::DesktopWindow> &spScreen) {
  m_spPreviousScreen = Kernel::getInstance().getGUIServer()->getDesktopWindow();
  Kernel::getInstance().getGUIServer()->setDesktopWindow(spScreen);
}

