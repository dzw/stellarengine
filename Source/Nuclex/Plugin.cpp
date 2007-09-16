//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Plugin.cpp - Nuclex extension Plugin                                      //
// ### # #      ###                                                                            //
// # ### #      ###  Plugin management server                                                  //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Plugin.h"

using namespace Nuclex;

const string DefaultPluginSettings::VersionFunctionName("pluginVersion");
const string DefaultPluginSettings::RegisterFunctionName("pluginRegister");
const string DefaultPluginSettings::UnregisterFunctionName("pluginUnregister");
const string DefaultPluginSettings::FileExtension(".plugin");



// ############################################################################################# //
// # Nuclex::Plugin::isPluginFilename()                                                        # //
// ############################################################################################# //
bool PluginImm::isPluginFilename(const string &sFilename) {

/*#ifdef NUCLEX_WIN32

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
#endif*/
  
  return false;
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
PluginImm::PluginImm(const string &sFilename, fnVersion *pVersion, fnRegister *pRegister, fnUnregister *pUnregister) :
  m_hPlugin(0),
  m_pfnVersion(pVersion),
  m_pfnRegister(pRegister),
  m_pfnUnregister(pUnregister) {
}

// ############################################################################################# //
// # Nuclex::Plugin::~Plugin()                                                      Destructor # //
// ############################################################################################# //
/** Destroys a Plugin. The library is freed from memory,
    resulting in any further calls to methods stored in the library to
    fail.
*/
PluginImm::~PluginImm() {

}