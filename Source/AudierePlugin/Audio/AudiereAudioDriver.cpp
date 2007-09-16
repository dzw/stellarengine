//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereAudioDriver.cpp - Audiere audio device                             //
// ### # #      ###                                                                            //
// # ### #      ###  Implementation of an audio device using the audiere library               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "AudierePlugin/Audio/AudiereAudioDriver.h"
#include "AudierePlugin/Audio/AudiereAudioDevice.h"

using namespace Nuclex;
using namespace Nuclex::Audio;

//  //
//  AudiereOutputModeEnumerator                                                                //
//  //
/// Driver enumerator for Audio server
/** Enumerates over all devices registered to the Audio server
*/
class AudiereOutputModeEnumerator :
  public AudioDriver::OutputModeEnumerator {
  public:
    /// Constructor
    /** Initializes an instance of CDisplayModeEnumerator

        @param  pDriverMap  Drivers to enumerate
    */
    AudiereOutputModeEnumerator() {}

    /// Destructor
    /** Destroys an instance of CDisplayModeIterator
    */
    ~AudiereOutputModeEnumerator() {}

    /// Cycle through renderers
    /** Returns the current renderer being enumerated and advances
        to the next. If no more renderers are remaning, NULL is returned

        @return The currently enumerated renderer
    */
    const AudioDriver::OutputMode *cycle() {
      return NULL;
    }

  private:
};

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDriver::AudiereAudioDriver()                       Constructor # //
// ############################################################################################# //
/** Initializes an instance of AudiereAudioDriver

    @param  sName         The name ofthe audiere device
    @param  sDescription  Human-readable description 
*/
AudiereAudioDriver::AudiereAudioDriver(const string &sName, const string &sDescription) :
  m_sDeviceName(sName),
  m_sDeviceDescription(sDescription),
  m_sName(string("Audiere using ") + sDescription) {}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDriver::~AudiereAudioDriver()                       Destructor # //
// ############################################################################################# //
/** Destroys an instance of AudiereAudioDriver
*/
AudiereAudioDriver::~AudiereAudioDriver() {}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDriver::createDevice()                                         # //
// ############################################################################################# //
/** Creates a renderer on this device. The renderer is an instance
    of the device with an output window. So the device manages vertex
    buffers, textures and other resources, while the renderer performs
    the actual drawing using these resources and handles the output
    window. This allows multiple windows to share the same resources.

    @return The created device
*/
shared_ptr<AudioDevice> AudiereAudioDriver::createDevice() {
  return shared_ptr<AudioDevice>(new AudiereAudioDevice(
    m_sDeviceName,
    OutputMode(Sound::SF_44100_16, 32)
  ));
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDriver::enumDisplayModes()                                     # //
// ############################################################################################# //
/** Can be used to enumerate all display modes the device supports

    @return A new display mode enumerator
*/
shared_ptr<AudioDriver::OutputModeEnumerator> AudiereAudioDriver::enumOutputModes() const {
  return shared_ptr<AudioDriver::OutputModeEnumerator>(new AudiereOutputModeEnumerator());
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDriver::getName()                                              # //
// ############################################################################################# //
/** Returns a human readable name uniquely describing the 
    device. Should include the graphics card and monitor used.

    @return The device's name
*/
const string &AudiereAudioDriver::getName() const {
  return m_sName;
}
