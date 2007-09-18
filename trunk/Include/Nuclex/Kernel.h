//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Kernel.h - Nuclex engine kernel                                           //
// ### # #      ###                                                                            //
// # ### #      ###  Kernel of the nuclex engine system                                        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_KERNEL_H
#define NUCLEX_KERNEL_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Application.h"
#include "Nuclex/Plugin.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/String.h"
#include "SigC++/SigC++.h"
#include <map>
/*
#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
*/

/// use package
#define USEPACKAGE(PACKAGE) \
	extern void PACKAGE::Interface_##PACKAGE(); \
	PACKAGE::Interface_##PACKAGE();
/// declaration package
#define DECLARATIONPACKAGE(PACKAGE) namespace PACKAGE { extern void Interface_##PACKAGE(); }
/// implement package
#define IMPLEMENTPACKAGE(PACKAGE)         \
  void Interface_##PACKAGE()                   \
  {                                         \
  Kernel::getInstance().loadPluginImm(#PACKAGE, \
  nuclexPluginVersion,					\
	nuclexPluginRegister,					\
	nuclexPluginUnregister);				\
  InitPlugin();                         \
  }											          \

namespace {

/// Custom marshal class for heartbeat accumulation
/*
class HeartBeatMarshal {
  public:
    typedef bool OutType;
    /// Get default value
    static bool default_value() { return false; }

    /// Constructor
    HeartBeatMarshal() : value_(false) {}
    /// Get resulting value
    bool &value() { return value_; }
    /// Marshal another call's return value
    bool marshal(bool newval) { value_ |= newval; return false; }

  private:
    bool value_;                                      ///< Accumulated return value
};
*/
}

namespace Nuclex {
  namespace Storage { class StorageServer; }
  namespace GUI { class GUIServer; }
  namespace Video { class VideoServer; }
  namespace Input { class InputServer; }
  namespace Audio { class AudioServer; }
  namespace Text { class TextServer; }
  namespace Scene { class SceneServer; }
  namespace Script { class ScriptServer; }
}

namespace Nuclex {

//  //
//  Nuclex::Kernel                                                                             //
//  //
/// Nuclex Kernel
/** @todo Denk dir was neues aus
*/
class Kernel {
  public:
    typedef SigC::Signal0<bool/*, HeartBeatMarshal*/> HeartBeatSignal;

    /// Message types
    enum MessageType {
      MT_NONE = 0,                                    ///< No type
      MT_INFO,                                        ///< Information
      MT_WARNING,                                     ///< Warning
      MT_ERROR,                                       ///< Error
      MT_EXCEPTION = -1                               ///< Exception
    };
    
    /// Load extension module
    NUCLEX_API static Kernel &getInstance();
    /// Emit message to all listeners
    NUCLEX_API static void logMessage(int eType, const string &sMessage);

  //
  // Kernel implementation
  //
  public:
    /// Update tick
    NUCLEX_API bool heartBeat();
    /// Request kernel shutdown
    NUCLEX_API void requestShutdown(const string &sCause = "");
    /// Load a plugin
    NUCLEX_API void loadPlugin(const string &sPlugin);
    /// Loads plugin immediacy
    NUCLEX_API void loadPluginImm(const string &sPlugin, fnVersion *pVersion, fnRegister *pRegister, fnUnregister *pUnregister);
    /// Load all plugins
    NUCLEX_API void loadAllPlugins(const string &sPath);
    /// unload all plugins
    NUCLEX_API void unloadAllPluginsImm();
    /// Load all plugins
    NUCLEX_API void loadAllPluginsImm();
    /// Unload a plugin
    NUCLEX_API void unloadPlugin(const string &sPlugin);
    /// Unload all plugins
    NUCLEX_API void unloadAllPlugins();
  
    /// Get application instance
    NUCLEX_API Application &getApplication() { return m_Application; }
    /// Get const application instance
    NUCLEX_API const Application &getApplication() const { return m_Application; }

    /// Get storage server
    NUCLEX_API const shared_ptr<Storage::StorageServer> &getStorageServer() const {
      return m_spStorageServer;
    }
    /// Get script server
    NUCLEX_API const shared_ptr<Script::ScriptServer> &getScriptServer() const {
      return m_spScriptServer;
    }
    /// Get font server
    NUCLEX_API const shared_ptr<Text::TextServer> &getTextServer() const {
      return m_spTextServer;
    }
    /// Get video server
    NUCLEX_API const shared_ptr<Video::VideoServer> &getVideoServer() const {
      return m_spVideoServer;
    }
    /// Get audio server
    NUCLEX_API const shared_ptr<Audio::AudioServer> &getAudioServer() const {
      return m_spAudioServer;
    }
    /// Get input server
    NUCLEX_API const shared_ptr<Input::InputServer> &getInputServer() const {
      return m_spInputServer;
    }
    /// Get GUI server
    NUCLEX_API const shared_ptr<GUI::GUIServer> &getGUIServer() const {
      return m_spGUIServer;
    }
    /// Get scene server
    NUCLEX_API const shared_ptr<Scene::SceneServer> &getSceneServer() const {
      return m_spSceneServer;
    }

    /// Engine heart beat    
    HeartBeatSignal OnHeartBeat;

  private:
    /// Settings for the plugin class
    struct PluginSettings {
      static string VersionFunctionName;
      static string RegisterFunctionName;
      static string UnregisterFunctionName;
      static string FileExtension;
    };
  
    /// Constructor
    Kernel(const string &sApplicationName = "Nuclex");
    /// Destructor
    ~Kernel();
    /// Not cloneable
    Kernel(const Kernel &);
    /// Not assignable
    void operator =(const Kernel &);

    /// Type of plugins used for the nuclex kernel
    typedef Plugin<Kernel, PluginSettings> KernelPlugin;
    typedef PluginImm KernelPluginImm;

    /// Map of loaded plugins
    typedef std::map<string, shared_ptr<KernelPlugin> > PluginMap;
    typedef std::map<string, shared_ptr<KernelPluginImm> > PluginMapImm;

    Application                        m_Application;      ///< Display
    bool                               m_bShutdownRequested; ///< Shutdown requested ?
    PluginMap                          m_Plugins;          ///< All plugins currently loaded
    PluginMapImm                       m_PluginsImm;
    shared_ptr<Storage::StorageServer> m_spStorageServer;  ///< Storage server
    shared_ptr<Script::ScriptServer>   m_spScriptServer;   ///< Script server
    shared_ptr<Video::VideoServer>     m_spVideoServer;    ///< Video server
    shared_ptr<Audio::AudioServer>     m_spAudioServer;    ///< Audio server
    shared_ptr<Text::TextServer>       m_spTextServer;     ///< Text server
    shared_ptr<Input::InputServer>     m_spInputServer;    ///< Input server
    shared_ptr<GUI::GUIServer>         m_spGUIServer;      ///< GUI server
    shared_ptr<Scene::SceneServer>     m_spSceneServer;    ///< Scene server
};

} // namespace Nuclex

#endif // NUCLEX_KERNEL_H
