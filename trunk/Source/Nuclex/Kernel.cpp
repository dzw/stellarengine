//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Kernel.cpp - Nuclex engine kernel                                         //
// ### # #      ###                                                                            //
// # ### #      ###  Kernel of the nuclex engine system                                        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Kernel.h"
#include "Nuclex/PlugIn.h"

#include "Nuclex/Storage/StorageServer.h"
#include "Nuclex/Storage/DirectoryArchive.h"
#include "Nuclex/Script/ScriptServer.h"
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Audio/AudioServer.h"
#include "Nuclex/Text/TextServer.h"
#include "Nuclex/Input/InputServer.h"
#include "Nuclex/GUI/GUIServer.h"
#include "Nuclex/Scene/SceneServer.h"

#include "Nuclex/Input/KeyboardInputDevice.h"
#include "Nuclex/Input/MouseInputDevice.h"
#include <iostream>

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace Nuclex;
using namespace std;

string Kernel::PluginSettings::VersionFunctionName = "nuclexPluginVersion";
string Kernel::PluginSettings::RegisterFunctionName = "nuclexPluginRegister";
string Kernel::PluginSettings::UnregisterFunctionName = "nuclexPluginUnregister";
string Kernel::PluginSettings::FileExtension = ".plugin";

// ############################################################################################# //
// # Nuclex::Kernel::getInstance()                                                             # //
// ############################################################################################# //
/** Retrieves the singleton kernel instance

    @return The one and only kernel
*/
Kernel &Kernel::getInstance() {
  static Kernel KernelSingleton;
  return KernelSingleton;
}

// ############################################################################################# //
// # Nuclex::Kernel::Kernel()                                                      Constructor # //
// ############################################################################################# //
/** Initializes a kernel instance and prepares all subsystems
*/
Kernel::Kernel(const string &sApplicationName) :
  m_Application(sApplicationName),
  m_bShutdownRequested(false),
  m_spStorageServer(new Storage::StorageServer()),
  m_spScriptServer(new Script::ScriptServer()),
  m_spTextServer(new Text::TextServer()),
  m_spVideoServer(new Video::VideoServer()),
  m_spAudioServer(new Audio::AudioServer()),
  m_spInputServer(new Input::InputServer()),
  m_spGUIServer(new GUI::GUIServer()),
  m_spSceneServer(new Scene::SceneServer()) {

  m_spInputServer->addDevice(  
    "Default Keyboard",
    shared_ptr<Input::InputDevice>(new Input::KeyboardInputDevice(*this))
  );
  if(Input::MouseInputDevice::available())
    m_spInputServer->addDevice(  
      "Default Mouse",
      shared_ptr<Input::InputDevice>(new Input::MouseInputDevice(*this))
    );

  m_spInputServer->setReceiver(m_spGUIServer);

  logMessage(MT_INFO, "Kernel startup complete.");
}

// ############################################################################################# //
// # Nuclex::Kernel::~Kernel()                                                      Destructor # //
// ############################################################################################# //
/** Shuts down the kernel and all subsystems
*/
Kernel::~Kernel() {
  logMessage(MT_INFO, "Commencing kernel shutdown.");
  
  m_spInputServer->setReceiver(shared_ptr<Input::InputReceiver>());
}

// ############################################################################################# //
// # Nuclex::Kernel::loadPlugin()                                                              # //
// ############################################################################################# //
/** Loads a nuclex plugin. The plugins services will be available directly after this method
    has returned. Plugins can be loaded at any point in time.

    @param  sPlugin  Filename of the plugin to be loaded
*/
void Kernel::loadPlugin(const string &sPlugin) {
  PluginMap::iterator PluginIt = m_Plugins.find(sPlugin);

  // Check whether the Plugin is already loaded
  if(PluginIt != m_Plugins.end()) {
    logMessage(MT_WARNING, string("Plugin '") + sPlugin + "' was already loaded");
    return;
  }

  logMessage(MT_INFO, string("Loading plugin '") + sPlugin + "'");

  // Try loading the plugin and registering it to ourselfes
  try {
    PluginMap::iterator PluginIt = m_Plugins.insert(PluginMap::value_type(
      sPlugin,
      shared_ptr<KernelPlugin>(new KernelPlugin(sPlugin))
    )).first;

    PluginIt->second->registerPlugin(*this);
  }
  catch(const KernelPlugin::CantLoadPluginException &Exception) {
    logMessage(
      MT_WARNING,
      string("Could not load Plugin '") + sPlugin + string("': ") + Exception.what()
    );

    throw;
  }
}

// ############################################################################################# //
// # Nuclex::Kernel::loadPlugin()                                                          # //
// ############################################################################################# //
/** Loads plugin immediacy
*/
void Kernel::loadPluginImm(const string &sPlugin, fnVersion *pVersion, fnRegister *pRegister, fnUnregister *pUnregister)
{
	PluginMapImm::iterator PluginIt = m_PluginsImm.find(sPlugin);

	if(PluginIt != m_PluginsImm.end()) {
		logMessage(MT_INFO, string("Plugin '") + sPlugin + "'  was alread loaded");
    return;
	}

  logMessage(MT_INFO, string("Loading plugin '") + sPlugin + "'");

  try
  {
    PluginMapImm::iterator PluginIt = m_PluginsImm.insert(PluginMapImm::value_type(
      sPlugin,
      shared_ptr<KernelPluginImm>(new KernelPluginImm(sPlugin, pVersion, pRegister, pUnregister))
      )).first;
    
    //PluginIt->second->registerPlugin();
  }
  catch(const KernelPluginImm::CantLoadPluginImmException &Exception) {
    logMessage(
      MT_WARNING,
      string("Could not load Plugin '") + sPlugin + string("': ") + Exception.what()
    );

    throw;
  }
}

// ############################################################################################# //
// # Nuclex::Kernel::loadAllPluginsImm()                                                          # //
// ############################################################################################# //
/** Register all plugins
*/
void Kernel::loadAllPluginsImm()
{
  PluginMapImm::iterator PluginIt = m_PluginsImm.begin();
  while (PluginIt != m_PluginsImm.end())
  {
    PluginIt->second->registerPlugin();
    PluginIt++;
  }
}

// ############################################################################################# //
// # Nuclex::Kernel::unloadAllPluginsImm()                                                          # //
// ############################################################################################# //
/** unload all plugins
*/
void Kernel::unloadAllPluginsImm() {
  // Check whether the plugin in question is loaded at all
  for(PluginMapImm::reverse_iterator PluginIt = m_PluginsImm.rbegin(); 
      PluginIt != m_PluginsImm.rend();
      ++PluginIt)
    PluginIt->second->unregisterPlugin();
  
  m_PluginsImm.clear();
}

// ############################################################################################# //
// # Nuclex::Kernel::loadAllPlugins()                                                          # //
// ############################################################################################# //
/** Loads all plugins under the specified path
*/
void Kernel::loadAllPlugins(const string &sPath) {
  Storage::DirectoryArchive PluginDirectory(sPath);

  // Enumerate all files in the specified directory
  shared_ptr<Storage::Archive::StreamEnumerator> spArchiveEnum = PluginDirectory.enumStreams();
  while(spArchiveEnum->next()) {
  
    // If the filename indicates a plugin, try to load it
    if(KernelPlugin::isPluginFilename(spArchiveEnum->get().sName)) {
      try {
        loadPlugin(spArchiveEnum->get().sName);
      }
      catch(KernelPlugin::CantLoadPluginException &e) {
        logMessage(
          MT_WARNING,
          string("Plugin '") + spArchiveEnum->get().sName + "' failed to load: " + e.what()
        );
      }
    }
  }
}

// ############################################################################################# //
// # Nuclex::Kernel::unloadPlugin()                                                            # //
// ############################################################################################# //
/** Unloads a nuclex plugin. This method should only be called when the plugin is not used
    anymore. If there are still active objects which belong to the plugin, the result is
    undefined (most likely a crash as soon as any of the remaining objects are accessed).
    
    @param  sPlugIn  Filename of the module to unload
*/
void Kernel::unloadPlugin(const string &sPlugin) {
  // Check whether the plugin in question is loaded at all
  PluginMap::iterator PluginIt = m_Plugins.find(sPlugin);
  if(PluginIt == m_Plugins.end()) {
    logMessage(
      MT_WARNING,
      string("Tried to unload plugin '") + sPlugin + "', which was not loaded"
    );

    return;
  }

  // Unregister and destroy
  PluginIt->second->unregisterPlugin(*this);
  m_Plugins.erase(PluginIt);
}

// ############################################################################################# //
// # Nuclex::Kernel::unloadAllPlugins()                                                        # //
// ############################################################################################# //
/** Unloads all loaded plugins

    This method does not have to be called. All remaining plugins are unloaded automatically
    when the program terminates.
*/
void Kernel::unloadAllPlugins() {
  // Check whether the plugin in question is loaded at all
  for(PluginMap::reverse_iterator PluginIt = m_Plugins.rbegin(); 
      PluginIt != m_Plugins.rend();
      ++PluginIt)
    PluginIt->second->unregisterPlugin(*this);
  
  m_Plugins.clear();
}

// ############################################################################################# //
// # Nuclex::Kernel::logMessage()                                                              # //
// ############################################################################################# //
/** Generate a kernel message. The message will pass through any registered listener. If no
    listener requests the deletion of the message, it will finally be logged to the systems
    debugging targets, eg. to stderr or OutputDebugString() on windows. This function actually
    interprets the lowest 2 bits of the message type and prefixes the message with the
    corresponding tag.

    @param  eType     Type of the message. Message listeners may add
                      their own meaning to this type with the exception
                      of the lowest 2 bits, which are predeclared to
                      serve as severity (see Kernel::MESSAGETYPE).
    @param  sMessage  Message to emit
*/
void Kernel::logMessage(int eType, const string &sMessage) {
  string sOut(sMessage);
  
  switch(eType & 3) {
    case Kernel::MT_INFO: { sOut = string("[info]   \t") + sOut; break; }
    case Kernel::MT_WARNING: { sOut = string("[warning]\t") + sOut; break; }
    case Kernel::MT_ERROR: { sOut = string("[error]  \t") + sOut; break; }
  }

  // Output to default logging target
  clog << sOut << endl;

  // Win32: Output to windows debug log
  #if defined(NUCLEX_WIN32) && defined(_DEBUG)
  ::OutputDebugString((sOut + "\n").c_str());
  #endif
}

// ############################################################################################# //
// # Nuclex::Kernel::heartBeat()                                                               # //
// ############################################################################################# //
/** Regularly call to update all subsystems, execute the message pump, etc.
*/
bool Kernel::heartBeat() {
  if(m_bShutdownRequested)
    return false;
    
  bool bQuit = m_Application.heartBeat();
  bQuit |= OnHeartBeat();

  return !bQuit;
}

// ############################################################################################# //
// # Nuclex::Kernel::requestShutdown()                                                         # //
// ############################################################################################# //
/** Requests a shutdown, causing the next heart beat call to return false
*/
void Kernel::requestShutdown(const string &sCause) {
  m_bShutdownRequested = true;
}
