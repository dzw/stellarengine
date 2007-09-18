//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereAudioDriver.h - Audiere audio device                               //
// ### # #      ###                                                                            //
// # ### #      ###  Implementation of an audio device using the audiere library               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIEREAUDIODRIVER_H
#define NUCLEX_AUDIO_AUDIEREAUDIODRIVER_H

#include "AudierePlugin/AudierePlugin.h"
#include "Nuclex/Audio/AudioDriver.h"
#include "Nuclex/Support/String.h"

namespace Nuclex { namespace Audio {

//  //
//  Nuclex::Audio::AudiereAudioDriver                                                          //
//  //
/// Audiere audio device
/**
*/
class AudiereAudioDriver : 
  public AudioDriver {
  public:
    /// Constructor
    NUCLEXAUDIERE_API AudiereAudioDriver(const string &sName, const string &sDescription);
    /// Destructor
    NUCLEXAUDIERE_API virtual ~AudiereAudioDriver();

  //
  // AudioDriver implementation
  //
  public:
    /// Create renderer
    NUCLEXAUDIERE_API shared_ptr<AudioDevice> createDevice();

    /// Get an enumerator over all available display modes
    NUCLEXAUDIERE_API shared_ptr<OutputModeEnumerator> enumOutputModes() const;

    /// Get device name
    NUCLEXAUDIERE_API const string &getName() const;

  private:
    /// Driver name
    string m_sName;
    /// Audiere device name
    string m_sDeviceName;
    /// Audiere device description
    string m_sDeviceDescription;
};

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_AUDIEREAUDIODRIVER_H
