//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudioServer.h - Audio device management                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Manages audio devices supplied through plugins                            //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIOSERVER_H
#define NUCLEX_AUDIO_AUDIOSERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Audio/AudioDriver.h"
#include "Nuclex/Support/String.h"

#include <map>

namespace Nuclex { namespace Audio {

//  //
//  Nuclex::Audio::AudioServer                                                                 //
//  //
/// Nuclex audio server
/** 
*/
class AudioServer {
  public:
    /// AudioDriver enumerator
    class DriverEnumerator;

    /// Destructor
    NUCLEX_API virtual ~AudioServer();

  //
  // AudioServer implementation
  //
  public:
    /// Retrieve device
    NUCLEX_API const shared_ptr<AudioDriver> &getDriver(const string &sName) const;
    /// Add device
    NUCLEX_API void addDriver(const string &sName, const shared_ptr<AudioDriver> &spDriver);
    /// Remove device
    NUCLEX_API void removeDriver(const string &sName);
    /// Remove all devices
    NUCLEX_API void clearDrivers();
    /// Get audio device enumerator
    NUCLEX_API shared_ptr<DriverEnumerator> enumDrivers();
    
  private:
    typedef std::map<string, shared_ptr<AudioDriver> > DriverMap;

    DriverMap m_Drivers;                              ///< Map of devices
};

//  //
//  Nuclex::Audio::AudioServer::DriverEnumerator                                               //
//  //
/** Enumerates a list of audio devices
*/
class AudioServer::DriverEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of DriverEnumerator
    */
    NUCLEX_API virtual ~DriverEnumerator() {}

  //
  // DriverEnumerator implementation
  //
  public:
    NUCLEX_API virtual bool next() = 0;
    NUCLEX_API virtual const shared_ptr<AudioDriver> &get() const = 0;
};

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_AUDIOSERVER_H
