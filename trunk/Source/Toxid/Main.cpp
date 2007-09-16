//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # Toxid.cpp - Toxid main application                                        //
//   #    ###  #   #                                                                           //
//   #    ###  #   #                                                                           //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/Main.h"
#include "Nuclex/Storage/XMLSerializer.h"
#include "Nuclex/Storage/StorageServer.h"
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Audio/AudioServer.h"
#include "Nuclex/Scene/SceneServer.h"
#include "Nuclex/Scene/Model.h"
#include "Nuclex/Support/TimeSpan.h"
#include "Toxid/UI/UIServer.h"
#include "Toxid/UI/TitleScreen.h"
#include "Toxid/Map/MapServer.h"
#include "Toxid/Game/GameServer.h"
#include "Nuclex/Video/Vertex.h"
//#include "Nuclex/Video/VertexBuffer2.h"

#include "package.h"


using namespace Toxid;

Main *Main::s_pInstance = NULL;

string Main::PluginSettings::VersionFunctionName = "toxidPluginVersion";
string Main::PluginSettings::RegisterFunctionName = "toxidPluginRegister";
string Main::PluginSettings::UnregisterFunctionName = "toxidPluginUnregister";

// ############################################################################################# //
// # NuclexMain()                                                                              # //
// ############################################################################################# //
/** Program entry point

    @param  Environment  Environmental data
*/
void NuclexMain(const Environment &TheEnvironment) {
  using namespace Nuclex::Video::VertexComponents;
  using namespace Nuclex::Video;
  
  typedef Vertex3<Position<false>, DiffuseColor, TextureCoordinates<1> > PlainVertex;
  //typedef VertexBuffer2<PlainVertex> PlainVertexBuffer;
/*
  //PlainVertexBuffer MyBuffer;
  PlainVertex V1;
  SimpleVertex V2;
  
  V2.Position = Point3<float>(1, 2, 3);
  V2.Color = 0x12345678;
  V2.TexCoord = Point2<float>(4, 5);
*/

  Main(TheEnvironment).run();
}                

// ############################################################################################# //
// # Toxid::Main::Main()                                                           Constructor # //
// ############################################################################################# //
/** Initializes an instance of Main. Reads the settings from the default configuration
    file and initializes the game's subsystem servers
    
    @param  Environment  Environmental data passed from nuclex
*/
Main::Main(const Environment &Environment) :
  m_Environment(Environment) {
  //Kernel::getInstance().loadAllPlugins(Environment.getApplicationPath());
  //RegisterAllPlugin(&Kernel::getInstance());
  LoadAllPlugin();
  Kernel::getInstance().loadAllPluginsImm();

  // This class should only be instantiated once
  assert(!s_pInstance);
  s_pInstance = this;

  // Load the game's settings
  loadSettings();

  // Set up the video device
  m_spVideoDevice = m_spVideoDriver->createDevice(m_DisplayMode);
  
  // Set up the audio device, if available
  if(m_spAudioDriver)
    m_spAudioDevice = m_spAudioDriver->createDevice();

  // Now the game's subsystem servers can be created
  m_spUIServer.reset(new UIServer());
  m_spMapServer.reset(new MapServer());
  m_spGameServer.reset(new GameServer());
}

// ############################################################################################# //
// # Toxid::Main::Main()                                                           Constructor # //
// ############################################################################################# //
/** Terminates an instance of Main. The current settings are saved into the default
    configuration file and all subsystems are shut down
*/
Main::~Main() {

  // Save the game's current settings
  saveSettings();

}

// ############################################################################################# //
// # Toxid::Main::run()                                                                        # //
// ############################################################################################# //
/** Application main loop which is responsible for keeping the entire application
    running and responsive until it is terminated
*/
void Main::run() {

  // Print out which drivers we're using to help identify problems
  Kernel::logMessage(
    Kernel::MT_INFO,
    string("Using video driver: ") + m_spVideoDriver->getName()
  );
  if(m_spAudioDriver)
    Kernel::logMessage(
      Kernel::MT_INFO,
      string("Using audio driver: ") + m_spAudioDriver->getName()
    );

  //!!TEMP: Normally, we would now enter the intro state. Because no architecutre for this
  //        exists as of yet, we'll directly activate the title screen!
  m_spUIServer->setScreen(shared_ptr<GUI::DesktopWindow>(new TitleScreen()));

  // Now begin with the main loop
  Kernel::logMessage(Kernel::MT_INFO, "Entering main loop");

  TimeSpan LastTime = TimeSpan::getRunningTime();
  while(Kernel::getInstance().heartBeat()) {

    // Advance in the simulation's time
    TimeSpan ThisTime = TimeSpan::getRunningTime();
    if(ThisTime != LastTime) {
      onTimeFrame((ThisTime - LastTime).toSeconds());
      LastTime = ThisTime;
    }

    // Render a new frame
    shared_ptr<Video::VideoDevice::RenderingContext> spVideoRC =
      m_spVideoDevice->renderFrame();

    if(spVideoRC) {
      shared_ptr<Audio::AudioDevice::RenderingContext> spAudioRC;
      if(m_spAudioDevice)
        spAudioRC = m_spAudioDevice->renderFrame();

      onRenderFrame(spVideoRC, spAudioRC);
      
      m_spUIServer->renderFrame(spVideoRC, spAudioRC);
    }
  }

  Kernel::getInstance().unloadAllPluginsImm();

  Kernel::logMessage(Kernel::MT_INFO, "Main loop terminated");
}

// ############################################################################################# //
// # Toxid::Main::loadSettings()                                                               # //
// ############################################################################################# //
/** Loads the game's settings from a configuration file

    @param  sFilename  Name of the file to load from
*/
void Main::loadSettings(const string &sFilename) {
  string sVideoDriver, sAudioDriver;

  try {
    // Parse the settings file
    Storage::XMLSerializer Settings(
      stringFromStream(Kernel::getInstance().getStorageServer()->openStream(sFilename))
    );

    // All settings are stored within a scope named 'settings'
    shared_ptr<Storage::Serializer> spSettings = Settings.openScope("Settings");

    // Get the video settings
    shared_ptr<Storage::Serializer> spVideo = spSettings->openScope("Video");
    sVideoDriver = spVideo->get<string>("_Driver");

    // Read display mode from video settings
    m_DisplayMode.load(spVideo->openScope("DisplayMode"));
    
    // Get the audio settings
    shared_ptr<Storage::Serializer> spAudio = spSettings->openScope("Audio");
    sAudioDriver = spAudio->get<string>("_Driver");
  }
  catch(const ResourceException &) {
    Kernel::logMessage(Kernel::MT_WARNING,
                    "settings.xml could not be opened or is malformed. Using default settings.");
  }

  // Select the video driver to use based on the settings
  try {
    m_spVideoDriver = Kernel::getInstance().getVideoServer()->getDriver(sVideoDriver);
  }
  catch(const InvalidArgumentException &) {
    // Use the first available driver if the configured driver wasn't found
    shared_ptr<Video::VideoServer::DriverEnumerator> spEnum = 
      Kernel::getInstance().getVideoServer()->enumDrivers();

    // We absolutely need a video driver!
    if(!spEnum->next())
      throw UserLevelException("Toxid::Main::Main()", "No supported video drivers available");

    m_spVideoDriver = spEnum->get();
  }

  // Select the audio driver to use based on the settings
  try {
    m_spAudioDriver = Kernel::getInstance().getAudioServer()->getDriver(sAudioDriver);
  }
  catch(const InvalidArgumentException &) {
    // Try to use the first available driver if the configured driver wasn't found
    shared_ptr<Audio::AudioServer::DriverEnumerator> spEnum = 
      Kernel::getInstance().getAudioServer()->enumDrivers();

    if(spEnum->next())
      m_spAudioDriver = spEnum->get();
  }
}

// ############################################################################################# //
// # Toxid::Main::saveSettings()                                                               # //
// ############################################################################################# //
/** Stores the game's settings in a configuration file

    @param  sFilename  Name of the file in which to store the settings
*/
void Main::saveSettings(const string &sFilename) {
  Storage::XMLSerializer Settings;

  // All settings are stored within a scope named 'settings'
  shared_ptr<Storage::Serializer> spSettings = Settings.createScope("Settings");
  
  // Save the video settings
  shared_ptr<Storage::Serializer> spVideo = spSettings->createScope("Video");
  spVideo->set<string>("_Driver", m_spVideoDriver->getName());
  
  // Save display mode in video settings
  m_DisplayMode.save(spVideo->createScope("DisplayMode"));
  
  // Save the audio settings, if available
  shared_ptr<Storage::Serializer> spAudio = spSettings->createScope("Audio");
  if(m_spAudioDriver)
    spAudio->set<string>("_Driver", m_spAudioDriver->getName());

  // Write the saved data into the configuration file
  shared_ptr<Storage::Stream> spStream = Kernel::getInstance().getStorageServer()->openStream(
    sFilename, Storage::Stream::AM_WRITE
  );
  string sXML = Settings.getXML();
  spStream->writeData(sXML.c_str(), sXML.length());
}
