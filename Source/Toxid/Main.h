//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # Nuclex.cpp - Nuclex base header                                           //
//   #    ###  #   #                                                                           //
//   #    ###  #   # Root header to be included by all nuclex headers                          //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_MAIN_H
#define TOXID_MAIN_H

#include "Toxid/Toxid.h"
#include "NuclexMain/Main.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Video/VideoDriver.h"
#include "Nuclex/Audio/AudioDriver.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Audio/AudioDevice.h"
#include "Nuclex/Storage/Serializer.h"
#include "Nuclex/PLugin.h"
#include "SigC++/SigC++.h"
#include <memory>
#include <map>

namespace Toxid {

class UIServer;
class MapServer;
class GameServer;

//  //
//  Main                                                                                       //
//  //
/// Toxid application
/** Encapsulates the application
*/
class Main {
  public:
    typedef SigC::Signal1<void, float> TimeFrameSignal;
    typedef SigC::Signal2<
      void,
      const shared_ptr<Video::VideoDevice::RenderingContext> &,
      const shared_ptr<Audio::AudioDevice::RenderingContext> &
    > RenderFrameSignal;
  
    /// Pseudo-Singleton accessor
    static Main &getInstance() { return *s_pInstance; }

    /// Constructor
    Main(const Nuclex::Environment &Environment);
    /// Destructor
    ~Main();
    
  //
  // Main implementation
  //
  public:
    /// Main loop
    void run();
    
    /// Access UI server
    UIServer &getUIServer() { return *m_spUIServer.get(); }
    /// Access map server
    MapServer &getMapServer() { return *m_spMapServer.get(); }
    /// Access game server
    GameServer &getGameServer() { return *m_spGameServer.get(); }
    
    /// Access active video driver
    const shared_ptr<Video::VideoDriver> &getVideoDriver() { return m_spVideoDriver; }
    /// Access active audio driver
    const shared_ptr<Audio::AudioDriver> &getAudioDriver() { return m_spAudioDriver; }
    
    /// Access active video device
    const shared_ptr<Video::VideoDevice> &getVideoDevice() { return m_spVideoDevice; }
    /// Access active audio device
    const shared_ptr<Audio::AudioDevice> &getAudioDevice() { return m_spAudioDevice; }

    /// Signal for each time frame
    TimeFrameSignal onTimeFrame;
    /// Signal for each rendering frame
    RenderFrameSignal onRenderFrame;

    /// Load application settings from file
    void loadSettings(const string &sFilename = "toxid.settings.xml");
    /// Save application settings to file
    void saveSettings(const string &sFilename = "toxid.settings.xml");

    /// Set display mode to use on each startup    
    void setDefaultDisplayMode(const Video::VideoDriver::DisplayMode &DisplayMode) {
      m_DisplayMode = DisplayMode;
    }
  
  private:
    struct PluginSettings {
      static string VersionFunctionName;
      static string RegisterFunctionName;
      static string UnregisterFunctionName;
    };
    typedef Plugin<Main, PluginSettings> ToxidPlugin;
    typedef std::map<string, shared_ptr<ToxidPlugin > > PluginMap;

    /// The pseudo singleton instance
    static Main *s_pInstance;

    /// Nuclex environment informations
    const Environment &m_Environment;
    /// Loaded toxid plugins
    PluginMap m_Plugins;

    /// Selected video driver
    shared_ptr<Video::VideoDriver> m_spVideoDriver;
    /// Selected audio driver
    shared_ptr<Audio::AudioDriver> m_spAudioDriver;
    /// Display mode currently active
    Video::VideoDriver::DisplayMode m_DisplayMode;

    /// The toxid UI server
    std::auto_ptr<UIServer> m_spUIServer;
    /// The toxid map server
    std::auto_ptr<MapServer> m_spMapServer;
    /// The toxid game server
    std::auto_ptr<GameServer> m_spGameServer;

    /// Video device being used for rendering
    shared_ptr<Video::VideoDevice> m_spVideoDevice;
    /// Audio device being used for rendering
    shared_ptr<Audio::AudioDevice> m_spAudioDevice;
};

} // namespace Toxid

#endif // TOXID_MAIN_H
