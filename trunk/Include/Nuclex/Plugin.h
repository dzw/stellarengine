//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Plugin.h - Nuclex generic plugin                                          //
// ### # #      ###                                                                            //
// # ### #      ###  Encapsulates a plugin and provides access to its registration             //
// #  ## #   # ## ## functions                                                                 //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_PLUGIN_H
#define NUCLEX_PLUGIN_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Exception.h"
#include "ScopeGuard/ScopeGuard.h"

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif NUCLEX_WIN32

namespace Nuclex {

/// Function to query the module for its excpected kernel version
typedef int  fnVersion();
/// Function to register a module
typedef void fnRegister();
/// Fucntion to unregister a module
typedef void fnUnregister();

//  //
//  Nuclex::PluginImm                                                                          //
//  //
/// Load Plugin Immedicary
/** Load Plugin Immedicary
*/
class PluginImm
{
  public:
    /// Plugin load error
    NUCLEX_DECLAREEXCEPTION(FailedException, CantLoadPluginImmException);
    
    static bool isPluginFilename(const string &sFilename);

    /// Constructor
    PluginImm(const string &sFilename, fnVersion *pVersion, fnRegister *pRegister, fnUnregister *pUnregister);
    /// Destructor
    ~PluginImm();

  //
  // Plugin implementation
  //
  public:
    size_t getVersion() const { return m_pfnVersion(); }
    /// Register module
    void registerPlugin() { m_pfnRegister(); }
    /// Unregister module
    void unregisterPlugin() { m_pfnUnregister(); }

  private:
    fnVersion    *m_pfnVersion;                       ///< Nuclex version information
    fnRegister   *m_pfnRegister;                      ///< Nuclex registration method
    fnUnregister *m_pfnUnregister;                    ///< Nuclex deregistration method
#ifdef NUCLEX_WIN32
    HMODULE       m_hPlugin;                          ///< DLL module handle
#endif
};







//  //
//  Nuclex::DefaultPluginSettings                                                              //
//  //
/// Default plugin settings
/** Carries the default settings for a plugin. Use this as an example on how a PluginSettings
    structure has to look like in order to be usable as argument to the plugin template
*/
struct DefaultPluginSettings {
  /// The name of the exported version query function in plugins
  static const string VersionFunctionName;
  /// The name of the exported register function in plugins
  static const string RegisterFunctionName;
  /// The name of the exported unregister function in plugins
  static const string UnregisterFunctionName;

  /// File extension used by this kind of plugin
  static const string FileExtension;
};

//  //
//  Nuclex::Plugin                                                                             //
//  //
/// Generic plugin
/** A plugin is a shared class library which can be dynamically loaded at runtime. Upon
    loading, a plugin integrates its services into the host's subsystems by registering its
    class factories and specializd implementations to one or more subsystem managers.

    In its current implementation, a plugin directly represents a DLL or shared library
    (.so). This library has to export 3 plain C functions by which a plugin is recognized.
    These functions are used to check the plugin's compatibility, to integrate the plugin
    into its host and to remove the plugin from its host again.
    
    The names of these functions are specified through the PluginSettings type, which
    is one of the template arguments to the class. Each plugin host should use its own
    unique name for these functions in order to prevent 
    
    @param  PluginHost      Where the plugin has to register itself
    @param  PluginSettings  Used to define various plugin parameters
*/ 
template<typename PluginHost, typename PluginSettings = DefaultPluginSettings>
class Plugin {
  public:
    /// Plugin load error
    NUCLEX_DECLAREEXCEPTION(FailedException, CantLoadPluginException);
    
    typedef PluginHost PluginHost;
    typedef PluginSettings PluginSettings;
    
    static bool isPluginFilename(const string &sFilename);

    /// Constructor
    Plugin(const string &sFilename);
    /// Destructor
    ~Plugin();

  //
  // Plugin implementation
  //
  public:
    size_t getVersion() const { return m_pfnVersion(); }
    /// Register module
    void registerPlugin(PluginHost &Host) { m_pfnRegister(&Host); }
    /// Unregister module
    void unregisterPlugin(PluginHost &Host) { m_pfnUnregister(&Host); }

  private:
    /// Function to query the module for its excpected kernel version
    typedef int  fnVersion();
    /// Function to register a module
    typedef void fnRegister(PluginHost *pHost);
    /// Fucntion to unregister a module
    typedef void fnUnregister(PluginHost *pHost);

    fnVersion    *m_pfnVersion;                       ///< Nuclex version information
    fnRegister   *m_pfnRegister;                      ///< Nuclex registration method
    fnUnregister *m_pfnUnregister;                    ///< Nuclex deregistration method
#ifdef NUCLEX_WIN32
    HMODULE       m_hPlugin;                          ///< DLL module handle
#endif
};

// ############################################################################################# //
// # Nuclex::Plugin::isPluginFilename()                                                        # //
// ############################################################################################# //
template<typename PluginHost, typename PluginSettings>
bool Plugin<PluginHost, PluginSettings>::isPluginFilename(const string &sFilename) {

#ifdef NUCLEX_WIN32

  struct Helper {
    static bool endsIn(const string &sString, const string &sEnd) {
      if(sString.length() < sEnd.length())
        return false;

      size_t StartIndex = sString.length() - sEnd.length();
      for(size_t CharIndex = 0; CharIndex < sEnd.length(); ++CharIndex)
        if(tolower(sString[CharIndex + StartIndex]) != tolower(sEnd[CharIndex]))
          return false;

      return true;
    }
  };
  
  if(Helper::endsIn(sFilename, PluginSettings::FileExtension + ".dll")) {
    #ifdef _DEBUG
    return Helper::endsIn(sFilename, string("-d") + PluginSettings::FileExtension + ".dll");
    #else
    return !Helper::endsIn(sFilename, string("-d") + PluginSettings::FileExtension + ".dll");
    #endif
  } else {
    return false;
  }
  
#else
#error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Plugin::Plugin()                                                      Constructor # //
// ############################################################################################# //
/** Creates a new instance of a Plugin. Looks for a library with the specified filename and
    tries to load it. The library has to provide the version, register and unregister
    functions in order to be accepted as a valid plugin. The names of these function is
    defined by the PluginSettings template argument.
    @throws CantLoadPluginException if the Plugin cannot be loaded

    @param  sFilename  Filename from which to load the Plugin
*/
template<typename PluginHost, typename PluginSettings>
Plugin<PluginHost, PluginSettings>::Plugin(const string &sFilename) :
  m_hPlugin(0),
  m_pfnVersion(NULL),
  m_pfnRegister(NULL),
  m_pfnUnregister(NULL) {

#ifdef NUCLEX_WIN32

  // Load specified library
  m_hPlugin = ::LoadLibrary(sFilename.c_str());
  if(!m_hPlugin)
    throw CantLoadPluginException(
      "Nuclex::Plugin::Plugin()",
      "Plugin is inaccessable or not a DLL"
    );

  { ScopeGuard FreeLibrary_hPlugin = MakeGuard(FreeLibrary, m_hPlugin);
  
    // Retrieve the required function pointers
    m_pfnVersion    = reinterpret_cast<fnVersion *>(
      ::GetProcAddress(m_hPlugin, string(PluginSettings::VersionFunctionName).c_str())
    );
    m_pfnRegister   = reinterpret_cast<fnRegister *>(
      ::GetProcAddress(m_hPlugin, string(PluginSettings::RegisterFunctionName).c_str())
    );
    m_pfnUnregister = reinterpret_cast<fnUnregister *>(
      ::GetProcAddress(m_hPlugin, string(PluginSettings::UnregisterFunctionName).c_str())
    );
    if(!m_pfnVersion || !m_pfnRegister || !m_pfnUnregister)
      throw CantLoadPluginException(
        "Nuclex::Plugin::Plugin()",
        "Not a valid plugin"
      );

    // If we reached this line, the plugin is ok
    FreeLibrary_hPlugin.Dismiss();
  }

#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Plugin::~Plugin()                                                      Destructor # //
// ############################################################################################# //
/** Destroys a Plugin. The library is freed from memory,
    resulting in any further calls to methods stored in the library to
    fail.
*/
template<typename PluginHost, typename PluginSettings>
Plugin<PluginHost, PluginSettings>::~Plugin() {
#ifdef NUCLEX_WIN32
  if(m_hPlugin)
    ::FreeLibrary(m_hPlugin);
#else
  #error Not implemented yet
#endif
}

} // namespace Nuclex

#endif // NUCLEX_PLUGIN_H
